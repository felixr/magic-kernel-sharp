#include "magic_test.h"
#include <float.h>
#include <math.h>

#define PI_ 3.14159265358979323846264338

#define EPS_0_ (10 * DBL_EPSILON)
#define EPS_1_ (10 * EPS_0_)
#define EPS_2_ (10 * EPS_1_)
#define EPS_3_ 1e-3
#define EPS_4_ (DBL_EPSILON / EPS_3_)

static void t_(byte a, double x, double e);
static double d_(byte a, byte n, double x, double eps);
static double eps_(byte n);
static void td_(byte a, byte n, double x);
static void tl_(byte a, double x, double e);
static ulong m_(ulong u, long d, ulong n);
static void check_magic_(Kernel *k, byte a, double kk);
static void check_lanczos_(Kernel *k, byte a, double kk);

START

TEST(Magic, MagicKernel) {
  bool half;
  byte a;
  byte n;
  sbyte xi;
  double x;

  for (a = 1; a <= 6; a++) {
    /* Test specific values.
     */
    switch (a) {
      case 1:
        t_(a, -0.51, 0);
        t_(a, -0.50, 1);
        t_(a, -0.25, 1);
        t_(a,  0.00, 1);
        t_(a, +0.25, 1);
        t_(a, +0.50, 0);
        t_(a, +0.51, 0);
        break;

      case 2:
        t_(a, -1.00, 0);
        t_(a, -0.75, 1.0 / 4);
        t_(a, -0.50, 1.0 / 2);
        t_(a, -0.25, 3.0 / 4);
        t_(a,  0.00, 1);
        t_(a, +0.25, 3.0 / 4);
        t_(a, +0.50, 1.0 / 2);
        t_(a, +0.75, 1.0 / 4);
        t_(a, +1.00, 0);
        break;

      case 3:
        t_(a, -1.50, 0);
        t_(a, -1.25, 1.0 / 32);
        t_(a, -1.00, 1.0 / 8);
        t_(a, -0.75, 9.0 / 32);
        t_(a, -0.50, 1.0 / 2);
        t_(a, -0.25, 11.0 / 16);
        t_(a,  0.00, 3.0 / 4);
        t_(a, +0.25, 11.0 / 16);
        t_(a, +0.50, 1.0 / 2);
        t_(a, +0.75, 9.0 / 32);
        t_(a, +1.00, 1.0 / 8);
        t_(a, +1.25, 1.0 / 32);
        t_(a, +1.50, 0);
        break;

      case 4:
        t_(a, -2.00, 0);
        t_(a, -1.75, 1.0 / 384);
        t_(a, -1.50, 1.0 / 48);
        t_(a, -1.25, 9.0 / 128);
        t_(a, -1.00, 1.0 / 6);
        t_(a, -0.75, 121.0 / 384);
        t_(a, -0.50, 23.0 / 48);
        t_(a, -0.25, 235.0 / 384);
        t_(a,  0.00, 2.0 / 3);
        t_(a, +0.25, 235.0 / 384);
        t_(a, +0.50, 23.0 / 48);
        t_(a, +0.75, 121.0 / 384);
        t_(a, +1.00, 1.0 / 6);
        t_(a, +1.25, 9.0 / 128);
        t_(a, +1.50, 1.0 / 48);
        t_(a, +1.75, 1.0 / 384);
        t_(a, +2.00, 0);
        break;

      case 5:
        t_(a, -2.50, 0);
        t_(a, -2.25, 1.0 / 6144);
        t_(a, -2.00, 1.0 / 384);
        t_(a, -1.75, 27.0 / 2048);
        t_(a, -1.50, 1.0 / 24);
        t_(a, -1.25, 155.0 / 1536);
        t_(a, -1.00, 19.0 / 96);
        t_(a, -0.75, 499.0 / 1536);
        t_(a, -0.50, 11.0 / 24);
        t_(a, -0.25, 1723.0 / 3072);
        t_(a,  0.00, 115.0 / 192);
        t_(a, +0.25, 1723.0 / 3072);
        t_(a, +0.50, 11.0 / 24);
        t_(a, +0.75, 499.0 / 1536);
        t_(a, +1.00, 19.0 / 96);
        t_(a, +1.25, 155.0 / 1536);
        t_(a, +1.50, 1.0 / 24);
        t_(a, +1.75, 27.0 / 2048);
        t_(a, +2.00, 1.0 / 384);
        t_(a, +2.25, 1.0 / 6144);
        t_(a, +2.50, 0);
        break;

      case 6:
        t_(a, -3.00, 0);
        t_(a, -2.75, 1.0 / 122880);
        t_(a, -2.50, 1.0 / 3840);
        t_(a, -2.25, 81.0 / 40960);
        t_(a, -2.00, 1.0 / 120);
        t_(a, -1.75, 3119.0 / 122880);
        t_(a, -1.50, 79.0 / 1280);
        t_(a, -1.25, 15349.0 / 122880);
        t_(a, -1.00, 13.0 / 60);
        t_(a, -0.75, 6719.0 / 20480);
        t_(a, -0.50, 841.0 / 1920);
        t_(a, -0.25, 31927.0 / 61440);
        t_(a,  0.00, 11.0 / 20);
        t_(a, +0.25, 31927.0 / 61440);
        t_(a, +0.50, 841.0 / 1920);
        t_(a, +0.75, 6719.0 / 20480);
        t_(a, +1.00, 13.0 / 60);
        t_(a, +1.25, 15349.0 / 122880);
        t_(a, +1.50, 79.0 / 1280);
        t_(a, +1.75, 3119.0 / 122880);
        t_(a, +2.00, 1.0 / 120);
        t_(a, +2.25, 81.0 / 40960);
        t_(a, +2.50, 1.0 / 3840);
        t_(a, +2.75, 1.0 / 122880);
        t_(a, +3.00, 0);
        break;
    }

    /* Test continuity of n-th derivatives, where n = [0, a - 2],  at each of
     * the joins of the rectangular windows. I.e. m_1 is not continuous at all;
     * m_2 is continuous (n = 0) at all joins; m_3 is continuous (n = 0) and
     * has continuous first derivative (n = 1); etc. The tests here are quite
     * approximate for higher derivatives, because the precision available is
     * only double precision, and the n-th numerical derivative has a
     * denominator of eps^n, but there are enough tests overall to ensure that
     * the formulas have been correctly transcribed. The program
     * draw_magic_kernels also allows a visual verification of the functions,
     * directly from the magic library.
     */
    half = a % 2 == 0;

    for (xi = -a / 2; xi <= +a / 2 + (half ? 0 : 1); xi++) {
      x = xi - (half ? 0 : 0.5);
      for (n = 0; n <= a - 2; n++) {
        td_(a, n, x);
      }
    }
  }
}

TEST(Magic, LanczosKernel) {
  byte a;
  sbyte xi;

  for (a = 2; a <= 4; a++) {
    switch (a) {
      case 2:
        tl_(a, 0.5, 4 * sqrt(2) / (PI_ * PI_));
        tl_(a, 1.5, -4 * sqrt(2) / (9 * PI_ * PI_));
        break;
      case 3:
        tl_(a, 0.5, 6 / (PI_ * PI_));
        tl_(a, 1.5, -4 / (3 * PI_ * PI_));
        break;
      case 4:
        tl_(a, 0.5, 8 / (PI_ * PI_) * sqrt(2 - sqrt(2)));
        tl_(a, 1.5, -8 / (9 * PI_ * PI_) * sqrt(2 + sqrt(2)));
        break;
    }

    /* Test the zeros at integral x.
     */
    for (xi = -a; xi <= +a; xi++) {
      EXPECT_NEAR(xi == 0 ? 1 : 0, lanczos_kernel(a, xi), 1e-16);
    }
  }
}

TEST(Magic, MagicKernelSharpResizer) {
  byte a;
  byte k;
  byte num_vs;
  byte v;
  byte vi;
  byte *vs = null;
  sbyte d;
  ulong n_in;
  ulong n_out;
  ulong num_kernels;
  ulong out;
  double kk;
  double t;
  Kernel *kernel;
  Kernel **kernels = null;

  for (a = 1; a <= 6; a++) {
    switch (a) {
      case 1:
      case 2:
        new(&vs, num_vs = 1);
        vs[0] = 0;
        break;
      case 3:
      case 4:
        new(&vs, num_vs = 3);
        vs[0] = 1;
        vs[1] = 3;
        vs[2] = 7;
        break;
      case 5:
        num_vs = 5;
        break;
      case 6:
        num_vs = 7;
        break;
    }

    if (a == 5 || a == 6) {
      new(&vs, num_vs);

      for (vi = 0; vi < num_vs; vi++) {
        vs[vi] = vi + 1;
      }
    }

    for (vi = 0; vi < num_vs; vi++) {
      v = vs[vi];

      /* Test k = 2, i.e. doubling in size. (Need to call it 'kk' because 'k'
       * is the index for kernels.) Periodic boundary conditions are easiest to
       * verify; extension boundary conditions are tested separately in
       * KernelTest.
      */
      n_in = maximum(maximum(a, v + 2), 2 * v + 1) + 1;
      n_out = 2 * n_in;
      kk = 2;

      num_kernels = kernels_new_resize(
          &kernels,
          n_in,
          n_out,
          true,
          kk,
          MAGIC_RESIZER_MAGIC_KERNEL_SHARP,
          a,
          v);

      switch (a) {
        case 1:
        case 2:
          EXPECT_ULEQ(1, num_kernels);
          break;

        case 3:
        case 4:
          switch (v) {
            case 1:
              EXPECT_ULEQ(2, num_kernels);
              break;
            case 3:
              EXPECT_ULEQ(3, num_kernels);
              break;
            case 7:
              EXPECT_ULEQ(4, num_kernels);
              break;
          }
          break;

        case 5:
        case 6:
          EXPECT_ULEQ(2, num_kernels);
          break;
      }

      for (k = 0; k < num_kernels; k++) {
        kernel = kernels[k];

        /* For upsizing, the last kernel is the Magic Kernel upsizer.
         */
        if (k == num_kernels - 1) {
          check_magic_(kernel, a, kk);
        } else {
          /* One of the Sharp kernels.
           */
          for (out = 0; out < n_in; out++) {
            switch (a) {
              case 3:
                d = 1 << k;
                t = 1e-15;
                EXPECT_NEAR(
                    k == 0 ? -0.25
                    : k == 1 ? 0.0277777777777778
                    : 0.000865051903114187,
                    kernel_get(kernel, out, m_(out, -d, n_in)),
                    t);
                EXPECT_NEAR(
                    k == 0 ? +1.50000000000000
                    : k == 1 ? +0.944444444444444
                    : +0.998269896193772,
                    kernel_get(kernel, out, out),
                    t);
                EXPECT_NEAR(
                    k == 0 ? -0.25
                    : k == 1 ? 0.0277777777777778
                    : 0.000865051903114187,
                    kernel_get(kernel, out, m_(out, +d, n_in)),
                    t);
                break;

              case 4:
                d = 1 << k;
                EXPECT_NEAR(
                    k == 0 ? -0.5
                    : k == 1 ? 0.0625
                    : 0.00510204081632653,
                    kernel_get(kernel, out, m_(out, -d, n_in)),
                    t);
                EXPECT_NEAR(
                    k == 0 ? +2.00000000000000
                    : k == 1 ? +0.875
                    : +0.989795918367347,
                    kernel_get(kernel, out, out),
                    t);
                EXPECT_NEAR(
                    k == 0 ? -0.5
                    : k == 1 ? 0.0625
                    : 0.00510204081632653,
                    kernel_get(kernel, out, m_(out, +d, n_in)),
                    t);
                break;

              case 5:

                d = -v;
                t = 1e-15;
                switch (v) {
                  case 1:
                    EXPECT_NEAR(
                        -0.9620253164556962,
                        kernel_get(kernel, out, m_(out, d++, n_in)),
                        t);
                    EXPECT_NEAR(
                        +2.924050632911392,
                        kernel_get(kernel, out, m_(out, d++, n_in)),
                        t);
                    EXPECT_NEAR(
                        -0.9620253164556962,
                        kernel_get(kernel, out, m_(out, d++, n_in)),
                        t);
                    break;

                  case 2:
                    EXPECT_NEAR(
                        +0.2345997630732780,
                        kernel_get(kernel, out, m_(out, d++, n_in)),
                        t);
                    EXPECT_NEAR(
                        -0.7363344051446945,
                        kernel_get(kernel, out, m_(out, d++, n_in)),
                        t);
                    EXPECT_NEAR(
                        +2.003469284142833,
                        kernel_get(kernel, out, m_(out, d++, n_in)),
                        t);
                    EXPECT_NEAR(
                        -0.7363344051446945,
                        kernel_get(kernel, out, m_(out, d++, n_in)),
                        t);
                    EXPECT_NEAR(
                        +0.2345997630732780,
                        kernel_get(kernel, out, m_(out, d++, n_in)),
                        t);
                    break;

                  case 3:
                    EXPECT_NEAR(
                        -0.09685000637650531,
                        kernel_get(kernel, out, m_(out, d++, n_in)),
                        t);
                    EXPECT_NEAR(
                        +0.3043402905960450,
                        kernel_get(kernel, out, m_(out, d++, n_in)),
                        t);
                    EXPECT_NEAR(
                        -0.8543606187032010,
                        kernel_get(kernel, out, m_(out, d++, n_in)),
                        t);
                    EXPECT_NEAR(
                        +2.293740668967323,
                        kernel_get(kernel, out, m_(out, d++, n_in)),
                        t);
                    EXPECT_NEAR(
                        -0.8543606187032010,
                        kernel_get(kernel, out, m_(out, d++, n_in)),
                        t);
                    EXPECT_NEAR(
                        +0.3043402905960450,
                        kernel_get(kernel, out, m_(out, d++, n_in)),
                        t);
                    EXPECT_NEAR(
                        -0.09685000637650531,
                        kernel_get(kernel, out, m_(out, d++, n_in)),
                        t);
                    break;

                  case 4:
                    EXPECT_NEAR(
                        +0.03331531825178078,
                        kernel_get(kernel, out, m_(out, d++, n_in)),
                        t);
                    EXPECT_NEAR(
                        -0.1046943317073733,
                        kernel_get(kernel, out, m_(out, d++, n_in)),
                        t);
                    EXPECT_NEAR(
                        +0.2942460118507935,
                        kernel_get(kernel, out, m_(out, d++, n_in)),
                        t);
                    EXPECT_NEAR(
                        -0.8149647950997771,
                        kernel_get(kernel, out, m_(out, d++, n_in)),
                        t);
                    EXPECT_NEAR(
                        +2.184195593409152,
                        kernel_get(kernel, out, m_(out, d++, n_in)),
                        t);
                    EXPECT_NEAR(
                        -0.8149647950997771,
                        kernel_get(kernel, out, m_(out, d++, n_in)),
                        t);
                    EXPECT_NEAR(
                        +0.2942460118507935,
                        kernel_get(kernel, out, m_(out, d++, n_in)),
                        t);
                    EXPECT_NEAR(
                        -0.1046943317073733,
                        kernel_get(kernel, out, m_(out, d++, n_in)),
                        t);
                    EXPECT_NEAR(
                        +0.03331531825178078,
                        kernel_get(kernel, out, m_(out, d++, n_in)),
                        t);
                    break;

                  case 5:
                    EXPECT_NEAR(
                        -0.01225237053199847,
                        kernel_get(kernel, out, m_(out, d++, n_in)),
                        t);
                    EXPECT_NEAR(
                        +0.03850348693956665,
                        kernel_get(kernel, out, m_(out, d++, n_in)),
                        t);
                    EXPECT_NEAR(
                        -0.1082197850474178,
                        kernel_get(kernel, out, m_(out, d++, n_in)),
                        t);
                    EXPECT_NEAR(
                        +0.3000818279353061,
                        kernel_get(kernel, out, m_(out, d++, n_in)),
                        t);
                    EXPECT_NEAR(
                        -0.8296809990522337,
                        kernel_get(kernel, out, m_(out, d++, n_in)),
                        t);
                    EXPECT_NEAR(
                        2.223135679513555,
                        kernel_get(kernel, out, m_(out, d++, n_in)),
                        t);
                    EXPECT_NEAR(
                        -0.8296809990522337,
                        kernel_get(kernel, out, m_(out, d++, n_in)),
                        t);
                    EXPECT_NEAR(
                        +0.3000818279353061,
                        kernel_get(kernel, out, m_(out, d++, n_in)),
                        t);
                    EXPECT_NEAR(
                        -0.1082197850474178,
                        kernel_get(kernel, out, m_(out, d++, n_in)),
                        t);
                    EXPECT_NEAR(
                        +0.03850348693956665,
                        kernel_get(kernel, out, m_(out, d++, n_in)),
                        t);
                    EXPECT_NEAR(
                        -0.01225237053199847,
                        kernel_get(kernel, out, m_(out, d++, n_in)),
                        t);
                    break;

                  default:
                    throw("impossible a (%u)", a);
                    break;
                }
                break;

              case 6:
                d = -v;
                t = 2e-15;

                switch (v) {
                  case 1:
                    EXPECT_NEAR(
                        -1.733333333333333,
                        kernel_get(kernel, out, m_(out, d++, n_in)),
                        t);
                    EXPECT_NEAR(
                        +4.466666666666667,
                        kernel_get(kernel, out, m_(out, d++, n_in)),
                        t);
                    EXPECT_NEAR(
                        -1.733333333333333,
                        kernel_get(kernel, out, m_(out, d++, n_in)),
                        t);
                    break;

                  case 2:
                    EXPECT_NEAR(
                        +0.3844696969696970,
                        kernel_get(kernel, out, m_(out, d++, n_in)),
                        t);
                    EXPECT_NEAR(
                        -1.066919191919192,
                        kernel_get(kernel, out, m_(out, d++, n_in)),
                        t);
                    EXPECT_NEAR(
                        +2.364898989898990,
                        kernel_get(kernel, out, m_(out, d++, n_in)),
                        t);
                    EXPECT_NEAR(
                        -1.066919191919192,
                        kernel_get(kernel, out, m_(out, d++, n_in)),
                        t);
                    EXPECT_NEAR(
                        +0.3844696969696970,
                        kernel_get(kernel, out, m_(out, d++, n_in)),
                        t);
                    break;

                  case 3:
                    EXPECT_NEAR(
                        -0.2143030303030303,
                        kernel_get(kernel, out, m_(out, d++, n_in)),
                        t);
                    EXPECT_NEAR(
                        +0.5987727272727273,
                        kernel_get(kernel, out, m_(out, d++, n_in)),
                        t);
                    EXPECT_NEAR(
                        -1.424090909090909,
                        kernel_get(kernel, out, m_(out, d++, n_in)),
                        t);
                    EXPECT_NEAR(
                        +3.079242424242424,
                        kernel_get(kernel, out, m_(out, d++, n_in)),
                        t);
                    EXPECT_NEAR(
                        -1.424090909090909,
                        kernel_get(kernel, out, m_(out, d++, n_in)),
                        t);
                    EXPECT_NEAR(
                        +0.5987727272727273,
                        kernel_get(kernel, out, m_(out, d++, n_in)),
                        t);
                    EXPECT_NEAR(
                        -0.2143030303030303,
                        kernel_get(kernel, out, m_(out, d++, n_in)),
                        t);
                    break;

                  case 4:
                    EXPECT_NEAR(
                        +0.08220782066935913,
                        kernel_get(kernel, out, m_(out, d++, n_in)),
                        t);
                    EXPECT_NEAR(
                        -0.2298477818477818,
                        kernel_get(kernel, out, m_(out, d++, n_in)),
                        t);
                    EXPECT_NEAR(
                        +0.5503261638646254,
                        kernel_get(kernel, out, m_(out, d++, n_in)),
                        t);
                    EXPECT_NEAR(
                        -1.275929995929996,
                        kernel_get(kernel, out, m_(out, d++, n_in)),
                        t);
                    EXPECT_NEAR(
                        +2.746487586487586,
                        kernel_get(kernel, out, m_(out, d++, n_in)),
                        t);
                    EXPECT_NEAR(
                        -1.275929995929996,
                        kernel_get(kernel, out, m_(out, d++, n_in)),
                        t);
                    EXPECT_NEAR(
                        +0.5503261638646254,
                        kernel_get(kernel, out, m_(out, d++, n_in)),
                        t);
                    EXPECT_NEAR(
                        -0.2298477818477818,
                        kernel_get(kernel, out, m_(out, d++, n_in)),
                        t);
                    EXPECT_NEAR(
                        +0.08220782066935913,
                        kernel_get(kernel, out, m_(out, d++, n_in)),
                        t);
                    break;

                  case 5:
                    EXPECT_NEAR(
                        -0.03716725263686590,
                        kernel_get(kernel, out, m_(out, d++, n_in)),
                        t);
                    EXPECT_NEAR(
                        +0.1039242666951974,
                        kernel_get(kernel, out, m_(out, d++, n_in)),
                        t);
                    EXPECT_NEAR(
                        -0.2489922600419838,
                        kernel_get(kernel, out, m_(out, d++, n_in)),
                        t);
                    EXPECT_NEAR(
                        +0.5811457277670622,
                        kernel_get(kernel, out, m_(out, d++, n_in)),
                        t);
                    EXPECT_NEAR(
                        -1.341163440610954,
                        kernel_get(kernel, out, m_(out, d++, n_in)),
                        t);
                    EXPECT_NEAR(
                        +2.884505917655089,
                        kernel_get(kernel, out, m_(out, d++, n_in)),
                        t);
                    EXPECT_NEAR(
                        -1.341163440610954,
                        kernel_get(kernel, out, m_(out, d++, n_in)),
                        t);
                    EXPECT_NEAR(
                        +0.5811457277670622,
                        kernel_get(kernel, out, m_(out, d++, n_in)),
                        t);
                    EXPECT_NEAR(
                        -0.2489922600419838,
                        kernel_get(kernel, out, m_(out, d++, n_in)),
                        t);
                    EXPECT_NEAR(
                        +0.1039242666951974,
                        kernel_get(kernel, out, m_(out, d++, n_in)),
                        t);
                    EXPECT_NEAR(
                        -0.03716725263686590,
                        kernel_get(kernel, out, m_(out, d++, n_in)),
                        t);
                    break;

                  case 6:
                    EXPECT_NEAR(
                        +0.01566783317792601,
                        kernel_get(kernel, out, m_(out, d++, n_in)),
                        t);
                    EXPECT_NEAR(
                        -0.04380950866016510,
                        kernel_get(kernel, out, m_(out, d++, n_in)),
                        t);
                    EXPECT_NEAR(
                        +0.1049702354211762,
                        kernel_get(kernel, out, m_(out, d++, n_in)),
                        t);
                    EXPECT_NEAR(
                        -0.2451622120057599,
                        kernel_get(kernel, out, m_(out, d++, n_in)),
                        t);
                    EXPECT_NEAR(
                        +0.5695613663384980,
                        kernel_get(kernel, out, m_(out, d++, n_in)),
                        t);
                    EXPECT_NEAR(
                        -1.313306144711206,
                        kernel_get(kernel, out, m_(out, d++, n_in)),
                        t);
                    EXPECT_NEAR(
                        +2.824156860879061,
                        kernel_get(kernel, out, m_(out, d++, n_in)),
                        t);
                    EXPECT_NEAR(
                        -1.313306144711206,
                        kernel_get(kernel, out, m_(out, d++, n_in)),
                        t);
                    EXPECT_NEAR(
                        +0.5695613663384980,
                        kernel_get(kernel, out, m_(out, d++, n_in)),
                        t);
                    EXPECT_NEAR(
                        -0.2451622120057599,
                        kernel_get(kernel, out, m_(out, d++, n_in)),
                        t);
                    EXPECT_NEAR(
                        +0.1049702354211762,
                        kernel_get(kernel, out, m_(out, d++, n_in)),
                        t);
                    EXPECT_NEAR(
                        -0.04380950866016510,
                        kernel_get(kernel, out, m_(out, d++, n_in)),
                        t);
                    EXPECT_NEAR(
                        +0.01566783317792601,
                        kernel_get(kernel, out, m_(out, d++, n_in)),
                        t);
                    break;

                  case 7:
                    EXPECT_NEAR(
                        -0.006807782905020841,
                        kernel_get(kernel, out, m_(out, d++, n_in)),
                        t);
                    EXPECT_NEAR(
                        +0.01903555025445177,
                        kernel_get(kernel, out, m_(out, d++, n_in)),
                        t);
                    EXPECT_NEAR(
                        -0.04561063488437055,
                        kernel_get(kernel, out, m_(out, d++, n_in)),
                        t);
                    EXPECT_NEAR(
                        +0.1065325457303594,
                        kernel_get(kernel, out, m_(out, d++, n_in)),
                        t);
                    EXPECT_NEAR(
                        -0.2476608103316123,
                        kernel_get(kernel, out, m_(out, d++, n_in)),
                        t);
                    EXPECT_NEAR(
                        +0.5748740071573847,
                        kernel_get(kernel, out, m_(out, d++, n_in)),
                        t);
                    EXPECT_NEAR(
                        -1.325346258310560,
                        kernel_get(kernel, out, m_(out, d++, n_in)),
                        t);
                    EXPECT_NEAR(
                        +2.849966766578736,
                        kernel_get(kernel, out, m_(out, d++, n_in)),
                        t);
                    EXPECT_NEAR(
                        -1.325346258310560,
                        kernel_get(kernel, out, m_(out, d++, n_in)),
                        t);
                    EXPECT_NEAR(
                        +0.5748740071573847,
                        kernel_get(kernel, out, m_(out, d++, n_in)),
                        t);
                    EXPECT_NEAR(
                        -0.2476608103316123,
                        kernel_get(kernel, out, m_(out, d++, n_in)),
                        t);
                    EXPECT_NEAR(
                        +0.1065325457303594,
                        kernel_get(kernel, out, m_(out, d++, n_in)),
                        t);
                    EXPECT_NEAR(
                        -0.04561063488437055,
                        kernel_get(kernel, out, m_(out, d++, n_in)),
                        t);
                    EXPECT_NEAR(
                        +0.01903555025445177,
                        kernel_get(kernel, out, m_(out, d++, n_in)),
                        t);
                    EXPECT_NEAR(
                        -0.006807782905020841,
                        kernel_get(kernel, out, m_(out, d++, n_in)),
                        t);
                    break;

                  default:
                    throw("impossible a (%u)", a);
                    break;
                }
                break;

              default:
                throw("impossible a (%u)", a);
                break;
            }
          }
        }
      }

      delete(&kernels, num_kernels, kernel_delete);
    }

    deletev(&vs);
  }

  /* Now test k = 1/2.
   */


}

TEST(Magic, MagicKernelResizer) {
  byte a;
  byte b;
  byte num_kernels;
  ulong n_in;
  ulong n_out;
  double kk;
  Kernel **kernels = null;

  for (a = 1; a <= 6; a++) {
    for (b = 0; b < 2; b++) {
      if (b == 0) {
        kk = 2;
        n_in = a + 1;
        n_out = 2 * n_in;
      } else {
        kk = 0.5;
        n_out = a + 1;
        n_in = 2 * n_out;
      }

      num_kernels = kernels_new_resize(
          &kernels,
          n_in,
          n_out,
          true,
          kk,
          MAGIC_RESIZER_MAGIC_KERNEL,
          a,
          0);
      EXPECT_ULEQ(1, num_kernels);
      check_magic_(kernels[0], a, kk);
      delete(&kernels, num_kernels, kernel_delete);
    }
  }
}

TEST(Magic, LanczosResizer) {
  byte a;
  byte b;
  byte num_kernels;
  ulong n_in;
  ulong n_out;
  double kk;
  Kernel **kernels = null;

  for (a = 2; a <= 3; a++) {
    for (b = 0; b < 2; b++) {
      if (b == 0) {
        kk = 2;
        n_in = 2 * a + 1;
        n_out = 2 * n_in;
      } else {
        kk = 0.5;
        n_out = 2 * a + 1;
        n_in = 2 * n_out;
      }

      num_kernels = kernels_new_resize(
          &kernels,
          n_in,
          n_out,
          true,
          kk,
          MAGIC_RESIZER_LANCZOS,
          a,
          0);
      EXPECT_ULEQ(1, num_kernels);
      check_lanczos_(kernels[0], a, kk);
      delete(&kernels, num_kernels, kernel_delete);
    }
  }
}

TEST(Magic, LinearResizer) {
  byte b;
  byte num_kernels;
  ulong n_in;
  ulong n_out;
  double kk;
  Kernel **kernels = null;

  /* Should be the same as Magic Kernel 2.
   */
  for (b = 0; b < 2; b++) {
    if (b == 0) {
      kk = 2;
      n_in = 10;
      n_out = 2 * n_in;
    } else {
      kk = 0.5;
      n_out = 10;
      n_in = 2 * n_out;
    }

    num_kernels = kernels_new_resize(
        &kernels,
        n_in,
        n_out,
        true,
        kk,
        MAGIC_RESIZER_LINEAR,
        0,
        0);
    EXPECT_ULEQ(1, num_kernels);
    check_magic_(kernels[0], 2, kk);
    delete(&kernels, num_kernels, kernel_delete);
  }
}

TEST(Magic, NearestNeighborResizer) {
  byte b;
  byte num_kernels;
  ulong n_in;
  ulong n_out;
  double kk;
  Kernel **kernels = null;

  /* Should be the same as Magic Kernel 1.
   */
  for (b = 0; b < 2; b++) {
    if (b == 0) {
      kk = 2;
      n_in = 10;
      n_out = 2 * n_in;
    } else {
      kk = 0.5;
      n_out = 10;
      n_in = 2 * n_out;
    }

    num_kernels = kernels_new_resize(
        &kernels,
        n_in,
        n_out,
        true,
        kk,
        MAGIC_RESIZER_NEAREST_NEIGHBOR,
        0,
        0);
    EXPECT_ULEQ(1, num_kernels);
    check_magic_(kernels[0], 1, kk);
    delete(&kernels, num_kernels, kernel_delete);
  }
}

TEST(Magic, Blur) {
  double kk;
  double t;
  ulong n;
  ulong out;
  Kernel *k = null;

  /* Test effectively no blurring, using a technicality to avoid the exception
   * being thrown for blur = 0.
   */
  n = 4;
  kk = 1e-308;
  t = 1e-15;
  kernel_new_blur(&k, n, true, kk);
  EXPECT_ULEQ(n, k->n_in);
  EXPECT_ULEQ(n, k->n_out);

  for (out = 0; out < n; out++) {
    EXPECT_NEAR(1, kernel_get(k, out, out), t);
  }

  kernel_delete(&k);

  /* Test blur = 1/4 which maps to k = 3/4 with the linear interpolation for
   * values of blur less than 1.
   */
  n = 5;
  kk = 0.25;
  kernel_new_blur(&k, n, true, kk);
  EXPECT_ULEQ(n, k->n_in);
  EXPECT_ULEQ(n, k->n_out);

  for (out = 0; out < n; out++) {
    EXPECT_NEAR(1.0 / 56, kernel_get(k, out, m_(out, -1, n)), t);
    EXPECT_NEAR(27.0 / 28, kernel_get(k, out, out), t);
    EXPECT_NEAR(1.0 / 56, kernel_get(k, out, m_(out, +1, n)), t);
  }

  kernel_delete(&k);

  /* Test blur = 1/2 which maps to k = 5/6 with the linear interpolation for
   * values of blur less than 1.
   */
  kk = 0.5;
  kernel_new_blur(&k, n, true, kk);

  for (out = 0; out < n; out++) {
    EXPECT_NEAR(9.0 / 168, kernel_get(k, out, m_(out, -1, n)), t);
    EXPECT_NEAR(75.0 / 84, kernel_get(k, out, out), t);
    EXPECT_NEAR(9.0 / 168, kernel_get(k, out, m_(out, +1, n)), t);
  }

  kernel_delete(&k);

  /* Test blur = 3/4 which maps to k = 11/12 with the linear interpolation for
   * values of blur less than 1.
   */
  kk = 0.75;
  kernel_new_blur(&k, n, true, kk);

  for (out = 0; out < n; out++) {
    EXPECT_NEAR(81.0 / 888, kernel_get(k, out, m_(out, -1, n)), t);
    EXPECT_NEAR(363.0 / 444, kernel_get(k, out, out), t);
    EXPECT_NEAR(81.0 / 888, kernel_get(k, out, m_(out, +1, n)), t);
  }

  kernel_delete(&k);

  /* Test blur = 1 (k = 1).
   */
  kk = 1;
  kernel_new_blur(&k, n, true, kk);

  for (out = 0; out < n; out++) {
    EXPECT_NEAR(1.0 / 8, kernel_get(k, out, m_(out, -1, n)), t);
    EXPECT_NEAR(3.0 / 4, kernel_get(k, out, out), t);
    EXPECT_NEAR(1.0 / 8, kernel_get(k, out, m_(out, +1, n)), t);
  }

  kernel_delete(&k);

  /* Test k = 4/3, which is the greatest value that remains three-tap.
   */
  kk = 4.0 / 3;
  kernel_new_blur(&k, n, true, kk);

  for (out = 0; out < n; out++) {
    EXPECT_NEAR(3.0 / 14, kernel_get(k, out, m_(out, -1, n)), t);
    EXPECT_NEAR(4.0 / 7, kernel_get(k, out, out), t);
    EXPECT_NEAR(3.0 / 14, kernel_get(k, out, m_(out, +1, n)), t);
  }

  kernel_delete(&k);

  /* k = 3/2.
   */
  kk = 1.5;
  n = 7;
  kernel_new_blur(&k, n, true, kk);

  for (out = 0; out < n; out++) {
    EXPECT_NEAR(1.0 / 106, kernel_get(k, out, m_(out, -2, n)), t);
    EXPECT_NEAR(25.0 / 106, kernel_get(k, out, m_(out, -1, n)), t);
    EXPECT_NEAR(27.0 / 53, kernel_get(k, out, out), t);
    EXPECT_NEAR(25.0 / 106, kernel_get(k, out, m_(out, +1, n)), t);
    EXPECT_NEAR(1.0 / 106, kernel_get(k, out, m_(out, +2, n)), t);
  }

  kernel_delete(&k);

  /* k = 2.
   */
  kk = 2;
  kernel_new_blur(&k, n, true, kk);

  for (out = 0; out < n; out++) {
    EXPECT_NEAR(1.0 / 16, kernel_get(k, out, m_(out, -2, n)), t);
    EXPECT_NEAR(1.0 / 4, kernel_get(k, out, m_(out, -1, n)), t);
    EXPECT_NEAR(3.0 / 8, kernel_get(k, out, out), t);
    EXPECT_NEAR(1.0 / 4, kernel_get(k, out, m_(out, +1, n)), t);
    EXPECT_NEAR(1.0 / 16, kernel_get(k, out, m_(out, +2, n)), t);
  }

  kernel_delete(&k);

  /* k = 3.
   */
  kk = 3;
  n = 11;
  kernel_new_blur(&k, n, true, kk);

  for (out = 0; out < n; out++) {
    EXPECT_NEAR(1.0 / 216, kernel_get(k, out, m_(out, -4, n)), t);
    EXPECT_NEAR(9.0 / 216, kernel_get(k, out, m_(out, -3, n)), t);
    EXPECT_NEAR(25.0 / 216, kernel_get(k, out, m_(out, -2, n)), t);
    EXPECT_NEAR(23.0 / 108, kernel_get(k, out, m_(out, -1, n)), t);
    EXPECT_NEAR(1.0 / 4, kernel_get(k, out, out), t);
    EXPECT_NEAR(23.0 / 108, kernel_get(k, out, m_(out, +1, n)), t);
    EXPECT_NEAR(25.0 / 216, kernel_get(k, out, m_(out, +2, n)), t);
    EXPECT_NEAR(9.0 / 216, kernel_get(k, out, m_(out, +3, n)), t);
    EXPECT_NEAR(1.0 / 216, kernel_get(k, out, m_(out, +4, n)), t);
  }

  kernel_delete(&k);
}

TEST(Magic, Sharpen) {
  ulong n;
  ulong out;
  double s;
  double t;
  Kernel *k = null;

  /* Again, test a nearly-vanishing value of 'sharpening' to get around the
   * input sanity checking on a technicality.
   */
  n = 4;
  s = 1e-308;
  t = 1e-15;
  kernel_new_sharpen(&k, n, true, s);
  EXPECT_ULEQ(n, k->n_in);
  EXPECT_ULEQ(n, k->n_out);

  for (out = 0; out < n; out++) {
    EXPECT_NEAR(1, kernel_get(k, out, out), t);
  }

  kernel_delete(&k);

  /* s = 0.5.
   */
  s = 0.5;
  kernel_new_sharpen(&k, n, true, s);

  for (out = 0; out < n; out++) {
    EXPECT_NEAR(-1.0 / 8, kernel_get(k, out, m_(out, -1, n)), t);
    EXPECT_NEAR(+5.0 / 4, kernel_get(k, out, out), t);
    EXPECT_NEAR(-1.0 / 8, kernel_get(k, out, m_(out, +1, n)), t);
  }

  kernel_delete(&k);

  /* s = 1, which undoes (to first order) the Magic Blur with k = 1, i.e. this
   * is Sharp 2013.
   */
  s = 1;
  kernel_new_sharpen(&k, n, true, s);

  for (out = 0; out < n; out++) {
    EXPECT_NEAR(-1.0 / 4, kernel_get(k, out, m_(out, -1, n)), t);
    EXPECT_NEAR(+3.0 / 2, kernel_get(k, out, out), t);
    EXPECT_NEAR(-1.0 / 4, kernel_get(k, out, m_(out, +1, n)), t);
  }

  kernel_delete(&k);

  /* s = 2.
   */
  s = 2;
  kernel_new_sharpen(&k, n, true, s);

  for (out = 0; out < n; out++) {
    EXPECT_NEAR(-1.0 / 2, kernel_get(k, out, m_(out, -1, n)), t);
    EXPECT_NEAR(+2, kernel_get(k, out, out), t);
    EXPECT_NEAR(-1.0 / 2, kernel_get(k, out, m_(out, +1, n)), t);
  }

  kernel_delete(&k);
}

FINISH

static void t_(byte a, double x, double e) {
  double (*f)(double x);
  double (*fs1[])(double x) = {
    null,
    magic_kernel_1,
    magic_kernel_2,
    magic_kernel_3,
    magic_kernel_4,
    magic_kernel_5,
    magic_kernel_6
  };
  double (*fs2[])(double x) = {
    null,
    nearest_neighbor_kernel,
    linear_kernel,
    null,
    null,
    null,
    null
  };

  EXPECT_NEAR(e, magic_kernel(a, x), EPS_0_);

  if (a > 1) {
    EXPECT_NEAR(e, magic_kernel(a, x - EPS_1_), EPS_2_);
    EXPECT_NEAR(e, magic_kernel(a, x + EPS_1_), EPS_2_);

    EXPECT_NEAR(e, fs1[a](x - EPS_1_), EPS_2_);
    EXPECT_NEAR(e, fs1[a](x + EPS_1_), EPS_2_);

    if ((f = fs2[a]) != null) {
      EXPECT_NEAR(e, f(x - EPS_1_), EPS_2_);
      EXPECT_NEAR(e, f(x + EPS_1_), EPS_2_);
    }
  }
}

static double d_(byte a, byte n, double x, double eps) {
  double delta;
  double left;
  double right;

  if (n == 0) {
    return magic_kernel(a, x);
  }

  left = d_(a, n - 1, x - eps, eps);
  right = d_(a, n - 1, x + eps, eps);

  delta = right - left;

  return delta / (2 * eps);
}

static double eps_(byte n) {
  return pow(EPS_4_, 1.0 / (n == 0 ? 1 : n));
}

static void td_(byte a, byte n, double x) {
  double delta;
  double mean;
  double eps;
  double left;
  double right;

  eps = eps_(n);

  left = d_(a, n, x - eps, eps);
  right = d_(a, n, x + eps, eps);

  delta = right - left;
  mean = (left + right) / 2;

  EXPECT_NEAR(0, delta, EPS_3_ * maximum(mean, 1) * a * (n + 1));
}

static void tl_(byte a, double x, double e) {
  double (*fs[])(double x) = {
    null,
    null,
    lanczos_2_kernel,
    lanczos_3_kernel,
    lanczos_4_kernel,
  };

  EXPECT_NEAR(e, lanczos_kernel(a, -x), EPS_0_);
  EXPECT_NEAR(e, lanczos_kernel(a, +x), EPS_0_);

  EXPECT_NEAR(e, fs[a](-x), EPS_0_);
  EXPECT_NEAR(e, fs[a](+x), EPS_0_);
}

static ulong m_(ulong u, long d, ulong n) {
  return (ulong)modulo((long)u + d, n);
}

static void check_magic_(Kernel *k, byte a, double kk){
  bool even;
  ulong n_in;
  ulong n_out;
  ulong out;
  long r;
  double center;
  double t;

  n_in = k->n_in;
  n_out = k->n_out;

  for (out = 0; out < n_out; out++) {
    if (kk == 2) {
      center = kernel_resize_in_from_out(out, kk);
      r = (long)ceil(center - 0.5 * a);
      even = (center - floor(center) > 0.5) ^ (a % 2 == 1);

      switch (a) {
        case 1:
          EXPECT_DEQ(1, kernel_get(k, out, out / 2));
          break;

        case 2:
          EXPECT_DEQ(
              even ? 1.0 / 4 : 3.0 / 4,
              kernel_get(k, out, modulo(r++, n_in)));
          EXPECT_DEQ(
              !even ? 1.0 / 4 : 3.0 / 4,
              kernel_get(k, out, modulo(r++, n_in)));
          break;

        case 3:
          EXPECT_DEQ(
              even ? 1.0 / 32 : 9.0 / 32,
              kernel_get(k, out, modulo(r++, n_in)));
          EXPECT_DEQ(
              11.0 / 16,
              kernel_get(k, out, modulo(r++, n_in)));
          EXPECT_DEQ(
              !even ? 1.0 / 32 : 9.0 / 32,
              kernel_get(k, out, modulo(r++, n_in)));
          break;

        case 4:
          t = 1e-16;
          EXPECT_NEAR(
              even ? 1.0 / 384 : 9.0 / 128,
              kernel_get(k, out, modulo(r++, n_in)),
              t);
          EXPECT_NEAR(
              even ? 121.0 / 384 : 235.0 / 384,
              kernel_get(k, out, modulo(r++, n_in)),
              t);
          EXPECT_NEAR(
              !even ? 121.0 / 384 : 235.0 / 384,
              kernel_get(k, out, modulo(r++, n_in)),
              t);
          EXPECT_NEAR(
              !even ? 1.0 / 384 : 9.0 / 128,
              kernel_get(k, out, modulo(r++, n_in)),
              t);
          break;

        case 5:
          t = 1e-15;
          EXPECT_NEAR(
              even ? 1.0 / 6144 : 27.0 / 2048,
              kernel_get(k, out, modulo(r++, n_in)),
              t);
          EXPECT_NEAR(
              even ? 155.0 / 1536 : 499.0 / 1536,
              kernel_get(k, out, modulo(r++, n_in)),
              t);
          EXPECT_NEAR(
              1723.0 / 3072,
              kernel_get(k, out, modulo(r++, n_in)),
              t);
          EXPECT_NEAR(
              !even ? 155.0 / 1536 : 499.0 / 1536,
              kernel_get(k, out, modulo(r++, n_in)),
              t);
          EXPECT_NEAR(
              !even ? 1.0 / 6144 : 27.0 / 2048,
              kernel_get(k, out, modulo(r++, n_in)),
              t);
          break;

        case 6:
          t = 1e-15;
          EXPECT_NEAR(
              even ? 1.0 / 122880 : 81.0 / 40960,
              kernel_get(k, out, modulo(r++, n_in)),
              t);
          EXPECT_NEAR(
              even ? 3119.0 / 122880 : 15349.0 / 122880,
              kernel_get(k, out, modulo(r++, n_in)),
              t);
          EXPECT_NEAR(
              even ? 6719.0 / 20480 : 31927.0 / 61440,
              kernel_get(k, out, modulo(r++, n_in)),
              t);
          EXPECT_NEAR(
              !even ? 6719.0 / 20480 : 31927.0 / 61440,
              kernel_get(k, out, modulo(r++, n_in)),
              t);
          EXPECT_NEAR(
              !even ? 3119.0 / 122880 : 15349.0 / 122880,
              kernel_get(k, out, modulo(r++, n_in)),
              t);
          EXPECT_NEAR(
              !even ? 1.0 / 122880 : 81.0 / 40960,
              kernel_get(k, out, modulo(r++, n_in)),
              t);
          break;

        default:
          throw("impossible");
          break;
      }
    } else {
      center = kernel_resize_in_from_out(out, kk);
      r = (long)ceil(center - 0.5 * a / kk);

      switch (a) {
        case 1:
          EXPECT_DEQ(0.5, kernel_get(k, out, modulo(r++, n_in)));
          EXPECT_DEQ(0.5, kernel_get(k, out, modulo(r++, n_in)));
          break;

        case 2:
          EXPECT_DEQ(1.0 / 8, kernel_get(k, out, modulo(r++, n_in)));
          EXPECT_DEQ(3.0 / 8, kernel_get(k, out, modulo(r++, n_in)));
          EXPECT_DEQ(3.0 / 8, kernel_get(k, out, modulo(r++, n_in)));
          EXPECT_DEQ(1.0 / 8, kernel_get(k, out, modulo(r++, n_in)));
          break;

        case 3:
          EXPECT_DEQ(1.0 / 64, kernel_get(k, out, modulo(r++, n_in)));
          EXPECT_DEQ(9.0 / 64, kernel_get(k, out, modulo(r++, n_in)));
          EXPECT_DEQ(11.0 / 32, kernel_get(k, out, modulo(r++, n_in)));
          EXPECT_DEQ(11.0 / 32, kernel_get(k, out, modulo(r++, n_in)));
          EXPECT_DEQ(9.0 / 64, kernel_get(k, out, modulo(r++, n_in)));
          EXPECT_DEQ(1.0 / 64, kernel_get(k, out, modulo(r++, n_in)));
          break;

        case 4:
          t = 1e-16;
          EXPECT_NEAR(1.0 / 768, kernel_get(k, out, modulo(r++, n_in)), t);
          EXPECT_NEAR(9.0 / 256, kernel_get(k, out, modulo(r++, n_in)), t);
          EXPECT_NEAR(121.0 / 768, kernel_get(k, out, modulo(r++, n_in)), t);
          EXPECT_NEAR(235.0 / 768, kernel_get(k, out, modulo(r++, n_in)), t);
          EXPECT_NEAR(235.0 / 768, kernel_get(k, out, modulo(r++, n_in)), t);
          EXPECT_NEAR(121.0 / 768, kernel_get(k, out, modulo(r++, n_in)), t);
          EXPECT_NEAR(9.0 / 256, kernel_get(k, out, modulo(r++, n_in)), t);
          EXPECT_NEAR(1.0 / 768, kernel_get(k, out, modulo(r++, n_in)), t);
          break;

        case 5:
          t = 2e-16;
          EXPECT_NEAR(1.0 / 12288, kernel_get(k, out, modulo(r++, n_in)), t);
          EXPECT_NEAR(27.0 / 4096, kernel_get(k, out, modulo(r++, n_in)), t);
          EXPECT_NEAR(155.0 / 3072, kernel_get(k, out, modulo(r++, n_in)), t);
          EXPECT_NEAR(499.0 / 3072, kernel_get(k, out, modulo(r++, n_in)), t);
          EXPECT_NEAR(1723.0 / 6144, kernel_get(k, out, modulo(r++, n_in)), t);
          EXPECT_NEAR(1723.0 / 6144, kernel_get(k, out, modulo(r++, n_in)), t);
          EXPECT_NEAR(499.0 / 3072, kernel_get(k, out, modulo(r++, n_in)), t);
          EXPECT_NEAR(155.0 / 3072, kernel_get(k, out, modulo(r++, n_in)), t);
          EXPECT_NEAR(27.0 / 4096, kernel_get(k, out, modulo(r++, n_in)), t);
          EXPECT_NEAR(1.0 / 12288, kernel_get(k, out, modulo(r++, n_in)), t);
          break;

        case 6:
          t = 4e-16;
          EXPECT_NEAR(1.0 / 245760, kernel_get(k, out, modulo(r++, n_in)), t);
          EXPECT_NEAR(81.0 / 81920, kernel_get(k, out, modulo(r++, n_in)), t);
          EXPECT_NEAR(
              3119.0 / 245760,
              kernel_get(k, out, modulo(r++, n_in)),
              t);
          EXPECT_NEAR(
              15349.0 / 245760,
              kernel_get(k, out, modulo(r++, n_in)),
              t);
          EXPECT_NEAR(6719.0 / 40960, kernel_get(k, out, modulo(r++, n_in)), t);
          EXPECT_NEAR(
              31927.0 / 122880,
              kernel_get(k, out, modulo(r++, n_in)),
              t);
          EXPECT_NEAR(
              31927.0 / 122880,
              kernel_get(k, out, modulo(r++, n_in)),
              t);
          EXPECT_NEAR(6719.0 / 40960, kernel_get(k, out, modulo(r++, n_in)), t);
          EXPECT_NEAR(
              15349.0 / 245760,
              kernel_get(k, out, modulo(r++, n_in)),
              t);
          EXPECT_NEAR(
              3119.0 / 245760,
              kernel_get(k, out, modulo(r++, n_in)),
              t);
          EXPECT_NEAR(81.0 / 81920, kernel_get(k, out, modulo(r++, n_in)), t);
          EXPECT_NEAR(1.0 / 245760, kernel_get(k, out, modulo(r++, n_in)), t);
          break;

        default:
          throw("impossible");
          break;
      }
    }
  }
}

static void check_lanczos_(Kernel *k, byte a, double kk){
  bool even;
  ulong n_in;
  ulong n_out;
  ulong out;
  long r;
  double center;
  double t;

  n_in = k->n_in;
  n_out = k->n_out;

  for (out = 0; out < n_out; out++) {
    if (kk == 2) {
      center = kernel_resize_in_from_out(out, kk);
      r = (long)ceil(center - a);
      even = (center - floor(center) > 0.5) ^ (a % 2 == 1);

      switch (a) {
        case 2:
          t = 1e-15;
          EXPECT_NEAR(
              even ? -0.0177266641518006 : -0.0838800679013836,
              kernel_get(k, out, modulo(r++, n_in)),
              t);
          EXPECT_NEAR(
              even ? +0.233000188614954 : +0.868606543438230,
              kernel_get(k, out, modulo(r++, n_in)),
              t);
          EXPECT_NEAR(
              !even ? +0.233000188614954 : +0.868606543438230,
              kernel_get(k, out, modulo(r++, n_in)),
              t);
          EXPECT_NEAR(
              !even ? -0.0177266641518006 : -0.0838800679013836,
              kernel_get(k, out, modulo(r++, n_in)),
              t);
          break;

        case 3:
          t = 1e-15;
          EXPECT_NEAR(
              !even ? +0.00737827086020932 : +0.0301122853618977,
              kernel_get(k, out, modulo(r++, n_in)),
              t);
          EXPECT_NEAR(
              !even ? -0.0679972630285518 : -0.133274635535962,
              kernel_get(k, out, modulo(r++, n_in)),
              t);
          EXPECT_NEAR(
              !even ? +0.271010568257079 : +0.892770774085327,
              kernel_get(k, out, modulo(r++, n_in)),
              t);
          EXPECT_NEAR(
              even ? +0.271010568257079 : +0.892770774085327,
              kernel_get(k, out, modulo(r++, n_in)),
              t);
          EXPECT_NEAR(
              even ? -0.0679972630285518 : -0.133274635535962,
              kernel_get(k, out, modulo(r++, n_in)),
              t);
          EXPECT_NEAR(
              even ? +0.00737827086020932 : +0.0301122853618977,
              kernel_get(k, out, modulo(r++, n_in)),
              t);
          break;

        default:
          throw("impossible");
          break;
      }
    } else {
      center = kernel_resize_in_from_out(out, kk);
      r = (long)ceil(center - a / kk);

      switch (a) {
        case 2:
          t = 1e-15;
          EXPECT_NEAR(
              -0.0177266641518006 / 2,
              kernel_get(k, out, modulo(r++, n_in)),
              t);
          EXPECT_NEAR(
              -0.0838800679013836 / 2,
              kernel_get(k, out, modulo(r++, n_in)),
              t);
          EXPECT_NEAR(
              +0.233000188614954 / 2,
              kernel_get(k, out, modulo(r++, n_in)),
              t);
          EXPECT_NEAR(
              +0.868606543438230 / 2,
              kernel_get(k, out, modulo(r++, n_in)),
              t);
          EXPECT_NEAR(
              +0.868606543438230 / 2,
              kernel_get(k, out, modulo(r++, n_in)),
              t);
          EXPECT_NEAR(
              +0.233000188614954 / 2,
              kernel_get(k, out, modulo(r++, n_in)),
              t);
          EXPECT_NEAR(
              -0.0838800679013836 / 2,
              kernel_get(k, out, modulo(r++, n_in)),
              t);
          EXPECT_NEAR(
              -0.0177266641518006 / 2,
              kernel_get(k, out, modulo(r++, n_in)),
              t);
          break;

        case 3:
          t = 1e-15;
          EXPECT_NEAR(
              +0.00737827086020932 / 2,
              kernel_get(k, out, modulo(r++, n_in)),
              t);
          EXPECT_NEAR(
              +0.0301122853618977 / 2,
              kernel_get(k, out, modulo(r++, n_in)),
              t);
          EXPECT_NEAR(
              -0.0679972630285518 / 2,
              kernel_get(k, out, modulo(r++, n_in)),
              t);
          EXPECT_NEAR(
              -0.133274635535962 / 2,
              kernel_get(k, out, modulo(r++, n_in)),
              t);
          EXPECT_NEAR(
              +0.271010568257079 / 2,
              kernel_get(k, out, modulo(r++, n_in)),
              t);
          EXPECT_NEAR(
              +0.892770774085327 / 2,
              kernel_get(k, out, modulo(r++, n_in)),
              t);
          EXPECT_NEAR(
              +0.892770774085327 / 2,
              kernel_get(k, out, modulo(r++, n_in)),
              t);
          EXPECT_NEAR(
              +0.271010568257079 / 2,
              kernel_get(k, out, modulo(r++, n_in)),
              t);
          EXPECT_NEAR(
              -0.133274635535962 / 2,
              kernel_get(k, out, modulo(r++, n_in)),
              t);
          EXPECT_NEAR(
              -0.0679972630285518 / 2,
              kernel_get(k, out, modulo(r++, n_in)),
              t);
          EXPECT_NEAR(
              +0.0301122853618977 / 2,
              kernel_get(k, out, modulo(r++, n_in)),
              t);
          EXPECT_NEAR(
              +0.00737827086020932 / 2,
              kernel_get(k, out, modulo(r++, n_in)),
              t);
          break;

        default:
          throw("impossible");
          break;
      }
    }
  }
}
