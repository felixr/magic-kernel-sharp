#include "LockTest.h"

START

TEST(Lock, Basic) {
  Lock lock_;

  lock_init(&lock_);
  lock(&lock_);
  unlock(&lock_);
}

FINISH
