#include "sinc_test.h"
#include <math.h>

#define PI_ 3.14159265358979323846264338

START

TEST(Sinc, Values) {
  int i;

  /* Test integer values.
   */
  for (i = -100; i <= +100; i++) {
    EXPECT_NEAR(i == 0 ? 1 : 0, sinc(i), 1e-15);
  }

  /* Test some other values.
   */
  EXPECT_NEAR(2 / PI_, sinc(-0.5), 1e-16);
  EXPECT_NEAR(2 / PI_, sinc(0.5), 1e-16);

  EXPECT_NEAR(2 * sqrt(2) / PI_, sinc(-0.25), 1e-15);
  EXPECT_NEAR(2 * sqrt(2) / PI_, sinc(0.25), 1e-15);

  EXPECT_NEAR(-2 / (3 * PI_), sinc(-1.5), 1e-16);
  EXPECT_NEAR(-2 / (3 * PI_), sinc(1.5), 1e-16);
}

FINISH
