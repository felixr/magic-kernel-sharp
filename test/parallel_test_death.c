#include "parallel_test_death.h"
#include <limits.h>

START_DEATH

TEST(Parallel, Memcpy) {
  /* Test the sanity limit.
   */
  EXPECT_DEATH(
      parallel_memcpy(null, null, ULONG_MAX / 11, 12),
      "n (1,676,976,733,973,595,601) cannot be greater "
      "than 768,614,336,404,564,650");
}

FINISH_DEATH
