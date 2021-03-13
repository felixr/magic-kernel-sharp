#include "erf.h"
#include <math.h>

#define GET_HIGH_WORD_(i, d) \
do { \
  ieee_double_shape_type gh_u; \
  gh_u.value = (d); \
  (i) = gh_u.parts.msw; \
} while (0)

#define SET_LOW_WORD_(d, v) \
do { \
  ieee_double_shape_type sl_u; \
  sl_u.value = (d); \
  sl_u.parts.lsw = (v); \
  (d) = sl_u.value; \
} while (0)

typedef union {
  double value;
  struct {
    uint lsw;
    uint msw;
  } parts;
} ieee_double_shape_type;

static const double tiny_ = 1e-300;
static const double half_ = 5.00000000000000000000e-01;
static const double one_ = 1.00000000000000000000e+00;
static const double two_ = 2.00000000000000000000e+00;
static const double erx_ = 8.45062911510467529297e-01;
static const double efx_ = 1.28379167095512586316e-01;
static const double efx_8 = 1.02703333676410069053e+00;
static const double pp0_ = 1.28379167095512558561e-01;
static const double pp1_ = -3.25042107247001499370e-01;
static const double pp2_ = -2.84817495755985104766e-02;
static const double pp3_ = -5.77027029648944159157e-03;
static const double pp4_ = -2.37630166566501626084e-05;
static const double qq1_ = 3.97917223959155352819e-01;
static const double qq2_ = 6.50222499887672944485e-02;
static const double qq3_ = 5.08130628187576562776e-03;
static const double qq4_ = 1.32494738004321644526e-04;
static const double qq5_ = -3.96022827877536812320e-06;
static const double pa0_ = -2.36211856075265944077e-03;
static const double pa1_ = 4.14856118683748331666e-01;
static const double pa2_ = -3.72207876035701323847e-01;
static const double pa3_ = 3.18346619901161753674e-01;
static const double pa4_ = -1.10894694282396677476e-01;
static const double pa5_ = 3.54783043256182359371e-02;
static const double pa6_ = -2.16637559486879084300e-03;
static const double qa1_ = 1.06420880400844228286e-01;
static const double qa2_ = 5.40397917702171048937e-01;
static const double qa3_ = 7.18286544141962662868e-02;
static const double qa4_ = 1.26171219808761642112e-01;
static const double qa5_ = 1.36370839120290507362e-02;
static const double qa6_ = 1.19844998467991074170e-02;
static const double ra0_ = -9.86494403484714822705e-03;
static const double ra1_ = -6.93858572707181764372e-01;
static const double ra2_ = -1.05586262253232909814e+01;
static const double ra3_ = -6.23753324503260060396e+01;
static const double ra4_ = -1.62396669462573470355e+02;
static const double ra5_ = -1.84605092906711035994e+02;
static const double ra6_ = -8.12874355063065934246e+01;
static const double ra7_ = -9.81432934416914548592e+00;
static const double sa1_ = 1.96512716674392571292e+01;
static const double sa2_ = 1.37657754143519042600e+02;
static const double sa3_ = 4.34565877475229228821e+02;
static const double sa4_ = 6.45387271733267880336e+02;
static const double sa5_ = 4.29008140027567833386e+02;
static const double sa6_ = 1.08635005541779435134e+02;
static const double sa7_ = 6.57024977031928170135e+00;
static const double sa8_ = -6.04244152148580987438e-02;
static const double rb0_ = -9.86494292470009928597e-03;
static const double rb1_ = -7.99283237680523006574e-01;
static const double rb2_ = -1.77579549177547519889e+01;
static const double rb3_ = -1.60636384855821916062e+02;
static const double rb4_ = -6.37566443368389627722e+02;
static const double rb5_ = -1.02509513161107724954e+03;
static const double rb6_ = -4.83519191608651397019e+02;
static const double sb1_ = 3.03380607434824582924e+01;
static const double sb2_ = 3.25792512996573918826e+02;
static const double sb3_ = 1.53672958608443695994e+03;
static const double sb4_ = 3.19985821950859553908e+03;
static const double sb5_ = 2.55305040643316442583e+03;
static const double sb6_ = 4.74528541206955367215e+02;
static const double sb7_ = -2.24409524465858183362e+01;

double erf(double x) {
  int hx;
  int ix;
  int i;
  double R;
  double S;
  double P;
  double Q;
  double s;
  double y;
  double z;
  double r;

  GET_HIGH_WORD_(hx, x);
  ix = hx & 0x7fffffff;
  if (ix >= 0x7ff00000) {
    i = ((uint)hx >> 31) << 1;
    return (double)(1 - i) + one_ / x;
  }

  if (ix < 0x3feb0000) {
    if(ix < 0x3e300000) {
      if (ix < 0x00800000) {
        return 0.125 * (8.0 * x + efx_8 * x);
      }
      return x + efx_ * x;
    }
    z = x * x;
    r = pp0_ + z * (pp1_ + z * (pp2_ + z * (pp3_ + z * pp4_)));
    s = one_ + z * (qq1_ + z * (qq2_ + z * (qq3_ + z * (qq4_ + z * qq5_))));
    y = r / s;
    return x + x * y;
  }

  if (ix < 0x3ff40000) {
    s = fabs(x) - one_;
    P = pa0_ + s * (pa1_ + s * (pa2_ + s * (pa3_ + s * (pa4_ + s *
              (pa5_ + s * pa6_)))));
    Q = one_ + s * (qa1_ + s * (qa2_ + s * (qa3_ + s * (qa4_ + s *
              (qa5_ + s * qa6_)))));
    if (hx >= 0) {
      return erx_ + P / Q;
    } else {
      return -erx_ - P / Q;
    }
  }

  if (ix >= 0x40180000) {
    if (hx >= 0) {
      return one_ - tiny_;
    } else {
      return tiny_ - one_;
    }
  }

  x = fabs(x);
  s = one_ / (x * x);
  if (ix < 0x4006DB6E) {
    R = ra0_ + s * (ra1_ + s * (ra2_ + s * (ra3_ + s * (ra4_ + s * (
                ra5_ + s * (ra6_ + s * ra7_))))));
    S = one_ + s * (sa1_ + s * (sa2_ + s * (sa3_ + s * (sa4_ + s * (
                sa5_ + s * (sa6_ + s * (sa7_ + s * sa8_)))))));
  } else {
    R = rb0_ + s * (rb1_ + s * (rb2_ + s * (rb3_ + s * (rb4_ + s * (
                rb5_ + s * rb6_)))));
    S = one_ + s * (sb1_ + s * (sb2_ + s * (sb3_ + s * (sb4_ + s * (
                sb5_ + s * (sb6_ + s * sb7_))))));
  }
  z  = x;
  SET_LOW_WORD_(z, 0);
  r = exp(-z * z - 0.5625) * exp((z - x) * (z + x) + R / S);
  if (hx >= 0) {
    return one_ - r / x;
  } else {
    return r / x - one_;
  }
}

double erfc(double x) {
  int hx;
  int ix;
  double R;
  double S;
  double P;
  double Q;
  double s;
  double y;
  double z;
  double r;

  GET_HIGH_WORD_(hx, x);
  ix = hx & 0x7fffffff;
  if (ix >= 0x7ff00000) {
    return (double)(((uint)hx >> 31) << 1) + one_ / x;
  }

  if (ix < 0x3feb0000) {
    if (ix < 0x3c700000) {
      return one_ - x;
    }
    z = x * x;
    r = pp0_ + z * (pp1_ + z * (pp2_ + z * (pp3_ + z * pp4_)));
    s = one_ + z * (qq1_ + z * (qq2_ + z * (qq3_ + z * (qq4_ + z * qq5_))));
    y = r / s;
    if (hx < 0x3fd00000) {
      return one_ - (x + x * y);
    } else {
      r = x * y;
      r += x - half_;
      return half_ - r ;
    }
  }
  if (ix < 0x3ff40000) {
    s = fabs(x) - one_;
    P = pa0_ + s * (pa1_ + s * (pa2_ + s * (pa3_ + s * (pa4_ + s *
              (pa5_ + s * pa6_)))));
    Q = one_ + s * (qa1_ + s * (qa2_ + s * (qa3_ + s * (qa4_ + s *
              (qa5_ + s * qa6_)))));
    if (hx >= 0) {
      z = one_ - erx_;
      return z - P / Q;
    } else {
      z = erx_ + P / Q;
      return one_ + z;
    }
  }
  if (ix < 0x403c0000) {
    x = fabs(x);
    s = one_ / (x * x);
    if (ix < 0x4006DB6D) {
      R = ra0_ + s * (ra1_ + s * (ra2_ + s * (ra3_ + s * (ra4_ + s * (
                  ra5_ + s * (ra6_ + s * ra7_))))));
      S = one_ + s * (sa1_ + s * (sa2_ + s * (sa3_ + s * (sa4_ + s * (
                  sa5_ + s * (sa6_ + s * (sa7_ + s * sa8_)))))));
    } else {
      if (hx < 0 && ix >= 0x40180000) {
        return two_ - tiny_;
      }
      R = rb0_ + s * (rb1_ + s * (rb2_ + s * (rb3_ + s * (rb4_ + s * (
                  rb5_ + s * rb6_)))));
      S = one_ + s * (sb1_ + s * (sb2_ + s * (sb3_ + s * (sb4_ + s * (
                  sb5_ + s * (sb6_ + s * sb7_))))));
    }
    z = x;
    SET_LOW_WORD_(z, 0);
    r = exp(-z * z - 0.5625) * exp((z - x) * (z + x) + R / S);
    if (hx > 0) {
      return r / x;
    } else {
      return two_ -r / x;
    }
  } else {
    if (hx > 0) {
      return tiny_ * tiny_;
    } else {
      return two_ - tiny_;
    }
  }
}
