#include "qsortx_test.h"

#define N_ 10000
#define M_ 10
#define P_ 9

static ulong size_;

static int std_cmp_(const void *a, const void *b) {
  byte ba;
  byte bb;
  byte *as;
  byte *bs;
  ulong u;

  as = (byte *)a;
  bs = (byte *)b;

  for (u = 0; u < size_; u++) {
    ba = as[u];
    bb = bs[u];
    if (ba < bb) {
      return -1;
    }
    if (ba > bb) {
      return +1;
    }
  }
  return 0;
}

static int imp_cmp_(void *a, void *b, void *x) {
  byte ba;
  byte bb;
  byte *as;
  byte *bs;
  ulong size;
  ulong u;

  as = (byte *)a;
  bs = (byte *)b;
  size = *(ulong *)x;

  for (u = 0; u < size; u++) {
    ba = as[u];
    bb = bs[u];
    if (ba < bb) {
      return -1;
    }
    if (ba > bb) {
      return +1;
    }
  }
  return 0;
}

static void print_(
    ulong n,
    ulong size,
    byte *std,
    byte *imp,
    bool q,
    ulong qu,
    ulong bu,
    ulong bv) {
  ulong u;
  ulong v;

  if (q) {
    fprintf(stderr,
        "qsortx failure at element %lu, byte %lu\n",
        qu / size,
        qu % size);
  } else {
    fprintf(stderr,
        "bsearchx failure at element %lu, byte %lu\n",
        bu, bv);
  }
  for (u = 0; u < n; u++) {
    fprintf(stderr, "[std %lu]: ", u);
    for (v = 0; v < size; v++) {
      fprintf(stderr, "%02x", std[u * size + v]);
    }
    fprintf(stderr, "\n");
    fprintf(stderr, "[imp %lu]: ", u);
    for (v = 0; v < size; v++) {
      fprintf(stderr, "%02x", imp[u * size + v]);
    }
    fprintf(stderr, "\n");
  }
}

static int test_cmp_(void *a, void *b, void *x) {
  int as;
  int bs;
  int ia;
  int ib;
  short s;

  ia = *(int *)a;
  ib = *(int *)b;
  s = *(short *)x;

  as = ia * s;
  bs = ib * s;

  return as < bs ? -1 : as > bs ? +1 : 0;
}

static int c(void *a, void *b, void *x) {
  uint ua;
  uint ub;

  ua = *(uint *)a;
  ub = *(uint *)b;
  return ua < ub ? -1 : ua > ub ? +1 : 0;
}

START

TEST(QsortxBsearchx, PassThrough) {
  short x;
  int key;
  int *r;
  int a[] = {24, 9, 1966};

  x = 7;
  qsortx(a, 3, test_cmp_, &x);
  key = 1966;
  r = (int *)bsearchx(&key, a, 3, test_cmp_, &x);
  EXPECT_PEQ(a + 2, r);
  x = -3;
  qsortx(a, 3, test_cmp_, &x);
  r = (int *)bsearchx(&key, a, 3, test_cmp_, &x);
  EXPECT_PEQ(a, r);
}

TEST(QsortxBsearchx, Comprehensive) {
  byte n_width;
  byte size_width;
  byte *std = null;
  byte *imp = null;
  ulong i;
  ulong n;
  ulong num_bytes;
  ulong size;
  ulong u;
  ulong v;
  Random *random = null;

  /* Test against stdlib version with global variable instead of pass-through.
   */
  random_new(&random);

  for (i = 0; i < N_; i++) {
    n_width = random_modulo(random, M_);
    n = random_modulo(random, 1 << n_width);
    size_width = random_modulo(random, P_);
    size = size_ = 1 + random_modulo(random, 1 << size_width);
    num_bytes = n * size_;
    new(&std, num_bytes);
    new(&imp, num_bytes);
    for (u = 0; u < num_bytes; u++) {
      std[u] = imp[u] = random_modulo(random, 256);
    }
    qsort(std, n, size, std_cmp_);
    qsortxn(imp, n, size, imp_cmp_, &size);
    EXPECT_ULEQ(size, size_);
    for (u = 0; u < num_bytes; u++) {
      if (std[u] != imp[u]) {
        print_(n, size, std, imp, true, u, 0, 0);
      }
      EXPECT_ULEQ(std[u], imp[u]);
    }
    for (u = 0; u < n; u++) {
      byte *p = (byte *)bsearchxn(
          &std[u * size],
          imp,
          n,
          size_,
          imp_cmp_,
          &size);
      EXPECT_ULEQ(size, size_);
      for (v = 0; v < size_; v++) {
        if (std[u * size + v] !=  p[v]) {
          print_(n, size, std, imp, false, 0, u, v);
        }
        EXPECT_ULEQ(std[u * size + v], p[v]);
      }
    }
    EXPECT_ULEQ(size, size_);
    deletev(&imp);
    deletev(&std);
  }
  random_delete(&random);
}

TEST(Bsearchx, FindIndex) {
  uint d[] = {0x07071968, 0x23031998, 0x24091966, 0x28032001};
  ulong i;
  ulong n;
  uint k;

  n = sizeof(d) / sizeof(*d);
  k = 0x07071968;
  EXPECT_ULEQ(0, bsearchx_find_index(&k, d, n, c, null));
  k = 0x23031998;
  EXPECT_ULEQ(1, bsearchx_find_index(&k, d, n, c, null));
  k = 0x24091966;
  EXPECT_ULEQ(2, bsearchx_find_index(&k, d, n, c, null));
  k = 0x28032001;
  EXPECT_ULEQ(3, bsearchx_find_index(&k, d, n, c, null));
  k = 0;
  k = 0x07071968;
  EXPECT_TRUE(bsearchx_try_find_index(&k, d, n, c, null, &i));
  EXPECT_ULEQ(0, i);
  k = 0x23031998;
  EXPECT_TRUE(bsearchx_try_find_index(&k, d, n, c, null, &i));
  EXPECT_ULEQ(1, i);
  k = 0x24091966;
  EXPECT_TRUE(bsearchx_try_find_index(&k, d, n, c, null, &i));
  EXPECT_ULEQ(2, i);
  k = 0x28032001;
  EXPECT_TRUE(bsearchx_try_find_index(&k, d, n, c, null, &i));
  EXPECT_ULEQ(3, i);
  k = 0;
  EXPECT_FALSE(bsearchx_try_find_index(&k, d, n, c, null, &i));
}

FINISH
