#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#define NUMPAT 1
#define NUMIN  3
#define NUMHID 7
#define NUMOUT 3
#define rando()  ((double)rand()/(RAND_MAX+1))


//void is used in data return. though you should use the data type you want to output.
void udfInput(double sonar1, double sonar2, double sonar3)
{
    int    i, j, k, p, np, op, ranpat[NUMPAT+1], epoch;
    int    NumPattern = NUMPAT, NumInput = NUMIN, NumHidden = NUMHID, NumOutput = NUMOUT;

    double Input[NUMPAT+1][NUMIN+1] = {{0, 1, 1, 5},{0, sonar1, sonar2, sonar3}};
    //All other codes are the same.................................
}
main ()
{
    //Capture sonar inputs and call udfInput() every time all the sonar values are taken.
}
