/*
 * Lock.h -- simple mutual exclusion lock based on posix pthreads lock
 */
#ifndef _LOCK_H_
#define  _LOCK_H_
#include <pthread.h>

class Lock{
  friend class CV;
 private:
  pthread_mutex_t lock;
 public: 
  Lock();
  ~Lock();
  void acquire();
  void release();
 protected:
  pthread_mutex_t *getRawLock(); // CV needs this
};
#endif
