#include "magic_double_image.h"

DoubleImage *double_image_new_resize(
    DoubleImage **pthis,
    DoubleImage *other,
    ulong width,
    ulong height,
    double k,
    char *method,
    byte a,
    byte v) {
  byte num_kernel_pairs;
  Kernel **x_kernels = null;
  Kernel **y_kernels = null;

  num_kernel_pairs = kernels_new_resize(
      &x_kernels,
      other->width,
      width,
      false,
      k,
      method,
      a,
      v);

  kernels_new_resize(
      &y_kernels,
      other->height,
      height,
      false,
      k,
      method,
      a,
      v);

  double_image_new_apply_multiple_xy_kernels(
    pthis,
    other,
    num_kernel_pairs,
    x_kernels,
    y_kernels);

  delete(&x_kernels, num_kernel_pairs, kernel_delete);
  delete(&y_kernels, num_kernel_pairs, kernel_delete);

  return *pthis;
}

DoubleImage *double_image_new_blur(
    DoubleImage **pthis,
    DoubleImage *other,
    double blur) {
  Kernel *x_kernel = null;
  Kernel *y_kernel = null;

  kernel_new_blur(&x_kernel, other->width, false, blur);
  kernel_new_blur(&y_kernel, other->height, false, blur);

  double_image_new_apply_xy_kernels(pthis, other, x_kernel, y_kernel);

  kernel_delete(&y_kernel);
  kernel_delete(&x_kernel);

  return *pthis;
}

DoubleImage *double_image_new_sharpen(
    DoubleImage **pthis,
    DoubleImage *other,
    double sharpening) {
  Kernel *x_kernel = null;
  Kernel *y_kernel = null;

  kernel_new_sharpen(&x_kernel, other->width, false, sharpening);
  kernel_new_sharpen(&y_kernel, other->height, false, sharpening);

  double_image_new_apply_xy_kernels(pthis, other, x_kernel, y_kernel);

  kernel_delete(&y_kernel);
  kernel_delete(&x_kernel);

  return *pthis;
}
