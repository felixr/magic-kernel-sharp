/* A general n_out x n_in normalized linear matrix kernel mapping linearly the
 * n_in elements of an input signal to the n_out elements of the desired output
 * signal, optimized for the case when this matrix is sparse, but still close
 * to as efficient as possible even for dense matrices. Automatically handles
 * boundary conditions, either periodic or extension. Application to a
 * particular signal makes use of parallel execution if the number of threads
 * specified is not 1.
 */
#ifndef KERNEL_H
#define KERNEL_H

#ifdef __cplusplus
extern "C" {
#endif

#include "qsortx.h"
#include "parallel.h"

typedef struct {
  bool periodic;
  ulong n_in;
  ulong n_out;
  ulong num_elements;
  ulong *ns;
  ulong **ins;
  double **weights;
} Kernel;

typedef struct {
  long first;
  long last;
} KernelPossible;

/* Create a kernel. The callback function possible(out, x) should return the
 * first and last *possible* input indexes for the given output index 'out' for
 * which the weight of the theoretical kernel *might*, in general, be nonzero.
 * If it turns out at construction time that some matrix elements within this
 * range are *actually* zero, the class does not physically store those matrix
 * elements. The first and last indexes specified by this 'possible' callback
 * function can in general be *outside* the bounds [0, n_in) of the input index
 * (which is why they are signed longs rather than ulongs). Boundary conditions
 * are handled by this class automatically: if 'periodic' is true, periodic
 * boundary conditions are applied, and any contributions for 'in' outside [0,
 * n_in) are added to the kernel for 'in mod n_in'; otherwise, extension
 * boundary conditions are applied, and such contributions are added to the
 * kernel for in = 0 if in < 0, or in = n_in - 1 if in >= n_in. The function
 * weight(in, out, x) should return the weight (which does *not* need to be
 * normalized; this class will automatically do that) from 'in' to 'out',
 * ignoring the fact that 'in' might be outside the input array, i.e., this
 * function can return a "pure" formula that ignores boundary conditions, which
 * this class will apply automatically according to the above rules (and the
 * class will also normalize the weights for any given value of 'out' to
 * unity).
 */
Kernel *kernel_new(
    Kernel **pthis,
    ulong n_in,
    ulong n_out,
    bool periodic,
    KernelPossible (*possible)(ulong out, void *x),
    double (*weight)(ulong out, long in, void *x),
    void *x);

/* Create a kernel that is the composition of two other kernels, i.e., k1 is
 * intended to be applied to the input signal, and then k2 is applied to the
 * output of k1. Note that this constructor constructs a *single* kernel that
 * is the composition of the two individual kernels, which may not in general
 * be as efficient as keeping the two separate instantiated kernels and
 * actually applying them consecutively.
 */
Kernel *kernel_new_compose(Kernel **pthis, Kernel *k1, Kernel *k2);

/* Create an identity kernel, for testing purposes.
 */
Kernel *kernel_new_identity(Kernel **pthis, ulong n);

void kernel_delete(Kernel **pthis);
void kernel_print(Kernel *this, FILE *f, byte n);

/* Apply a kernel. The callback function read(in, x) should return the input
 * value at index 'in', and the function write(out, v, x) should write value
 * 'v' to output element 'out'. See parallel.h for num_threads: 1 means
 * single-threaded, and 0 means use the default number of threads.
 */
void kernel_apply(
    Kernel *this,
    double (*read)(ulong in, void *x),
    void (*write)(ulong out, double v, void *x),
    void *x,
    ulong num_threads);

/* Test whether two kernels are equal, where corresponding elements are
 * considered equal if they are within the given tolerance.
 */
bool kernel_equals(Kernel *this, Kernel *other, double tolerance);

/* Transform from an input index to an output index for resizing by a factor of
 * k, with the assumption that the left edge of the first output tile aligns
 * with the left edge of the first input tile, as is appropriate for image
 * data. If we measure the input space by the real value x, where x = 0 is the
 * left edge of the first input tile and x = 1 is the right edge of it, and
 * likewise y for the output space, where y = 0 and y = 1 are the left and
 * right edges of the first output tile, then clearly for these continuous
 * variables y = k x. If the zero-offset integer indexes 'in' and 'out' are
 * then assumed to refer to the *center* positions of their respective tiles,
 * then clearly x(in) = in + 1/2 and y(out) = out + 1/2, so out + 1/2 = k(in +
 * 1/2).
 */
double kernel_resize_out_from_in(double in, double k);
double kernel_resize_in_from_out(double out, double k);

/* Get a matrix element from the kernel. Should not need to be used, since
 * kernel_apply() or kernel_apply_multiple() are the efficient ways to apply
 * the kernel, but this is here for unit testing purposes. (This function has
 * to do a binary search on the nonzero matrix elements.)
 */
double kernel_get(Kernel *this, ulong out, ulong in);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* _H */
