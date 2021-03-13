#define MAGIC_C
#include "magic.h"
#include <math.h>

typedef struct {
  byte a;
  byte v;
  byte w;
  double k;
  double sharpening;
} Info_;

static KernelPossible sharpen_possible_(ulong out, void *x);
static double sharpen_weight_(ulong out, long in, void *x);

static KernelPossible blur_possible_(ulong out, void *x);
static double blur_weight_(ulong out, long in, void *x);

static KernelPossible resizer_possible_(ulong out, void *x, double support);
static double resizer_weight_(
    ulong out,
    long in,
    void *x,
    double (*f)(double x));

static byte magic_kernel_resizer_(
    Kernel ***pthis,
    ulong n_in,
    ulong n_out,
    bool periodic,
    double k,
    byte a);
static void magic_kernel_(
    Kernel **pthis,
    ulong n_in,
    ulong n_out,
    bool periodic,
    double k,
    byte a);
static KernelPossible magic_kernel_possible_(
    ulong out,
    void *x);
static double magic_kernel_weight_(
    ulong out,
    long in,
    void *x);

static byte magic_kernel_sharp_resizer_(
    Kernel ***pthis,
    ulong n_in,
    ulong n_out,
    bool periodic,
    double k,
    byte a,
    byte v);
static byte sharps_(Kernel ***pthis, ulong n, bool periodic, byte a, byte v);
static KernelPossible sharp_recurse_possible_(ulong out, void *x);
static double sharp_recurse_weight_(ulong out, long in, void *x);
static KernelPossible sharp_possible_(ulong out, void *x);
static double sharp_weight_(ulong out, long in, void *x);

static byte lanczos_resizer_(
    Kernel ***pthis,
    ulong n_in,
    ulong n_out,
    bool periodic,
    double k,
    byte a);
static KernelPossible lanczos_possible_(
    ulong out,
    void *x);
static double lanczos_weight_(
    ulong out,
    long in,
    void *x);

static void check_(ulong n_in, ulong n_out, double k);

double magic_kernel(byte a, double x) {
  double x2;
  double x3;
  double x4;
  double x5;

  switch (a) {
    case 1:
      if (x < -0.5 || x >= +0.5) {
        return 0;
      }

      return 1;

    case 2:
      if (x <= -1 || x >= +1) {
        return 0;
      }

      return 1 - fabs(x);

    case 3:
      if (x <= -1.5 || x >= +1.5) {
        return 0;
      }

      x2 = x * x;

      if (x <= -0.5) {
        return x2 / 2 + 3 * x / 2 + 9.0 / 8;
      }

      if (x <= +0.5) {
        return -x2 + 3.0 / 4;
      }

      return x2 / 2 - 3 * x / 2 + 9.0 / 8;

    case 4:
      if (x <= -2 || x >= +2) {
        return 0;
      }

      x2 = x * x;
      x3 = x2 * x;

      if (x <= 0) {
        if (x <= -1) {
          return x3 / 6 + x2 + 2 * x + 4.0 / 3;
        }

        return -x3 / 2 - x2 + 2.0 / 3;
      }

      if (x <= +1) {
        return x3 / 2 - x2 + 2.0 / 3;
      }

      return -x3 / 6 + x2 - 2 * x + 4.0 / 3;

    case 5:
      if (x <= -2.5 || x >= +2.5) {
        return 0;
      }

      x2 = x * x;
      x3 = x2 * x;
      x4 = x3 * x;

      if (x <= -0.5) {
        if (x <= -1.5) {
          return x4 / 24 + 5 * x3 / 12 + 25 * x2 / 16 + 125 * x / 48
            + 625.0 / 384;
        }

        return -x4 / 6 - 5 * x3 / 6 - 5 * x2 / 4 - 5 * x / 24 + 55.0 / 96;
      }

      if (x <= +0.5) {
        return x4 / 4 - 5 * x2 / 8 + 115.0 / 192;
      }

      if (x <= +1.5) {
        return -x4 / 6 + 5 * x3 / 6 - 5 * x2 / 4 + 5 * x / 24 + 55.0 / 96;
      }

      return x4 / 24 - 5 * x3 / 12 + 25 * x2 / 16 - 125 * x / 48 + 625.0 / 384;

    case 6:
      if (x <= -3 || x >= +3) {
        return 0;
      }

      x2 = x * x;
      x3 = x2 * x;
      x4 = x3 * x;
      x5 = x4 * x;

      if (x <= 0) {
        if (x <= -2) {
          return x5 / 120 + x4 / 8 + 3 * x3 / 4 + 9 * x2 / 4 + 27 * x / 8
            + 81.0 / 40;
        }

        if (x <= -1) {
          return -x5 / 24 - 3 * x4 / 8 - 5 * x3 / 4 - 7 * x2 / 4 - 5 * x / 8
            + 17.0 / 40;
        }

        return x5 / 12 + x4 / 4 - x2 / 2 + 11.0 / 20;
      }

      if (x <= +1) {
        return -x5 / 12 + x4 / 4 - x2 / 2 + 11.0 / 20;
      }

      if (x <= +2) {
        return x5 / 24 - 3 * x4 / 8 + 5 * x3 / 4 - 7 * x2 / 4 + 5 * x / 8
          + 17.0 / 40;
      }

      return -x5 / 120 + x4 / 8 - 3 * x3 / 4 + 9 * x2 / 4 - 27 * x / 8
        + 81.0 / 40;

    default:
      throw("a (%u) must be in [1, 6]", a);
  }

  return 0;
}

double magic_kernel_1(double x) {
  return magic_kernel(1, x);
}

double magic_kernel_2(double x) {
  return magic_kernel(2, x);
}

double magic_kernel_3(double x) {
  return magic_kernel(3, x);
}

double magic_kernel_4(double x) {
  return magic_kernel(4, x);
}

double magic_kernel_5(double x) {
  return magic_kernel(5, x);
}

double magic_kernel_6(double x) {
  return magic_kernel(6, x);
}

double nearest_neighbor_kernel(double x) {
  return magic_kernel_1(x);
}

double linear_kernel(double x) {
  return magic_kernel_2(x);
}

double lanczos_kernel(byte a, double x) {
  if (a == 0) {
    throw("a cannot be zero");
  }

  if (x <= -(long)a || x >= +(long)a) {
    return 0;
  }

  return sinc(x) * sinc(x / a);
}

double lanczos_2_kernel(double x) {
  return lanczos_kernel(2, x);
}

double lanczos_3_kernel(double x) {
  return lanczos_kernel(3, x);
}

double lanczos_4_kernel(double x) {
  return lanczos_kernel(4, x);
}

byte kernels_new_resize(
    Kernel ***pthis,
    ulong n_in,
    ulong n_out,
    bool periodic,
    double k,
    char *method,
    byte a,
    byte v) {
  check_(n_in, n_out, k);

  if (string_equals(method, MAGIC_RESIZER_MAGIC_KERNEL_SHARP)) {
    return magic_kernel_sharp_resizer_(
        pthis,
        n_in,
        n_out,
        periodic,
        k,
        a,
        v);
  }

  if (string_equals(method, MAGIC_RESIZER_MAGIC_KERNEL)) {
    return magic_kernel_resizer_(
        pthis,
        n_in,
        n_out,
        periodic,
        k,
        a);
  }

  if (string_equals(method, MAGIC_RESIZER_LINEAR)) {
    return magic_kernel_sharp_resizer_(
        pthis,
        n_in,
        n_out,
        periodic,
        k,
        2,
        0);
  }

  if (string_equals(method, MAGIC_RESIZER_NEAREST_NEIGHBOR)) {
    return magic_kernel_sharp_resizer_(
        pthis,
        n_in,
        n_out,
        periodic,
        k,
        1,
        0);
  }

  if (string_equals(method, MAGIC_RESIZER_LANCZOS)) {
    return lanczos_resizer_(
        pthis,
        n_in,
        n_out,
        periodic,
        k,
        a);
  }

  throw("Resizing method '%s' not recognized", method);
  return 0;
}

Kernel *kernel_new_blur(Kernel **pthis, ulong n, bool periodic, double blur) {
  Info_ info;

  if (blur <= 0) {
    throw("Blur (%g) must be positive", blur);
  }

  info.k = blur >= 1 ? blur : (2 + blur) / 3;

  return kernel_new(
      pthis,
      n,
      n,
      periodic,
      blur_possible_,
      blur_weight_,
      &info);
}

Kernel *kernel_new_sharpen(
    Kernel **pthis,
    ulong n,
    bool periodic,
    double sharpening) {
  Info_ info;

  if (sharpening <= 0) {
    throw("Sharpening (%g) must be positive", sharpening);
  }

  info.sharpening = sharpening;

  return kernel_new(
      pthis,
      n,
      n,
      periodic,
      sharpen_possible_,
      sharpen_weight_,
      &info);
}

static KernelPossible sharpen_possible_(ulong out, void *x) {
  KernelPossible possible;

  possible.first = (long)out - 1;
  possible.last = (long)out + 1;

  return possible;
}

static double sharpen_weight_(ulong out, long in, void *x) {
  ulong rel;
  double s;
  Info_ *info;

  rel = (ulong)labs((long)out - in);

  info = x;
  s = info->sharpening;

  switch (rel) {
    case 0:
      return 1 + 0.5 * s;
    case 1:
      return -0.25 * s;
    default:
      throw("impossible");
      return 0;
  }
}

static KernelPossible blur_possible_(ulong out, void *x) {
  double width;
  KernelPossible possible;
  Info_ *info;

  info = x;
  width = 1.5 * info->k;

  possible.first = (long)floor(out - width);
  possible.last = (long)ceil(out + width);

  return possible;
}

static double blur_weight_(ulong out, long in, void *x) {
  Info_ *info;

  info = x;

  return magic_kernel_3(((long)out - in) / info->k);
}

static KernelPossible resizer_possible_(ulong out, void *x, double support) {
  double center;
  double k;
  double d;
  KernelPossible possible;
  Info_ *info;

  info = x;
  k = info->k;
  center = kernel_resize_in_from_out(out, k);

  d = 0.5 * support / minimum(k, 1);
  possible.first = (long)floor(center - d);
  possible.last = (long)ceil(center + d);

  return possible;
}

static double resizer_weight_(
    ulong out,
    long in,
    void *x,
    double (*f)(double x)) {
  double center;
  double k;
  Info_ *info;

  info = x;
  k = info->k;

  if (k >= 1) {
    center = kernel_resize_in_from_out(out, k);
    return f(in - center);
  }

  center = kernel_resize_out_from_in(in, k);
  return f((long)out - center);
}

static byte magic_kernel_resizer_(
    Kernel ***pthis,
    ulong n_in,
    ulong n_out,
    bool periodic,
    double k,
    byte a) {
  Kernel **this;

  this = new(pthis, 1);

  magic_kernel_(
      &this[0],
      n_in,
      n_out,
      periodic,
      k,
      a);

  return 1;
}

static void magic_kernel_(
    Kernel **pthis,
    ulong n_in,
    ulong n_out,
    bool periodic,
    double k,
    byte a) {
  Info_ info;

  if (a == 0 || a > 6) {
    throw("a (%u) must be in [1, 6]", a);
  }

  info.k = k;
  info.a = a;

  kernel_new(
      pthis,
      n_in,
      n_out,
      periodic,
      magic_kernel_possible_,
      magic_kernel_weight_,
      &info);
}

static KernelPossible magic_kernel_possible_(ulong out, void *x) {
  Info_ *info;

  info = x;

  return resizer_possible_(out, x, info->a);
}

static double magic_kernel_weight_(ulong out, long in, void *x) {
  byte a;
  Info_ *info;
  double (*fs[])(double x) = {
    null,
    magic_kernel_1,
    magic_kernel_2,
    magic_kernel_3,
    magic_kernel_4,
    magic_kernel_5,
    magic_kernel_6
  };

  info = x;
  a = info->a;

  return resizer_weight_(out, in, x, fs[a]);
}

static byte magic_kernel_sharp_resizer_(
    Kernel ***pthis,
    ulong n_in,
    ulong n_out,
    bool periodic,
    double k,
    byte a,
    byte v) {
  bool upsize;
  byte num_sharps;
  byte s;
  Kernel *mk = null;
  Kernel **sharps = null;
  Kernel **this;

  magic_kernel_(&mk, n_in, n_out, periodic, k, a);

  if (a == 1 || a == 2) {
    this = new(pthis, 1);
    donate(&mk, &this[0]);
    return 1;
  }

  upsize = k >= 1;

  num_sharps = sharps_(&sharps, upsize ? n_in : n_out, periodic, a, v);

  this = new(pthis, 1 + num_sharps);

  if (upsize) {
    for (s = 0; s < num_sharps; s++) {
      donate(&sharps[s], &this[s]);
    }

    donate(&mk, &this[num_sharps]);
  } else {
    donate(&mk, &this[0]);

    for (s = 0; s < num_sharps; s++) {
      donate(&sharps[s], &this[1 + s]);
    }
  }

  delete(&sharps, num_sharps, kernel_delete);
  return 1 + num_sharps;
}

static byte sharps_(Kernel ***pthis, ulong n, bool periodic, byte a, byte v) {
  byte num_sharps;
  Info_ info;
  Kernel **this;

  num_sharps = a == 3 || a == 4 ? (v == 1 ? 1 : v == 3 ? 2 : 3) : 1;
  this = new(pthis, num_sharps);

  info.a = a;
  info.v = v;

  switch (a) {
    case 3:
    case 4:
      switch (v) {
        case 7:
          info.w = 3;
          kernel_new(
              &this[2],
              n,
              n,
              periodic,
              sharp_recurse_possible_,
              sharp_recurse_weight_,
              &info);
          /* Fall-through.
           */
        case 3:
          info.w = 2;
          kernel_new(
              &this[1],
              n,
              n,
              periodic,
              sharp_recurse_possible_,
              sharp_recurse_weight_,
              &info);
          /* Fall-through.
           */
        case 1:
          info.w = 1;
          kernel_new(
              &this[0],
              n,
              n,
              periodic,
              sharp_recurse_possible_,
              sharp_recurse_weight_,
              &info);
          break;

        default:
          throw("v (%lu) invalid for a = %u: must be 1, 3, or 7", v, a);
      }
      break;

    case 5:
    case 6:
      kernel_new(
          &this[0],
          n,
          n,
          periodic,
          sharp_possible_,
          sharp_weight_,
          &info);
      break;

    default:
      throw("'a' (%u) invalid: must be in [3, 6]", a);
      return 0;
  }

  return num_sharps;
}

static KernelPossible sharp_recurse_possible_(ulong out, void *x) {
  byte d;
  byte w;
  KernelPossible possible;
  Info_ *info;

  info = x;
  w = info->w;

  switch (w) {
    case 1:
      d = 1;
      break;
    case 2:
      d = 2;
      break;
    case 3:
      d = 4;
      break;
    default:
      throw("impossible: w (%u) invalid: must be in [1, 3]", w);
      d = 0;
  }

  possible.first = (long)out - (long)d;
  possible.last = (long)out + (long)d;

  return possible;
}

static double sharp_recurse_weight_(ulong out, long in, void *x) {
  bool t;
  byte a;
  byte w;
  ulong rel;
  Info_ *info;

  rel = (ulong)labs((long)out - in);

  info = x;
  a = info->a;
  w = info->w;
  t = a == 3;

  if (a != 3 && a != 4) {
    throw("impossible: a (%u) must be 3 or 4", a);
  }

  switch (w) {
    case 1:
      switch (rel) {
        case 0:
          return t ? 3.0 / 2 : 2;
        case 1:
          return t ? -1.0 / 4 : -1.0 / 2;
        default:
          throw("impossible rel (%lu)", rel);
          return 0;
      }

    case 2:
      switch (rel) {
        case 0:
          return t ? 17.0 / 18 : 7.0 / 8;
        case 2:
          return t ? 1.0 / 36 : 1.0 / 16;
        default:
          return 0;
      }

    case 3:
      switch (rel) {
        case 0:
          return t ? 577.0 / 578 : 97.0 / 98;
        case 4:
          return t ? 1.0 / 1156 : 1.0 / 196;
        default:
          return 0;
      }

    default:
      throw("impossible: w (%u) invalid: must be in [1, 3]", w);
      return 0;
  }
}

static KernelPossible sharp_possible_(ulong out, void *x) {
  byte v;
  KernelPossible possible;
  Info_ *info;

  info = x;
  v = info->v;

  possible.first = (long)out - (long)v;
  possible.last = (long)out + (long)v;

  return possible;
}

static double sharp_weight_(ulong out, long in, void *x) {
  bool f;
  byte a;
  byte v;
  ulong rel;
  Info_ *info;

  rel = (ulong)labs((long)out - in);

  info = x;
  a = info->a;
  v = info->v;
  f = a == 5;

  if (a != 5 && a != 6) {
    throw("impossible: a (%u) must be 5 or 6", a);
  }

  switch (v) {
    case 1:
      switch (rel) {
        case 0:
          return f ? 231.0 / 79 : 67.0 / 15;
        case 1:
          return f ? -76.0 / 79 : -26.0 / 15;
        default:
          throw("impossible rel (%lu) for v = %u", rel, v);
          return 0;
      }

    case 2:
      switch (rel) {
        case 0:
          return f ? 23677.0 / 11818 : 1873.0 / 792;
        case 1:
          return f ? -229.0 / 311 : -845.0 / 792;
        case 2:
          return f ? 5545.0 / 23636 : 203.0 / 528;
        default:
          throw("impossible rel (%lu) for v = %u", rel, v);
          return 0;
      }

    case 3:
      switch (rel) {
        case 0:
          return f ? 4784243.0 / 2085782 : 20323.0 / 6600;
        case 1:
          return f ? -46895.0 / 54889 : -3133.0 / 2200;
        case 2:
          return f ? 1269575.0 / 4171564 : 13173.0 / 22000;
        case 3:
          return f ? -5316.0 / 54889 : -884.0 / 4125;
        default:
          throw("impossible rel (%lu) for v = %u", rel, v);
          return 0;
      }

    case 4:
      switch (rel) {
        case 0:
          return f ? 1929838907.0 / 883546745 : 168703.0 / 61425;
        case 1:
          return f ? -720059492.0 / 883546745 : -78374.0 / 61425;
        case 2:
          return f ? 259980106.0 / 883546745 : 2197246.0 / 3992625;
        case 3:
          return f ? -92502336.0 / 883546745 : -70592.0 / 307125;
        case 4:
          return f ? 29435641.0 / 883546745 : 13129.0 / 159705;
        default:
          throw("impossible rel (%lu) for v = %u", rel, v);
          return 0;
      }

    case 5:
      switch (rel) {
        case 0:
          return f ? 389128858733.0 / 175036036855 : 223979.0 / 77649;
        case 1:
          return f ? -145224073928.0 / 175036036855 : -104140.0 / 77649;
        case 2:
          return f ? 52525133894.0 / 175036036855 : 53330.0 / 91767;
        case 3:
          return f ? -18942362284.0 / 175036036855 : -19334.0 / 77649;
        case 4:
          return f ? 6739497759.0 / 175036036855 : 104905.0 / 1009437;
        case 5:
          return f ? -428921276.0 / 35007207371 : -74.0 / 1991;
        default:
          throw("impossible rel (%lu) for v = %u", rel, v);
          return 0;
      }

    case 6:
      if (f) {
        throw("impossible: cannot have v = 6 for a = 5");
      }

      switch (rel) {
        case 0:
          return 7963722673.0 / 2819858480;
        case 1:
          return -3703337469.0 / 2819858480;
        case 2:
          return 1284865959.0 / 2255886784;
        case 3:
          return -276529097.0 / 1127943392;
        case 4:
          return 592002417.0 / 5639716960;
        case 5:
          return -247073229.0 / 5639716960;
        case 6:
          return 176724289.0 / 11279433920;
        default:
          throw("impossible rel (%lu) for v = %u", rel, v);
          return 0;
      }

    case 7:
      if (f) {
        throw("impossible: cannot have v = 7 for a = 5");
      }

      switch (rel) {
        case 0:
          return 8415120629.0 / 2952708336;
        case 1:
          return -3913360945.0 / 2952708336;
        case 2:
          return 20369223277.0 / 35432500032;
        case 3:
          return -4387620835.0 / 17716250016;
        case 4:
          return 1887357215.0 / 17716250016;
        case 5:
          return -808049411.0 / 17716250016;
        case 6:
          return 674477135.0 / 35432500032;
        case 7:
          return -3769012.0 / 553632813;
        default:
          throw("impossible rel (%lu) for v = %u", rel, v);
          return 0;
      }

    default:
      throw("v (%u) invalid: must be in [1, 7]", v);
      return 0;
  }
}

static byte lanczos_resizer_(
    Kernel ***pthis,
    ulong n_in,
    ulong n_out,
    bool periodic,
    double k,
    byte a) {
  Info_ info;
  Kernel **this;

  if (a < 2 || a > 3) {
    throw("a (%u) must be 2 or 3", a);
  }

  this = new(pthis, 1);

  info.k = k;
  info.a = a;

  kernel_new(
      &this[0],
      n_in,
      n_out,
      periodic,
      lanczos_possible_,
      lanczos_weight_,
      &info);

  return 1;
}

static KernelPossible lanczos_possible_(ulong out, void *x) {
  Info_ *info;

  info = x;

  return resizer_possible_(out, x, 2 * info->a);
}

static double lanczos_weight_(ulong out, long in, void *x) {
  byte a;
  Info_ *info;
  double (*fs[])(double x) = {
    null,
    null,
    lanczos_2_kernel,
    lanczos_3_kernel
  };

  info = x;
  a = info->a;

  return resizer_weight_(out, in, x, fs[a]);
}

static void check_(ulong n_in, ulong n_out, double k) {
  bool ikr;
  ulong ki;
  ulong kr;
  ulong min;
  ulong max;

  if (n_in == 0) {
    throw("n_in cannot be zero");
  }

  if (n_out == 0) {
    throw("n_out cannot be zero");
  }

  if (k <= 0) {
    throw("k (%g) must be positive", k);
  }

  if (k == 1) {
    throw("k cannot be 1");
  }

  if (k >= 1) {
    if (floor(k) == k && n_out % (ki = (ulong)round(k)) == 0) {
      min = max = n_out / ki;
    } else {
      min = (ulong)floor(n_out / k);
      max = (ulong)ceil(n_out / k);
    }

    if (n_in < min) {
      throw(
          "n_in (%lu) too small for n_out (%lu) and k (%g): must be %s %lu",
          n_in,
          n_out,
          k,
          min == max ? "exactly" : "at least",
          min);
    }
    if (n_in > max) {
      throw(
          "n_in (%lu) too large for n_out (%lu) and k (%g): must be %s %lu",
          n_in,
          n_out,
          k,
          min == max ? "exactly" : "no greater than",
          max);
    }
  } else {
    if ((ikr = k == 1.0 / (kr = (ulong)floor(1 / k))) && n_in % kr == 0) {
      min = max = n_in / kr;
    } else {
      min = maximum((ulong)floor(n_in * k), 1);
      max = (ulong)ceil(n_in * k);
    }

    if (n_out < min) {
      if (ikr) {
        throw(
            "n_out (%lu) too small for n_in (%lu) and k (1/%lu): "
            "must be %s %lu",
            n_out,
            n_in,
            kr,
            min == max ? "exactly" : "at least",
            min);
      }
      throw(
          "n_out (%lu) too small for n_in (%lu) and k (%g): must be %s %lu",
          n_out,
          n_in,
          k,
          min == max ? "exactly" : "at least",
          min);
    }
    if (n_out > max) {
      if (ikr) {
        throw(
            "n_out (%lu) too large for n_in (%lu) and k (1/%lu): "
            "must be %s %lu",
            n_out,
            n_in,
            kr,
            min == max ? "exactly" : "no greater than",
            max);
      }
      throw(
          "n_out (%lu) too large for n_in (%lu) and k (%g): must be %s %lu",
          n_out,
          n_in,
          k,
          min == max ? "exactly" : "no greater than",
          max);
    }
  }
}
