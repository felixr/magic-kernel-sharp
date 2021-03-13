#include "double_image_kernel_test_death.h"

START_DEATH

TEST(DoubleImageKernel, Death) {
  Kernel *x_kernel = null;
  Kernel *y_kernel = null;
  DoubleImage *din = null;
  DoubleImage *dout = null;

  double_image_new(&din, 2, 3, true);
  kernel_new_identity(&x_kernel, 4);
  kernel_new_identity(&y_kernel, 3);
  EXPECT_DEATH(
      double_image_new_apply_xy_kernels(&dout, din, x_kernel, y_kernel),
      "X kernel n_in (4) must equal input image width (2)");
  double_image_delete(&dout);
  kernel_delete(&y_kernel);
  kernel_delete(&x_kernel);
  kernel_new_identity(&x_kernel, 2);
  kernel_new_identity(&y_kernel, 5);
  EXPECT_DEATH(
      double_image_new_apply_xy_kernels(&dout, din, x_kernel, y_kernel),
      "Y kernel n_in (5) must equal input image height (3)");
  double_image_delete(&dout);
  kernel_delete(&y_kernel);
  kernel_delete(&x_kernel);
  double_image_delete(&din);
}

FINISH_DEATH
