/*
 * CV.h -- simple condition variable based on pthreads
 */
#ifndef _CV_H_
#define _CV_H_
#include <pthread.h>
#include "Lock.h"

class CV{
 private:
  pthread_cond_t cond;
 public:
  CV();
  ~CV();
  void wait(Lock *lock);
  void signal();
  void broadcast();
};
#endif
