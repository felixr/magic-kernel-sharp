/* Apply Magic Kernel Sharp and related kernels to DoubleImages, as efficiently
 * as possible.
 */
#ifndef MAGIC_DOUBLE_IMAGE_H
#define MAGIC_DOUBLE_IMAGE_H

#ifdef __cplusplus
extern "C" {
#endif

#include "double_image_kernel.h"
#include "magic.h"

/* Resize a DoubleImage. The methods available, and the parameters 'a' and 'v,'
 * are described in magic.h.
 */
DoubleImage *double_image_new_resize(
    DoubleImage **pthis,
    DoubleImage *other,
    ulong width,
    ulong height,
    double k,
    char *method,
    byte a,
    byte v);

/* Blur a DoubleImage. The "width" (two standard deviations) of the blurring is
 * approximately equal to 'blur'.
 */
DoubleImage *double_image_new_blur(
    DoubleImage **pthis,
    DoubleImage *other,
    double blur);

/* Sharpen an image. A sharpening of 1.0 "undoes" a blur of 1.0 above, to first
 * order.
 */
DoubleImage *double_image_new_sharpen(
    DoubleImage **pthis,
    DoubleImage *other,
    double sharpening);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* _H */
