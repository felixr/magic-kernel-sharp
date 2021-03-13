#include "double_image_kernel_test.h"
#include <math.h>

static KernelPossible possible_(ulong out, void *x);
static double weight_(ulong out, long in, void *x);

START

TEST(DoubleImageKernel, ApplySinglePair) {
  ulong i;
  ulong j;
  double x_scale;
  double y_scale;
  Kernel *x_kernel = null;
  Kernel *y_kernel = null;
  DoubleImage *din = null;
  DoubleImage *dout = null;

  double_image_new(&din, 3, 3, true);
  din->r[0][1] = 1;
  din->g[1][2] = 2;
  din->b[2][0] = 3;
  din->a[1][1] = 0.5;
  x_scale = 2;
  kernel_new(&x_kernel, 3, 4, false, possible_, weight_, &x_scale);
  y_scale = 3;
  kernel_new(&y_kernel, 3, 6, false, possible_, weight_, &y_scale);
  double_image_new_apply_xy_kernels(&dout, din, x_kernel, y_kernel);

  for (i = 0; i < 4; i++) {
    for (j = 0; j < 6; j++) {
      EXPECT_NEAR(i > 1 ? 0 : j < 3 ? 0.09 : 0.01, dout->r[i][j], 1e-16);
      EXPECT_NEAR(j < 3 ? 0 : i < 2 ? 1.62 : 0.18, dout->g[i][j], 1e-16);
      EXPECT_NEAR(i < 2 ? 0 : j < 3 ? 0.27 : 0, dout->b[i][j], 1e-16);
      EXPECT_NEAR(
          i < 2 ? (j < 3 ? 0.595 : 0.955) : j < 3 ? 0.955 : 0.995,
          dout->a[i][j], 1e-15);
    }
  }

  double_image_delete(&dout);
  kernel_delete(&y_kernel);
  kernel_delete(&x_kernel);
  double_image_delete(&din);
}

TEST(DoubleImageKernel, ApplyDoublePair) {
  ulong i;
  ulong j;
  double x_scale;
  double y_scale;
  Kernel **x_kernels = null;
  Kernel **y_kernels = null;
  DoubleImage *din = null;
  DoubleImage *dout = null;

  /* Aritificially split the above into two separate pairs for testing
   * purposes, even though this will never actually occur in practice.
   */
  new(&x_kernels, 2);
  new(&y_kernels, 2);
  double_image_new(&din, 3, 3, true);
  din->r[0][1] = 1;
  din->g[1][2] = 2;
  din->b[2][0] = 3;
  din->a[1][1] = 0.5;
  x_scale = 2;
  kernel_new(&x_kernels[0], 3, 4, false, possible_, weight_, &x_scale);
  kernel_new_identity(&y_kernels[0], 3);
  kernel_new_identity(&x_kernels[1], 4);
  y_scale = 3;
  kernel_new(&y_kernels[1], 3, 6, false, possible_, weight_, &y_scale);
  double_image_new_apply_multiple_xy_kernels(
      &dout,
      din,
      2,
      x_kernels,
      y_kernels);

  for (i = 0; i < 4; i++) {
    for (j = 0; j < 6; j++) {
      EXPECT_NEAR(i > 1 ? 0 : j < 3 ? 0.09 : 0.01, dout->r[i][j], 1e-16);
      EXPECT_NEAR(j < 3 ? 0 : i < 2 ? 1.62 : 0.18, dout->g[i][j], 1e-16);
      EXPECT_NEAR(i < 2 ? 0 : j < 3 ? 0.27 : 0, dout->b[i][j], 1e-16);
      EXPECT_NEAR(
          i < 2 ? (j < 3 ? 0.595 : 0.955) : j < 3 ? 0.955 : 0.995,
          dout->a[i][j], 1e-15);
    }
  }

  double_image_delete(&dout);
  double_image_delete(&din);
  delete(&y_kernels, 2, kernel_delete);
  delete(&x_kernels, 2, kernel_delete);
}

FINISH

static KernelPossible possible_(ulong out, void *x) {
  double scale;
  KernelPossible possible;

  scale = *(double *)x;

  if (scale >= 1) {
    possible.first = (long)floor((double)out / scale);
    possible.last = (long)ceil((double)out / scale + 1);
  } else {
    possible.first = (long)floor((double)out / scale);
    possible.last = (long)ceil((double)(out + 1) / scale);
  }

  return possible;
}

static double weight_(ulong out, long in, void *x) {
  long rel;
  double scale;

  scale = *(double *)x;

  if (scale >= 1) {
   rel = in - (long)floor((double)out / scale);
  } else {
    rel = in - (long)floor((double)out / scale);
  }

  switch (rel) {
    case 0:
      return 1;
    case 1:
      return 9;
    case 2:
      return 0;
    default:
      throw("impossible");
      return 0;
  }
}

