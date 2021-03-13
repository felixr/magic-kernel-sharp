#include "sinc.h"
#include <math.h>

#define PI_ 3.14159265358979323846264338

double sinc(double x) {
  double pi_x;

  if (x == 0) {
    return 1;
  }

  pi_x = PI_ * x;

  return sin(pi_x) / pi_x;
}
