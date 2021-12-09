/**
Network Course Project
Le Cai & Jacob Calvin
to compile: gcc minQ.c -o minQ -lm -std=c99
to run: ./ranQ lambda mu mode
Mode 1 : RSS vs Lambda
Mode 2 : RSS vs Mu
Mode 3 : RSS vs Rho
**/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define Q_LEN 10
#define MAXPACKET 10000
#define DATAPOINTS 15

float exprnd(float input)
{
    float ex = ((float) rand() / RAND_MAX);
     
    while ((ex == 0) || (ex == 1))
    {
        ex = ((float) rand() / RAND_MAX);
    }

    return((-1/input) * log(ex));    
}

void minQ(int lam, int mu)
{
  float intervalTime;
  float serviceTime;
  float rho = intervalTime / (2 * serviceTime);
  float totalWaitTime = 0;
  float lossRate = 0;
  float totalQ0Counter = 0;
  float totalQ1Counter = 0;
  int t;
  time_t toki;
  srand(time(&toki)); // use time as RNG seed
  for (t = 0; t < 10; t++)
  {
    int Q0Counter = 0;
    int Q1Counter = 0;
    int packetLost = 0;
    int Q0Sum = 0;
    int  Q1Sum = 0;
    float arrivalTime_0[10] = { 0 };
    float arrivalTime_1[10] = { 0 };
    float serviceTime_0[10] = { 0 };
    float serviceTime_1[10] = { 0 };
    float waitTime = 0;
    float totalTime = 0;
    float Q0Opening = 0;
    float Q1Opening = 0;
    for (int i = 0; i < MAXPACKET; i++)
    {
      // Get intervalTime and serviceTime for new packet
      intervalTime = exprnd(lam);
      serviceTime = exprnd(mu);
      // intervalTime = 0.1;
      // serviceTime = 0.25;
      // increment to total time counter
      totalTime += intervalTime;
      Q0Sum += Q0Counter;
      Q1Sum += Q1Counter;

      if (i == 0) // First Packet
      {
        if (rand()>=(RAND_MAX/2))
        {
          arrivalTime_0[i] = 0;
          serviceTime_0[i] = serviceTime;
          totalTime = 0; // reset totalTime as 0
          Q0Counter++;
        }
        else
        {
          arrivalTime_1[i] = 0;
          serviceTime_1[i] = serviceTime;
          totalTime = 0; // reset totalTime as 0
          Q1Counter++;
        }
      }
      else
      {
      // Random Assign Queue
      if (Q0Counter <= Q1Counter) // True: Queue 0 False: Queue 1
      {
        if (Q0Counter >= Q_LEN)
        {
          packetLost++;
        }
        else
        {
          // Queue 0
          arrivalTime_0[Q0Counter] = totalTime;
          serviceTime_0[Q0Counter] = serviceTime;
          Q0Counter++;
        }
      }
      else
      {
        // Attempt to add to Q1
        if (Q1Counter >= Q_LEN)
        {
          packetLost++;
        }
        else
        {
          arrivalTime_1[Q1Counter] = totalTime;
          serviceTime_1[Q1Counter] = serviceTime;
          Q1Counter++;
        }
      }
      }

      // Servicing
      if (Q0Counter > 0 && totalTime >= Q0Opening)
      {
        Q0Opening = totalTime + serviceTime_0[0];
        waitTime += serviceTime_0[0] + totalTime - arrivalTime_0[0];
        for (int j=0; j<Q0Counter-1; j++)
        {
          arrivalTime_0[j] = arrivalTime_0[j+1];
          serviceTime_0[j] = serviceTime_0[j+1];
        }
        Q0Counter--;
      }
      if (Q1Counter > 0 && totalTime >= Q1Opening)
      {
        Q1Opening = totalTime + serviceTime_1[0];
        waitTime += serviceTime_1[0] + totalTime - arrivalTime_1[0];
        for (int j=0; j<Q1Counter-1; j++)
        {
          arrivalTime_1[j] = arrivalTime_1[j+1];
          serviceTime_1[j] = serviceTime_1[j+1];
        }
        Q1Counter--;
      }
    }
    totalWaitTime = waitTime / (float)(MAXPACKET - packetLost);
    totalQ0Counter += (float)Q0Sum / MAXPACKET;
    totalQ1Counter += (float)Q1Sum / MAXPACKET;
    lossRate += ((float)packetLost / (float)MAXPACKET);
  }
  printf("The Packet lose rate is :%0.8f\n",lossRate/10);
  printf("The avg Queue 0 Length is %0.8f\n",totalQ0Counter/10);
  printf("The avg Queue 1 Length is %0.8f\n",totalQ1Counter/10);
  printf("The avg Sojourn time is   %0.8f\n",totalWaitTime/10);
  printf("lambda is %d\n",lam);
  printf("mu is %d\n\n",mu);
}

int main(int argc, char *argv[])
{
  int LAMBDA = atoi(argv[1]);
  int MU = atoi(argv[2]);
  int MODE = atoi(argv[3]);
  int x;

  switch(MODE) {
    case 1:
      printf("Min Queue Strategy - Arrival Rate Change\n");
      x = 0;
      do {
        minQ(LAMBDA, MU);
        LAMBDA = LAMBDA + 1;
        x++;
      } while(x < DATAPOINTS);
      break;

    case 2:
      printf("Min Queue Strategy - Service Rate Change\n");
      x = 0;
      do {
        minQ(LAMBDA, MU);
        MU = MU + 1;
        x++;
      } while(x < DATAPOINTS);
      break;

    case 3:
      printf("Min Queue Strategy - Traffic Load Change\n");
        x = 0;
        do {
          printf("rho: %f\n", ((float)LAMBDA / (2 * MU)));
          minQ(LAMBDA, MU);
          LAMBDA = LAMBDA + 1;
          x++;
        } while(x < DATAPOINTS);
        break;
  }
  return 0;
}
