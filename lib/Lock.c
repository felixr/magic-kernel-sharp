#include "Lock.h"

void lock_init(Lock *lock) {
  pthread_mutex_init(lock, null);
}

void lock(Lock *lock) {
  int err;

  if ((err = pthread_mutex_lock(lock)) != 0) {
    throw("lock() failed with error %d", err);
  }
}

void unlock(Lock *lock) {
  int err;

  if ((err = pthread_mutex_unlock(lock)) != 0) {
    throw("unlock() failed with error %d", err);
  }
}
