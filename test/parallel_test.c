#include "parallel_test.h"

#define M_ 16
#define N_ 100
#define P_ 8
#define T_ 12

static void add_(ulong **pthis, ulong *a, ulong *b, void *x) {
  *(ulong *)newo(pthis) = *a + *b;
}

static void delete_(ulong **pthis) {
  deleteo(pthis);
}

START

TEST(Parallel, Ranges) {
  ParallelRange *ranges = null;

  parallel_compute_ranges(&ranges, -817, 123, 7);
  EXPECT_LEQ(-817, ranges[0].start);
  EXPECT_LEQ(-683, ranges[0].end);
  EXPECT_LEQ(-683, ranges[1].start);
  EXPECT_LEQ(-549, ranges[1].end);
  EXPECT_LEQ(-549, ranges[2].start);
  EXPECT_LEQ(-415, ranges[2].end);
  EXPECT_LEQ(-415, ranges[3].start);
  EXPECT_LEQ(-280, ranges[3].end);
  EXPECT_LEQ(-280, ranges[4].start);
  EXPECT_LEQ(-146, ranges[4].end);
  EXPECT_LEQ(-146, ranges[5].start);
  EXPECT_LEQ(-12, ranges[5].end);
  EXPECT_LEQ(-12, ranges[6].start);
  EXPECT_LEQ(123, ranges[6].end);
  deletev(&ranges);
}

TEST(Parallel, Speed) {
  ulong i;

  /* Use this to test setup/teardown speed. Seems around 25 us / thread total.
   */
  for (i = 0; i < N_; i++) {
    parallel_memcpy(null, null, 0, 16);
  }
}

TEST(Parallel, Memcpy) {
  byte n_width;
  byte num_threads_width;
  byte *dst = null;
  byte *src = null;
  ulong i;
  ulong n;
  ulong num_threads;
  ulong u;
  Random *random = null;

  /* Test normal operation.
   */
  random_new(&random);
  for (i = 0; i < N_; i++) {
    n_width = random_modulo(random, M_);
    n = random_modulo(random, 1 << n_width);

    /* Include 0 threads, which will pull the default.
    */
    num_threads_width = random_modulo(random, P_);
    num_threads = random_modulo(random, 1 << num_threads_width);
    new(&src, n);
    new(&dst, n);
    for (u = 0; u < n; u++) {
      src[u] = random_modulo(random, 256);
    }
    parallel_memcpy(dst, src, n, num_threads);
    for (u = 0; u < n; u++) {
      EXPECT_ULEQ(src[u], dst[u]);
    }
    deletev(&dst);
    deletev(&src);
  }

  /* Simulate resource constraints, since some operating systems can handle a
   * huge number of thread creations (which makes them impossible to test using
   * actual system calls), and some can't (so we need to test that the library
   * handles the situation correctly).
   */
  parallel_simulate_resource_constraints(17);
  n = 34567;
  num_threads = 1234;
  new(&src, n);
  new(&dst, n);
  for (u = 0; u < n; u++) {
    src[u] = random_modulo(random, 256);
  }
  parallel_memcpy(dst, src, n, num_threads);
  for (u = 0; u < n; u++) {
    EXPECT_ULEQ(src[u], dst[u]);
  }
  deletev(&dst);
  deletev(&src);
  parallel_simulate_resource_constraints(0);

  random_delete(&random);
}

TEST(Parallel, Merge) {
  ulong n;
  ulong num_threads;
  ulong u;
  ulong *sum = null;
  ulong **inputs = null;

  for (num_threads = 1; num_threads <= 50; num_threads++) {
    for (n = 2; n < T_; n++) {
      new(&inputs, n);
      for (u = 0; u < n; u++) {
        *(ulong *)newo(&inputs[u]) = u + 1;
      }
      parallel_merge(
          &sum,
          n,
          inputs,
          add_,
          delete_,
          null,
          true,
          num_threads);
      EXPECT_ULEQ(n * (n + 1) / 2, *sum);
      for (u = 0; u < n; u++) {
        EXPECT_NULL(inputs[u]);
      }
      deleteo(&sum);
      for (u = 0; u < n; u++) {
        *(ulong *)newo(&inputs[u]) = u + 1;
      }
      parallel_merge(
          &sum,
          n,
          inputs,
          add_,
          delete_,
          null,
          false,
          num_threads);
      EXPECT_ULEQ(n * (n + 1) / 2, *sum);
      for (u = 0; u < n; u++) {
        EXPECT_NONNULL(inputs[u]);
      }
      delete(&inputs, n, delete_);
      deleteo(&sum);
    }
  }
}

FINISH
