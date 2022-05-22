/*
 * BarberShop.h -- Barber shop for sleeping barber 
 * problem
 */
#include <stdio.h>
#ifndef _BARBERSHOP_H_
#define _BARBERSHOP_H_
#include "Lock.h"
#include "CV.h"

const int NCHAIRS = 5;
const int NO_CUST_CLOSING_TIME = -1;

class BarberShop{
 private:
  Lock lock;
  CV wakeBarber;
  CV nextCustomer;

  bool timeToClose;
  bool open;
  int arrivalCount;
  int cutCount;
  //int fullCount;
  
 public:
  BarberShop();
  ~BarberShop() {};
  void barberDay(int BrustTime[], int indexes[]); // Main loop for barber thread
  bool getHairCut(); // Called by customer thread
  void ClosingTime(); // Called by clock thread
  CV curentCustomer[10];

 private:
  void openStore();
  int waitForCustomer();
  void doneCutting();
  void printFinalStats();


  bool stillNeedHaircut(int custId);
  bool waitingRoomFull();
};
#endif
