#include "NewPing.h"

#define MAX_DISTANCE 400 // Maximum distance

// Init variables
int leftMotor[] = {3, 4};
int rightMotor[] = {5, 6};


void setup()
{
    Serial.begin(9600);
    for (int i = 0; i < 2; i++){
        pinMode(leftMotor[i], OUTPUT);
        pinMode(rightMotor[i], OUTPUT);
    }
    Serial.println("Serial initialized");
    delay(3000);
}


int leftDistance, frontDistance, rightDistance; // distance in CM

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


void Direction()
{
    if (leftDistance > rightDistance && leftDistance > frontDistance)
    {setMotors(0,0);
    delay(5);
    setMotors(200,50);}
    
    else if (rightDistance > leftDistance && rightDistance > frontDistance) 
   { setMotors(0,0);
    delay(5);
    setMotors(50,200);}
    
    
    else if (frontDistance > leftDistance && frontDistance > rightDistance) 
    
   { setMotors(100,100);
    delay(500);}
    
    
    else
    
    setMotors(0,0);
}

void setMotors(int l, int r)
{
    if (l >= 0 && r >= 0)
    {
        analogWrite(leftMotor[0], l);
        digitalWrite(leftMotor[1], LOW);
        analogWrite(rightMotor[0], r);
        digitalWrite(rightMotor[1], LOW);
    }

    else if (l < 0 && r < 0)
    {
        r = -r;
        l = -l;
        digitalWrite(leftMotor[0], LOW);
        analogWrite(leftMotor[1], l);
        digitalWrite(rightMotor[0], LOW);
        analogWrite(rightMotor[1], r);
    }

    else if (l > 0 && r < 0)
    {
        analogWrite(leftMotor[0], l);
        digitalWrite(leftMotor[1], LOW);
        digitalWrite(rightMotor[0], LOW);
        analogWrite(rightMotor[1], r);
    }

    else if (l < 0 && r > 0)
    {
        digitalWrite(leftMotor[0], LOW);
        analogWrite(leftMotor[1], l);
        analogWrite(rightMotor[0], r);
        digitalWrite(rightMotor[1], LOW);
    }

    else
    {
        digitalWrite(leftMotor[0], LOW);
        digitalWrite(leftMotor[1], LOW);
        digitalWrite(rightMotor[0], LOW);
        digitalWrite(rightMotor[1], LOW);
    }
}
void loop()
{
  updateDistance();
  Direction();
  
}
