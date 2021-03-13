#include "normal.h"
#include <math.h>

#define SQRT2_ 1.41421356237309504880
#define TWO_ON_SQRTPI_ 1.12837916709551257390
#define ONE_ON_SQRT_TWO_PI_ 0.398942280401433
#define NORM_CENTRAL_ 0.7

double normal(double x) {
  return exp(-x * x / 2) * ONE_ON_SQRT_TWO_PI_;
}

double norminv(double y) {
  if (y <= 0) {
    return -HUGE_VAL;
  }
  if (y >= 1) {
    return HUGE_VAL;
  }

  return SQRT2_ * erfinv(2 * y - 1);
}

double erfinv(double y) {
  static const double a_[4] = {
    0.886226899, -1.645349621, 0.914624893, -0.140543331};
  static const double b_[4] = {
    -2.118377725, 1.442710462, -0.329097515, 0.012229801};
  static const double c_[4] = {
    -1.970840454, -1.624906493, 3.429567803, 1.641345311};
  static const double d_[2] = {3.543889200, 1.637067800};
  double x;
  double z;

  if (y <= -1) {
    return -HUGE_VAL;
  }
  if (y >= 1) {
    return HUGE_VAL;
  }

  if (fabs(y) <= NORM_CENTRAL_) {
    z = y * y;
    x = y * (((a_[3] * z + a_[2]) * z + a_[1]) * z + a_[0]) /
      ((((b_[3] * z + b_[2]) * z + b_[1]) * z + b_[0]) * z + 1);
  } else if (NORM_CENTRAL_ < y) {
    z = sqrt(-log((1 - y) / 2));
    x = (((c_[3] * z + c_[2]) * z + c_[1]) * z + c_[0]) /
      ((d_[1] * z + d_[0]) * z + 1);
  } else {
    z = sqrt(-log((1 + y) / 2));
    x = -(((c_[3] * z + c_[2]) * z + c_[1]) * z + c_[0]) /
      ((d_[1] * z + d_[0]) * z + 1);
  }

  x = x - (erf(x) - y) / (TWO_ON_SQRTPI_ * exp(-x * x));
  x = x - (erf(x) - y) / (TWO_ON_SQRTPI_ * exp(-x * x));

  return x;
}
