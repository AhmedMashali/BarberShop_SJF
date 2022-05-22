#include <assert.h>
#include <pthread.h>
#include "Lock.h"

Lock::Lock()
{
  int err = pthread_mutex_init(&lock, NULL);
  assert(!err);
}

Lock::~Lock()
{
  pthread_mutex_destroy(&lock);
}

void
Lock::acquire()
{
  int err = pthread_mutex_lock(&lock);
  assert(!err);
}

void
Lock::release()
{
  int err = pthread_mutex_unlock(&lock);
  assert(!err);
}


// CV needs this protected function
pthread_mutex_t *
Lock::getRawLock()
{
  return &lock;
} 
