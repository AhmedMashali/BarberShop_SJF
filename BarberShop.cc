/*
 * BarberShop.cc -- Barber shop for sleeping barber 
 * problem
 */
#include <assert.h>
#include <errno.h>
#include <pthread.h>
#include <stdio.h>
#include <sys/time.h>
#include "BarberShop.h"
#include "Lock.h"
#include "CV.h"
#include "thread.h"

// Routine constructor
BarberShop::BarberShop()
{
  open = false;
  timeToClose = false;
  arrivalCount = 0;
  cutCount = 0;
}


void BarberShop::barberDay(int BrustTime[], int indexes[])
{
  // BarberDay is not an atomic action.
  // No lock. Only touch object's state 
  // by calling methods that lock.
  int cust;
  printf("Opening for the day\n");
  openStore();
  while(1){
    cust = waitForCustomer();
    if(cust == NO_CUST_CLOSING_TIME){
      printf("Closing for the day No customer\n");
      ClosingTime();
      printFinalStats();
      return;
    }
    else if ( cust == -2)  {
      printf("Closing for the day the time is over\n");  
      printFinalStats();
      return;
    }

    printf("Cut hair %d Brust time: %i\n", indexes[cust], BrustTime[cust]);
    thread_sleep(BrustTime[cust], 0); // Time to cut, changed!
    doneCutting();
  }
}

void BarberShop::openStore()
{
  lock.acquire();
  open = true;
  lock.release();
  return;
}

int BarberShop::waitForCustomer()
{
  int custId;
  lock.acquire();
  while(!(timeToClose || arrivalCount > cutCount )){     //emptyAndOpen
    wakeBarber.wait(&lock);
    
  }
  if(timeToClose){
    open = false; custId=-2;// Stop new arrivals
  }
  else if(arrivalCount > cutCount){
    custId = cutCount; //next index of threads list <------
  }
  else{
    custId = NO_CUST_CLOSING_TIME; //-1

  }
  lock.release();
  return custId;
}

void BarberShop::doneCutting()
{
  lock.acquire();
  cutCount++;
  nextCustomer.broadcast();
  lock.release();
  return;
}

void BarberShop::printFinalStats()
{
  lock.acquire();
  printf("Stats: arr=%d cut=%d \n",
         arrivalCount - 1,cutCount);
  assert(arrivalCount - 1== cutCount); 
  lock.release();
}

bool BarberShop::getHairCut()
{
  int myNumber;
  bool ret;
  lock.acquire();
  if(!open || waitingRoomFull()){
    ret = false;
  }
  else{
    // "Take a number" for FIFO service
    myNumber = ++arrivalCount;
    wakeBarber.signal();
    while(stillNeedHaircut(myNumber)){//Ensure FIFO order by letting customers leave in order they arrive
      nextCustomer.wait(&lock);
    }
    ret = true;
  }
  lock.release();
  return ret;
}

void BarberShop::ClosingTime()
{
  lock.acquire();
  timeToClose = true;
  wakeBarber.signal();
  lock.release();
}



bool BarberShop::stillNeedHaircut(int custId)
{
  // Ensure FIFO order by letting customers
  // leave in order they arrive
  if(custId > cutCount){
    return true;
  }
  else{
    return false;
  }
}

bool BarberShop::waitingRoomFull()
{
  // +1 b/c barber chair
  if(arrivalCount-cutCount == NCHAIRS+1){
    return true;
  }
  else{
    return false;
  }
}