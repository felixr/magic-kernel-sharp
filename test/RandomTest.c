#include "RandomTest.h"
#include <float.h>
#include <limits.h>
#include <math.h>

#define N_ 20000000
#define SIGMAS_ 4

static double double_random_uint_(Random *random);
static double double_random_ulong_(Random *random);
static double double_random_long_(Random *random);
static char *version_(
    int n,
    double mn,
    double mx,
    bool normal,
    double (*gen)(Random *random));

START

TEST(Random, Uint) {
  EXPECT(version_(N_, 0, UINT_MAX, false, double_random_uint_));
}

TEST(Random, Ulong) {
  EXPECT(version_(N_, 0, (double)ULONG_MAX, false, double_random_ulong_));
}

TEST(Random, DoubleCC) {
  EXPECT(version_(N_, 0, 1, false, random_double_cc));
}

TEST(Random, DoubleCO) {
  EXPECT(version_(N_, 0, 1, false, random_double_co));
}

TEST(Random, DoubleOC) {
  EXPECT(version_(N_, 0, 1, false, random_double_oc));
}

TEST(Random, DoubleOO) {
  EXPECT(version_(N_, 0, 1, false, random_double_oo));
}

TEST(Random, Long) {
  EXPECT(
      version_(
        N_,
        (double)LONG_MIN,
        (double)LONG_MAX,
        false,
        double_random_long_));
}

FINISH

static char *version_(
    int n,
    double mn,
    double mx,
    bool normal,
    double (*gen)(Random *random)) {
  char *f = null;
  int i;
  double r;
  double sum;
  double sum_sq;
  double mu;
  double s_sq;
  double e_mu;
  double se_mu;
  double e_s_sq;
  double se_s_sq;
  double z;
  Random *random = null;

  random_new(&random);
  sum = sum_sq = 0;
  for (sum = 0, i = 0; i < n; i++) {
    r = gen(random);
    if (r < mn) {
      return string_new_f(&f, "Returned %g but minimum %g", r, mn);
    }
    if (r > mx) {
      return string_new_f(&f, "Returned %g but maximum %g", r, mx);
    }
    sum += r;
    sum_sq += r * r;
  }
  mu = sum / n;
  s_sq = sum_sq / (n - 1) - mu * mu;
  e_s_sq = normal ? 1 : (mx - mn) * (mx - mn) / 12;
  se_s_sq = sqrt(2 * e_s_sq * e_s_sq / (n - 1));
  e_mu = normal ? 0 : (mn + mx) / 2;
  se_mu = sqrt(e_s_sq / n);
  z = (mu - e_mu) / se_mu;
  if (fabs(z) > SIGMAS_) {
    return string_new_f(
        &f,
        "Mean %g incorrect (expected %g) at %g sigmas (actual %g sigmas)",
        mu,
        e_mu,
        SIGMAS_,
        z);
  }
  z = (s_sq - e_s_sq) / se_s_sq;
  if (fabs(z) > SIGMAS_) {
    return string_new_f(
        &f,
        "Standard deviation %g incorrect (expected %g) at %g sigmas "
        "(actual %g sigmas)",
        sqrt(s_sq),
        sqrt(e_s_sq),
        SIGMAS_,
        z);
  }
  random_delete(&random);
  return null;
}

static double double_random_uint_(Random *random) {
  return random_uint(random);
}

static double double_random_ulong_(Random *random) {
  return random_ulong(random);
}

static double double_random_long_(Random *random) {
  return random_long(random);
}
