#include <math.h>
#include "NewPing.h"

/******************************************************************
 * Network Configuration - customized per network 
 ******************************************************************/

const int PatternCount =1;
const int InputNodes = 3;
const int HiddenNodes = 7;
const int OutputNodes = 3;
const float LearningRate = 0.3;
const float Momentum = 0.9;
const float InitialWeightMax = 0.5;
const float Success = 0.0004;
float Input[PatternCount][InputNodes];
float Target[PatternCount][OutputNodes];

/******************************************************************
 * Get distance and calculate for input
 ******************************************************************/

#define MAX_DISTANCE 400 
 
 double leftDistance, frontDistance, rightDistance;
 double left,right,front;
 int lefttarget,righttarget,fronttarget;
 
NewPing leftSonar(9, 8, MAX_DISTANCE);
NewPing frontSonar (11, 10, MAX_DISTANCE);
NewPing rightSonar (13, 12, MAX_DISTANCE);

void updateDistance()
{
    Serial.println("Updating distances: ");
    leftDistance = leftSonar.ping() / US_ROUNDTRIP_CM;
    frontDistance = frontSonar.ping() / US_ROUNDTRIP_CM;
    rightDistance = rightSonar.ping() / US_ROUNDTRIP_CM;

    Serial.println("Front: " + String(frontDistance) + " cm");
    Serial.println("Left: " + String(leftDistance) + " cm");
    Serial.println("Right: " + String(rightDistance) + " cm");
}
void calculateinputdistance()
{ 
  if (leftDistance < rightDistance && leftDistance < frontDistance)
  {
    left=leftDistance/leftDistance;
    right=rightDistance/leftDistance;
    front=frontDistance/leftDistance;
    Serial.println("Front: " + String(front) + " cm");
    Serial.println("Left: " + String(left) + " cm");
    Serial.println("Right: " + String(right) + " cm");
    if(left>right&&left>front)
    {
      lefttarget=1;
      righttarget=0;
      fronttarget=0;
    }
    else if(right>left&&right>front)
    {
      lefttarget=0;
      righttarget=1;
      fronttarget=0;
    }
    else if(front>left&&front>right)
    {
      lefttarget=0;
      righttarget=0;
      fronttarget=1;
    }
    
  } 
    else if (rightDistance < leftDistance && rightDistance < frontDistance) 
    {
    left=leftDistance/rightDistance;
    right=rightDistance/rightDistance;
    front=frontDistance/rightDistance;
    Serial.println("Front: " + String(front) + " cm");
    Serial.println("Left: " + String(left) + " cm");
    Serial.println("Right: " + String(right) + " cm");
    if(left>right&&left>front)
    {
      lefttarget=1;
      righttarget=0;
      fronttarget=0;
    }
    else if(right>left&&right>front)
    {
      lefttarget=0;
      righttarget=1;
      fronttarget=0;
    }
    else if(front>left&&front>right)
    {
      lefttarget=0;
      righttarget=0;
      fronttarget=1;
    }
  } 
    else if (frontDistance < leftDistance && frontDistance < rightDistance) 
    {
    left=leftDistance/frontDistance;
    right=rightDistance/frontDistance;
    front=frontDistance/frontDistance;
    Serial.println("Front: " + String(front) + " cm");
    Serial.println("Left: " + String(left) + " cm");
    Serial.println("Right: " + String(right) + " cm");
    if(left>right&&left>front)
    {
      lefttarget=1;
      righttarget=0;
      fronttarget=0;
    }
    else if(right>left&&right>front)
    {
      lefttarget=0;
      righttarget=1;
      fronttarget=0;
    }
    else if(front>left&&front>right)
    {
      lefttarget=0;
      righttarget=0;
      fronttarget=1;
    }
  }
   }
void inputtarget()
{
  updateDistance();
  calculateinputdistance();
  
  Input[0][0] = left;
  Target[0][0] = lefttarget;
  
  Input[0][1] = right;
  Target[0][1] = righttarget;
  
  Input[0][2] = front;
  Target[0][2] = fronttarget;
  
 // Input[PatternCount][InputNodes]={left,right,front};
  //Target[PatternCount][OutputNodes]={left,right,front};
  
}

/******************************************************************
 * End Network Configuration
 ******************************************************************/


int i, j, p, q, r;
int ReportEvery1000;
int RandomizedIndex[PatternCount];
long  TrainingCycle;
float Rando;
float Error;
float Accum;


float Hidden[HiddenNodes];
float Output[OutputNodes];
float HiddenWeights[InputNodes+1][HiddenNodes];
float OutputWeights[HiddenNodes+1][OutputNodes];
float HiddenDelta[HiddenNodes];
float OutputDelta[OutputNodes];
float ChangeHiddenWeights[InputNodes+1][HiddenNodes];
float ChangeOutputWeights[HiddenNodes+1][OutputNodes];

void setup()
{
  Serial.begin(9600);
  randomSeed(analogRead(3));
  ReportEvery1000 = 1;
  for( p = 0 ; p < PatternCount ; p++ ) {    
    RandomizedIndex[p] = p ;
  }
  
}  

void loop (){

inputtarget();
/******************************************************************
* Initialize HiddenWeights and ChangeHiddenWeights 
******************************************************************/

  for( i = 0 ; i < HiddenNodes ; i++ ) {    
    for( j = 0 ; j <= InputNodes ; j++ ) { 
      ChangeHiddenWeights[j][i] = 0.0 ;
      Rando = float(random(100))/100;
      HiddenWeights[j][i] = 2.0 * ( Rando - 0.5 ) * InitialWeightMax ;
    }
  }
/******************************************************************
* Initialize OutputWeights and ChangeOutputWeights
******************************************************************/

  for( i = 0 ; i < OutputNodes ; i ++ ) {    
    for( j = 0 ; j <= HiddenNodes ; j++ ) {
      ChangeOutputWeights[j][i] = 0.0 ;  
      Rando = float(random(100))/100;        
      OutputWeights[j][i] = 2.0 * ( Rando - 0.5 ) * InitialWeightMax ;
    }
  }
  Serial.println("Initial/Untrained Outputs: ");
  toTerminal();
/******************************************************************
* Begin training 
******************************************************************/

  for( TrainingCycle = 1 ; TrainingCycle < 2147483647 ; TrainingCycle++) {    

/******************************************************************
* Randomize order of training patterns
******************************************************************/

    for( p = 0 ; p < PatternCount ; p++) {
      q = random(PatternCount);
      r = RandomizedIndex[p] ; 
      RandomizedIndex[p] = RandomizedIndex[q] ; 
      RandomizedIndex[q] = r ;
    }
    Error = 0.0 ;
/******************************************************************
* Cycle through each training pattern in the randomized order
******************************************************************/
    for( q = 0 ; q < PatternCount ; q++ ) {    
      p = RandomizedIndex[q];

/******************************************************************
* Compute hidden layer activations
******************************************************************/

      for( i = 0 ; i < HiddenNodes ; i++ ) {    
        Accum = HiddenWeights[InputNodes][i] ;
        for( j = 0 ; j < InputNodes ; j++ ) {
          Accum += Input[p][j] * HiddenWeights[j][i] ;
        }
        Hidden[i] = 1.0/(1.0 + exp(-Accum)) ;
      }

/******************************************************************
* Compute output layer activations and calculate errors
******************************************************************/

      for( i = 0 ; i < OutputNodes ; i++ ) {    
        Accum = OutputWeights[HiddenNodes][i] ;
        for( j = 0 ; j < HiddenNodes ; j++ ) {
          Accum += Hidden[j] * OutputWeights[j][i] ;
        }
        Output[i] = 1.0/(1.0 + exp(-Accum)) ;   
        OutputDelta[i] = (Target[p][i] - Output[i]) * Output[i] * (1.0 - Output[i]) ;   
        Error += 0.5 * (Target[p][i] - Output[i]) * (Target[p][i] - Output[i]) ;
      }

/******************************************************************
* Backpropagate errors to hidden layer
******************************************************************/

      for( i = 0 ; i < HiddenNodes ; i++ ) {    
        Accum = 0.0 ;
        for( j = 0 ; j < OutputNodes ; j++ ) {
          Accum += OutputWeights[i][j] * OutputDelta[j] ;
        }
        HiddenDelta[i] = Accum * Hidden[i] * (1.0 - Hidden[i]) ;
      }


/******************************************************************
* Update Inner-->Hidden Weights
******************************************************************/


      for( i = 0 ; i < HiddenNodes ; i++ ) {     
        ChangeHiddenWeights[InputNodes][i] = LearningRate * HiddenDelta[i] + Momentum * ChangeHiddenWeights[InputNodes][i] ;
        HiddenWeights[InputNodes][i] += ChangeHiddenWeights[InputNodes][i] ;
        for( j = 0 ; j < InputNodes ; j++ ) { 
          ChangeHiddenWeights[j][i] = LearningRate * Input[p][j] * HiddenDelta[i] + Momentum * ChangeHiddenWeights[j][i];
          HiddenWeights[j][i] += ChangeHiddenWeights[j][i] ;
        }
      }

/******************************************************************
* Update Hidden-->Output Weights
******************************************************************/

      for( i = 0 ; i < OutputNodes ; i ++ ) {    
        ChangeOutputWeights[HiddenNodes][i] = LearningRate * OutputDelta[i] + Momentum * ChangeOutputWeights[HiddenNodes][i] ;
        OutputWeights[HiddenNodes][i] += ChangeOutputWeights[HiddenNodes][i] ;
        for( j = 0 ; j < HiddenNodes ; j++ ) {
          ChangeOutputWeights[j][i] = LearningRate * Hidden[j] * OutputDelta[i] + Momentum * ChangeOutputWeights[j][i] ;
          OutputWeights[j][i] += ChangeOutputWeights[j][i] ;
        }
      }
    }

/******************************************************************
* Every 1000 cycles send data to terminal for display
******************************************************************/
    ReportEvery1000 = ReportEvery1000 - 1;
    if (ReportEvery1000 == 0)
    {
      Serial.println(); 
      Serial.println(); 
      Serial.print ("TrainingCycle: ");
      Serial.print (TrainingCycle);
      Serial.print ("  Error = ");
      Serial.println (Error, 5);

      toTerminal();

      if (TrainingCycle==1)
      {
        ReportEvery1000 = 999;
      }
      else
      {
        ReportEvery1000 = 1000;
      }
    }    


/******************************************************************
* If error rate is less than pre-determined threshold then end
******************************************************************/

    if( Error < Success ) break ;  
  }
  Serial.println ();
  Serial.println(); 
  Serial.print ("TrainingCycle: ");
  Serial.print (TrainingCycle);
  Serial.print ("  Error = ");
  Serial.println (Error, 5);

  toTerminal();

  Serial.println ();  
  Serial.println ();
  Serial.println ("Training Set Solved! ");
  Serial.println ("--------"); 
  Serial.println ();
  Serial.println ();  
  ReportEvery1000 = 1;
}

void toTerminal()
{

  for( p = 0 ; p < PatternCount ; p++ ) { 
    Serial.println(); 
    Serial.print ("  Training Pattern: ");
    Serial.println (p);      
    Serial.print ("  Input ");
    for( i = 0 ; i < InputNodes ; i++ ) {
      Serial.print (Input[p][i], DEC);
      Serial.print (" ");
    }
    Serial.print ("  Target ");
    for( i = 0 ; i < OutputNodes ; i++ ) {
      Serial.print (Target[p][i], DEC);
      Serial.print (" ");
    }
/******************************************************************
* Compute hidden layer activations
******************************************************************/

    for( i = 0 ; i < HiddenNodes ; i++ ) {    
      Accum = HiddenWeights[InputNodes][i] ;
      for( j = 0 ; j < InputNodes ; j++ ) {
        Accum += Input[p][j] * HiddenWeights[j][i] ;
      }
      Hidden[i] = 1.0/(1.0 + exp(-Accum)) ;
    }

/******************************************************************
* Compute output layer activations and calculate errors
******************************************************************/

    for( i = 0 ; i < OutputNodes ; i++ ) {    
      Accum = OutputWeights[HiddenNodes][i] ;
      for( j = 0 ; j < HiddenNodes ; j++ ) {
        Accum += Hidden[j] * OutputWeights[j][i] ;
      }
      Output[i] = 1.0/(1.0 + exp(-Accum)) ; 
    }
    Serial.print ("  Output ");
    for( i = 0 ; i < OutputNodes ; i++ ) {       
      Serial.print (Output[i], 5);
      Serial.print (" ");
    }
  }


}

