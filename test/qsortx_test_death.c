#include "qsortx_test_death.h"

static int c(void *a, void *b, void *x) {
  uint ua;
  uint ub;

  ua = *(uint *)a;
  ub = *(uint *)b;
  return ua < ub ? -1 : ua > ub ? +1 : 0;
}

START_DEATH

TEST(Bsearchx, FindIndex) {
  uint d[] = {0x07071968, 0x23031998, 0x24091966, 0x28032001};
  ulong n;
  uint k;

  n = sizeof(d) / sizeof(*d);
  k = 0;
  EXPECT_DEATH(
      bsearchx_find_index(&k, d, n, c, null),
      "Could not find element");
}

FINISH_DEATH
