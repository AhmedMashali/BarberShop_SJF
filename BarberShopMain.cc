#include <stdio.h>
#include "BarberShop.h"
#include "thread.h"
#include "Lock.h"
#include "CV.h"
#include <bits/stdc++.h>

#define NCUST 10
#define SIMTIME 55

/*  1- array of brust time
    2- array of indexes
    3- array of threads as customers
    4- arguments "brust time, indexes" to print
    5- create CV for each thread "customer"
    6- sorting arrays based on brust time
    7- giving signal for each thread "customer" by the order in array
*/    

int count = 0;

// Brust time of threads
int BrustTime[NCUST] = {1, 4, 5, 2, 3, 6, 9, 7, 10, 8};
// original indexes
int indexes[NCUST];

void *barberMain(void *bsPtr)
{
  BarberShop *bs = (BarberShop *)bsPtr;
  bs->barberDay(BrustTime, indexes);
  return NULL;
}

void *custMain(void *bsPtr)
{
  bool ret;
  //create CV giving wait for each thread "customer"
  CV tmpCV = *(new CV());

  BarberShop *bs = (BarberShop *)bsPtr;
  // to point to CV for each thread --> storage
  bs->curentCustomer[count] = tmpCV;
  count++;
  // make wait
  Lock lock;
  bs->curentCustomer[count].wait(&lock);
  while(1){
    ret= bs->getHairCut();
  }
}

void *clockMain(void *bsPtr)
{
  BarberShop *bs = (BarberShop *)bsPtr;
  thread_sleep(SIMTIME, 0);
  printf("CLOCK: Closing time\n");
  bs->ClosingTime();
  return NULL;
}


int main(int argc, char **argv)
{
  int ii;
  BarberShop *bs = new BarberShop();
  pthread_t barber; 
  pthread_t customers[NCUST];
  pthread_t clock;

  //giving values for indexes
  for(int i = 0; i < NCUST; i++)
  {
    indexes[i] = i;
  }

  
  pthread_create(&barber, NULL, barberMain, bs);


  for(ii = 0; ii < NCUST; ii++){
    pthread_create(&customers[ii], NULL, custMain, bs);
  }
  

  // asceinding threads based on brust time
  for (int i = 0; i < NCUST; i++)
  {
  int minP = i;
    for (int j = i + 1; j < 10; j++)
    {
      if (BrustTime[j] < BrustTime[minP])
      {
      minP = j;
      }
    }
    int t = BrustTime[i];
    BrustTime[i] = BrustTime[minP];
    BrustTime[minP] = t;
    
    pthread_t tt = customers[i];
    customers[i] = customers[minP];
    customers[minP] = t;
    
    int t2 = indexes[i];
    indexes[i] = indexes[minP];
    indexes[minP] = t2;

    CV tmpp = bs->curentCustomer[i];
    bs->curentCustomer[i] = bs->curentCustomer[minP];
    bs->curentCustomer[minP] = tmpp;

  }
  

  //giving signal for each thread
  for (int i = 0; i < NCUST; i++)
  {
    bs->curentCustomer[i].signal();
  }

  pthread_create(&clock,NULL, clockMain, bs);

  thread_join(barber);
}