#include <assert.h>
#include <pthread.h>
#include "CV.h"
#include "Lock.h"
#include <stdlib.h>

CV::CV()
{
  int err = pthread_cond_init(&cond, NULL);
  assert(!err);
}

CV::~CV()
{
  int err = pthread_cond_destroy(&cond);
  assert(!err);
}

void
CV::wait(Lock *lock)
{
  int err = pthread_cond_wait(&cond, lock->getRawLock());
  assert(!err);
}

void 
CV::signal()
{
  int err = pthread_cond_signal(&cond);
  assert(!err);
}

void 
CV::broadcast()
{
  int err = pthread_cond_broadcast(&cond);
  assert(!err);
}