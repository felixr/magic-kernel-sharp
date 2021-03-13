/* Simple parallel functions using pthreads.
 */
#ifndef PARALLEL_H
#define PARALLEL_H

#ifdef __cplusplus
extern "C" {
#endif

#include "processors.h"

/* The function parallel_compute_ranges() computes an array of these
 * structures, partitioning a given range of integers [start, end] into ranges
 * that are as equal in size as possible. This array of ranges is useful for
 * many parallel compute tasks.
 */
typedef struct {
  long start;
  long end;
} ParallelRange;

/* Run a function with the specified number of threads. Each thread t will get
 * its corresponding object pointer xs[t]. The function will attempt to start
 * all threads, no matter how many are specified, and relies on the operating
 * system to reject the creation of new threads when there are insufficient
 * resources; these threads are queued up to be started, and the function will
 * wait a second before attempting to create these threads.
 */
void parallel_do(void *(*run)(void *x), ulong num_threads, void **xs);

/* Do a memcpy using parallel threads. Breaks up the bytes into ranges across
 * the specified number of threads.
 */
void parallel_memcpy(void *dst, void *src, ulong num_bytes, ulong num_threads);

/* Return the default number of threads, which is one less than the number of
 * processors, if there is more than one processor, or else one.
 */
ulong parallel_get_default_num_threads(void);

/* Convenience function for computing the number of threads required
 */
ulong parallel_get_num_threads(ulong min_count_per_thread, ulong count);

/* Partition a range into essentially equal subranges, as described above.
 */
ParallelRange *parallel_compute_ranges(
    ParallelRange **pranges,
    long start,
    long end,
    ulong num_threads);

/* Parallel implementation of merge.
 */
#define parallel_merge(pt, n, os, m, d, x, di, nt) \
  parallel_mergeI( \
      (void **)(pt), \
      (n), \
      (void **)(os), \
      (void (*)(void **, void *, void *, void *))(m), \
      (void (*)(void **))(d), \
      (void *)(x), \
      (di), \
      (nt))
void parallel_mergeI(
    void **pthis,
    ulong num_others,
    void **others,
    void (*merge)(void **pthis, void *a, void *b, void *x),
    void (*destructor)(void **pthis),
    void *x,
    bool delete_inputs,
    ulong num_threads);

/* Simulate thread resource constraints, for testing: one out of every
 * denominator thread creation calls will be randomly simulated to fail.
 */
void parallel_simulate_resource_constraints(ulong denominator);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* _H */
