/* Apply x and y kernels to all channels of a DoubleImage.
 */
#ifndef DOUBLE_IMAGE_KERNEL_H
#define DOUBLE_IMAGE_KERNEL_H

#ifdef __cplusplus
extern "C" {
#endif

#include "DoubleImage.h"
#include "Kernel.h"

/* Create a new DoubleImage by applying x and y kernels to an existing
 * DoubleImage.
 */
DoubleImage *double_image_new_apply_xy_kernels(
    DoubleImage **pthis,
    DoubleImage *other,
    Kernel *x_kernel,
    Kernel *y_kernel);

/* Create a new DoubleImage by applying multiple pairs of x and y kernels to an
 * existing DoubleImage. I.e. x_kernels[0] and y_kernels[0] are applied to
 * 'other', and then x_kernels[1] and y_kernels[1] are applied to the reslt,
 * and so on.
 */
DoubleImage *double_image_new_apply_multiple_xy_kernels(
    DoubleImage **pthis,
    DoubleImage *other,
    ulong num_kernel_pairs,
    Kernel **x_kernels,
    Kernel **y_kernels);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* _H */
