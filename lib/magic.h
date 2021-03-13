/* The Magic Kernel Sharp and related kernels, per johncostella.com/magic.
 */
#ifndef MAGIC_H
#define MAGIC_H

#ifdef __cplusplus
extern "C" {
#endif

#include "Kernel.h"
#include "sinc.h"

/* The resizing methods, and descriptions of them.
 */
#define MAGIC_RESIZER_MAGIC_KERNEL_SHARP "MAGIC_KERNEL_SHARP"
#define MAGIC_RESIZER_MAGIC_KERNEL "MAGIC_KERNEL"
#define MAGIC_RESIZER_LANCZOS "LANCZOS"
#define MAGIC_RESIZER_LINEAR "LINEAR"
#define MAGIC_RESIZER_NEAREST_NEIGHBOR "NEAREST_NEIGHBOR"

#ifdef MAGIC_C
char *MAGIC_RESIZERS[] = {
  MAGIC_RESIZER_MAGIC_KERNEL_SHARP, "Magic Kernel Sharp, as described in "
    "johncostella.com/magic/mks.pdf. Valid values of 'a' are 1 through\f6. "
    "1\fis equivalent to NEAREST_NEIGHBOR, and 2 is equivalent to LINEAR; "
    "'v'\fis ignored for these two kernels. Valid values of 'v' are 1, 3, "
    "and\f7 for a = 3 or\f4; 1 through\f5 for a\f=\f5; and 1 through\f7 for "
    "a\f=\f6.",
  MAGIC_RESIZER_MAGIC_KERNEL, "The Magic Kernel, without any Sharp step. "
    "Valid values of 'a' are 1\fthrough\f6; 1\fis equivalent to "
    "NEAREST_NEIGHBOR, and 2\fis equivalent to LINEAR.",
  MAGIC_RESIZER_LINEAR, "Linear interpolation. 'a'\fand 'v' are ignored.",
  MAGIC_RESIZER_NEAREST_NEIGHBOR, "Nearest neighbor. For downsizing, "
    "equivalent to simple averaging over the corresponding 'tile' in the input "
    "space. 'a'\fand 'v' are ignored.",
  MAGIC_RESIZER_LANCZOS, "Lanczos kernels. 'a' must be 2 or\f3. 'v'\fis "
    "ignored."
};
byte MAGIC_NUM_RESIZERS
  = sizeof(MAGIC_RESIZERS) / (2 * sizeof(char *));
#else
extern char *MAGIC_RESIZERS[];
extern byte MAGIC_NUM_RESIZERS;
#endif

/* The formula for the Magic Kernel, m_a(x). Valid values of 'a' are 1-6.
 */
double magic_kernel(byte a, double x);

/* Synonyms for these formulas with the value of 'a' baked in.
 */
double magic_kernel_1(double x);
double magic_kernel_2(double x);
double magic_kernel_3(double x);
double magic_kernel_4(double x);
double magic_kernel_5(double x);
double magic_kernel_6(double x);

/* Synonym for the nearest neighbor kernel, m_1(x) = k_1(x).
 */
double nearest_neighbor_kernel(double x);

/* Synonym for the linear kernel, m_2(x) = k_2(x).
 */
double linear_kernel(double x);

/* The formulas for the Lanczos kernels, L_a(x).
 */
double lanczos_kernel(byte a, double x);

/* Synonyms for the Lnczos formulas with a = 2 to 4 baked in.
 */
double lanczos_2_kernel(double x);
double lanczos_3_kernel(double x);
double lanczos_4_kernel(double x);

/* Create the array of resizing kernels corresponding to one of the resizing
 * methods listed above. Returns the number of kernels in the array. They must
 * be applied in the order returned. The value of the resizing factor, k, must
 * be consistent with n_in and n_out: when k > 1, n_in must lie in the interval
 * [floor(n_out / k), ceil(n_out / k)]; when k < 1, n_out must lie in the
 * interval [floor(k n_in), ceil(k n_in)]. This effectively ensures that all
 * input and output tiles are connected in some way; i.e., cropping or
 * extending an image (other than to the nearest pixel) is not supported here.
 * These intervals permit rounding n_out up or down, depending on the
 * particular need. See kernel.h for the 'periodic' flag.
 */
byte kernels_new_resize(
    Kernel ***pthis,
    ulong n_in,
    ulong n_out,
    bool periodic,
    double k,
    char *method,
    byte a,
    byte v);

/* Create the Magic Blur kernel, which uses the Continuum Magic Kernel as a
 * blurring kernel. 'blur' must be positive; in the limit that 'blur'
 * approaches zero, the blurring vanishes. 'blur' can be thought of, roughly,
 * as the "blurring diameter"; the standard deviation of the point spread
 * function is always close to blur / 2, and so a "width" of two standard
 * deviations is always close to 'blur'. For values of 'blur' greater than
 * unity, the k of the Continuum Magic Kernel is set to 'blur'; for values of
 * 'blur' less than unity, a linear interpolation between (blur = 0, k = 2/3)
 * and (blur = 1. k = 1) is made, because the Continuum Magic Kernel's support
 * drops to less than the inter-pixel spacing for k < 2/3. (The above
 * approximate property for the standard deviation still applies, in terms of
 * 'blur', after this transformation is made.)
 */
Kernel *kernel_new_blur(Kernel **pthis, ulong n, bool periodic, double blur);

/* Create a simple three-tap sharpening kernel. The 'sharpening' value is
 * calibrated so that it "undoes," to first order, a blur of the same amount
 * using the blur kernel above.
 */
Kernel *kernel_new_sharpen(
    Kernel **pthis,
    ulong n,
    bool periodic,
    double sharpening);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* _H */
