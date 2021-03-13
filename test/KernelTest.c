#include "KernelTest.h"
#include <math.h>

typedef struct {
  double *in;
  double *out;
} Info_;

static KernelPossible possible_1_(ulong out, void *x);
static double weight_1_(ulong out, long in, void *x);
static KernelPossible possible_2_(ulong out, void *x);
static double weight_2_(ulong out, long in, void *x);
static KernelPossible possible_3_(ulong out, void *x);
static double weight_3_(ulong out, long in, void *x);

static double read_(ulong in, void *x);
static void write_(ulong out, double v, void *x);

static KernelPossible possible_tile_(ulong out, void *x);
static double weight_tile_(ulong out, long in, void *x);

static KernelPossible possible_bloom_(ulong out, void *x);
static double weight_bloom_(ulong out, long in, void *x);

START

TEST(Kernel, Construct) {
  double scale;
  Kernel *k = null;

  /* Test small numbers of n_in and n_out, checking members, with what is just
   * an identity spec (above).
   */
  kernel_new(&k, 1, 1, false, possible_1_, weight_1_, null);
  EXPECT_ULEQ(1, k->n_in);
  EXPECT_ULEQ(1, k->n_out);
  EXPECT_FALSE(k->periodic);
  EXPECT_DEQ(1, kernel_get(k, 0, 0));
  kernel_delete(&k);

  kernel_new(&k, 1, 1, true, possible_1_, weight_1_, null);
  EXPECT_ULEQ(1, k->n_in);
  EXPECT_ULEQ(1, k->n_out);
  EXPECT_TRUE(k->periodic);
  EXPECT_DEQ(1, kernel_get(k, 0, 0));
  kernel_delete(&k);

  kernel_new(&k, 2, 1, false, possible_1_, weight_1_, null);
  EXPECT_ULEQ(2, k->n_in);
  EXPECT_ULEQ(1, k->n_out);
  EXPECT_DEQ(1, kernel_get(k, 0, 0));
  kernel_delete(&k);

  kernel_new(&k, 1, 2, false, possible_1_, weight_1_, null);
  EXPECT_ULEQ(1, k->n_in);
  EXPECT_ULEQ(2, k->n_out);
  EXPECT_DEQ(1, kernel_get(k, 0, 0));
  EXPECT_DEQ(1, kernel_get(k, 1, 0));
  kernel_delete(&k);

  kernel_new(&k, 2, 2, false, possible_1_, weight_1_, null);
  EXPECT_DEQ(1, kernel_get(k, 0, 0));
  EXPECT_DEQ(0, kernel_get(k, 0, 1));
  EXPECT_DEQ(1, kernel_get(k, 1, 1));
  EXPECT_DEQ(0, kernel_get(k, 1, 0));
  kernel_delete(&k);

  kernel_new(&k, 3, 3, false, possible_1_, weight_1_, null);
  EXPECT_DEQ(1, kernel_get(k, 0, 0));
  EXPECT_DEQ(1, kernel_get(k, 1, 1));
  EXPECT_DEQ(1, kernel_get(k, 2, 2));
  kernel_delete(&k);

  /* Now test a kernel that has two taps, and include scaling information in
   * the passthrough x. Start with same-size and the input having enough
   * elements to avoid boundary conditions.
   */
  scale = 1;
  kernel_new(&k, 3, 2, false, possible_2_, weight_2_, &scale);
  EXPECT_DEQ(0.25, kernel_get(k, 0, 0));
  EXPECT_DEQ(0.75, kernel_get(k, 0, 1));
  EXPECT_DEQ(0.25, kernel_get(k, 1, 1));
  EXPECT_DEQ(0.75, kernel_get(k, 1, 2));
  kernel_delete(&k);

  /* Now invoke boundary conditions.
   */
  kernel_new(&k, 2, 2, false, possible_2_, weight_2_, &scale);
  EXPECT_DEQ(0.25, kernel_get(k, 0, 0));
  EXPECT_DEQ(0.75, kernel_get(k, 0, 1));
  EXPECT_DEQ(1, kernel_get(k, 1, 1));
  kernel_delete(&k);

  kernel_new(&k, 2, 2, true, possible_2_, weight_2_, &scale);
  EXPECT_DEQ(0.25, kernel_get(k, 0, 0));
  EXPECT_DEQ(0.75, kernel_get(k, 0, 1));
  EXPECT_DEQ(0.75, kernel_get(k, 1, 0));
  EXPECT_DEQ(0.25, kernel_get(k, 1, 1));
  kernel_delete(&k);

  kernel_new(&k, 3, 3, false, possible_2_, weight_2_, &scale);
  EXPECT_DEQ(0.25, kernel_get(k, 0, 0));
  EXPECT_DEQ(0.75, kernel_get(k, 0, 1));
  EXPECT_DEQ(0.25, kernel_get(k, 1, 1));
  EXPECT_DEQ(0.75, kernel_get(k, 1, 2));
  EXPECT_DEQ(1, kernel_get(k, 2, 2));
  kernel_delete(&k);

  kernel_new(&k, 3, 3, true, possible_2_, weight_2_, &scale);
  EXPECT_DEQ(0.25, kernel_get(k, 0, 0));
  EXPECT_DEQ(0.75, kernel_get(k, 0, 1));
  EXPECT_DEQ(0.25, kernel_get(k, 1, 1));
  EXPECT_DEQ(0.75, kernel_get(k, 1, 2));
  EXPECT_DEQ(0.75, kernel_get(k, 2, 0));
  EXPECT_DEQ(0.25, kernel_get(k, 2, 2));
  kernel_delete(&k);

  /* Now change the scale (but with a non-interpolating kernel).
   */
  scale = 2;
  kernel_new(&k, 3, 4, false, possible_2_, weight_2_, &scale);
  EXPECT_DEQ(0.25, kernel_get(k, 0, 0));
  EXPECT_DEQ(0.75, kernel_get(k, 0, 1));
  EXPECT_DEQ(0.25, kernel_get(k, 1, 0));
  EXPECT_DEQ(0.75, kernel_get(k, 1, 1));
  EXPECT_DEQ(0.25, kernel_get(k, 2, 1));
  EXPECT_DEQ(0.75, kernel_get(k, 2, 2));
  EXPECT_DEQ(0.25, kernel_get(k, 3, 1));
  EXPECT_DEQ(0.75, kernel_get(k, 3, 2));
  kernel_delete(&k);

  scale = 0.5;
  kernel_new(&k, 4, 2, false, possible_2_, weight_2_, &scale);
  EXPECT_DEQ(0.25, kernel_get(k, 0, 0));
  EXPECT_DEQ(0.75, kernel_get(k, 0, 1));
  EXPECT_DEQ(0.25, kernel_get(k, 1, 2));
  EXPECT_DEQ(0.75, kernel_get(k, 1, 3));
  kernel_delete(&k);

  /* Now test zeros in the actual weights.
   */
  kernel_new(&k, 4, 3, false, possible_3_, weight_3_, null);
  EXPECT_ULEQ(5, k->num_elements);
  EXPECT_DEQ(0.25, kernel_get(k, 0, 1));
  EXPECT_DEQ(0.75, kernel_get(k, 0, 3));
  EXPECT_DEQ(0.25, kernel_get(k, 1, 2));
  EXPECT_DEQ(0.75, kernel_get(k, 1, 3));
  EXPECT_DEQ(1, kernel_get(k, 2, 3));
  kernel_delete(&k);
}

TEST(Kernel, Compose) {
  Kernel *k1 = null;
  Kernel *k2 = null;
  Kernel *k = null;

  /* Let's use Charles Bloom's 2011 decomposition of the original "magic"
   * kernel as our test case!
   */
  kernel_new(&k1, 3, 6, false, possible_tile_, weight_tile_, null);
  kernel_new(&k2, 6, 6, false, possible_bloom_, weight_bloom_, null);
  kernel_new_compose(&k, k1, k2);
  kernel_delete(&k2);
  kernel_delete(&k1);

  /* Members.
   */
  EXPECT_ULEQ(3, k->n_in);
  EXPECT_ULEQ(6, k->n_out);
  EXPECT_FALSE(k->periodic);

  /* Fully inside.
   */
  EXPECT_DEQ(0.75, kernel_get(k, 1, 0));
  EXPECT_DEQ(0.25, kernel_get(k, 1, 1));

  EXPECT_DEQ(0.25, kernel_get(k, 2, 0));
  EXPECT_DEQ(0.75, kernel_get(k, 2, 1));

  EXPECT_DEQ(0.75, kernel_get(k, 3, 1));
  EXPECT_DEQ(0.25, kernel_get(k, 3, 2));

  EXPECT_DEQ(0.25, kernel_get(k, 4, 1));
  EXPECT_DEQ(0.75, kernel_get(k, 4, 2));

  /* Edges.
   */
  EXPECT_DEQ(1, kernel_get(k, 0, 0));

  EXPECT_DEQ(1, kernel_get(k, 5, 2));

  kernel_delete(&k);
}

TEST(Kernel, Identity) {
  Kernel *k = null;

  kernel_new_identity(&k, 1);
  EXPECT_DEQ(1, kernel_get(k, 0, 0));
  kernel_delete(&k);

  kernel_new_identity(&k, 2);
  EXPECT_DEQ(1, kernel_get(k, 0, 0));
  EXPECT_DEQ(0, kernel_get(k, 0, 1));
  EXPECT_DEQ(0, kernel_get(k, 1, 0));
  EXPECT_DEQ(1, kernel_get(k, 1, 1));
  kernel_delete(&k);

  kernel_new_identity(&k, 3);
  EXPECT_DEQ(1, kernel_get(k, 0, 0));
  EXPECT_DEQ(0, kernel_get(k, 0, 1));
  EXPECT_DEQ(0, kernel_get(k, 0, 2));
  EXPECT_DEQ(0, kernel_get(k, 1, 0));
  EXPECT_DEQ(1, kernel_get(k, 1, 1));
  EXPECT_DEQ(0, kernel_get(k, 1, 2));
  EXPECT_DEQ(0, kernel_get(k, 2, 0));
  EXPECT_DEQ(0, kernel_get(k, 2, 1));
  EXPECT_DEQ(1, kernel_get(k, 2, 2));
  kernel_delete(&k);
}

TEST(Kernel, Apply) {
  Info_ info;
  Kernel *k = null;

  info.in = null;
  info.out = null;

  kernel_new(&k, 4, 3, false, possible_3_, weight_3_, null);
  new(&info.in, 4);
  new(&info.out, 3);
  info.in[0] = 1;
  info.in[1] = 2;
  info.in[2] = 3;
  info.in[3] = 4;
  kernel_apply(k, read_, write_, &info, 1);
  EXPECT_DEQ(3.5, info.out[0]);
  EXPECT_DEQ(3.75, info.out[1]);
  EXPECT_DEQ(4, info.out[2]);
  deletev(&info.out);
  deletev(&info.in);
  kernel_delete(&k);
}

TEST(Kernel, Equals) {
  Kernel *k1 = null;
  Kernel *k2 = null;
  Kernel *k = null;
  Kernel *k_alt = null;

  /* Use the Bloom decomposition again, as it has useful non-trivial structure.
   */
  kernel_new(&k1, 3, 6, false, possible_tile_, weight_tile_, null);
  kernel_new(&k2, 6, 6, false, possible_bloom_, weight_bloom_, null);
  kernel_new_compose(&k, k1, k2);

  /* It should equal itself.
   */
  EXPECT_TRUE(kernel_equals(k, k, 0));

  /* Now create it again, but alter two elements. NOTE: This depends on the
   * specific internal implementation of Kernel, so would have to be modified
   * if this implementation were changed.
   */
  kernel_new_compose(&k_alt, k1, k2);
  kernel_delete(&k2);
  kernel_delete(&k1);
  k_alt->weights[3][0] += 0.1;
  k_alt->weights[3][1] -= 0.1;

  /* They should not be equal.
   */
  EXPECT_FALSE(kernel_equals(k, k_alt, 0));

  kernel_delete(&k_alt);
  kernel_delete(&k);
}

TEST(Kernel, ResizeCoordinates) {
  EXPECT_DEQ(0.5, kernel_resize_out_from_in(0, 2));
  EXPECT_DEQ(2.5, kernel_resize_out_from_in(1, 2));
  EXPECT_DEQ(4.5, kernel_resize_out_from_in(2, 2));

  EXPECT_DEQ(-0.25, kernel_resize_in_from_out(0, 2));
  EXPECT_DEQ(0.25, kernel_resize_in_from_out(1, 2));
  EXPECT_DEQ(0.75, kernel_resize_in_from_out(2, 2));
  EXPECT_DEQ(1.25, kernel_resize_in_from_out(3, 2));
  EXPECT_DEQ(1.75, kernel_resize_in_from_out(4, 2));

  EXPECT_DEQ(0, kernel_resize_in_from_out(kernel_resize_out_from_in(0, 2), 2));
  EXPECT_DEQ(1, kernel_resize_in_from_out(kernel_resize_out_from_in(1, 2), 2));
  EXPECT_DEQ(2, kernel_resize_in_from_out(kernel_resize_out_from_in(2, 2), 2));
  EXPECT_DEQ(3, kernel_resize_in_from_out(kernel_resize_out_from_in(3, 2), 2));

  EXPECT_DEQ(0, kernel_resize_out_from_in(kernel_resize_in_from_out(0, 2), 2));
  EXPECT_DEQ(1, kernel_resize_out_from_in(kernel_resize_in_from_out(1, 2), 2));
  EXPECT_DEQ(2, kernel_resize_out_from_in(kernel_resize_in_from_out(2, 2), 2));
  EXPECT_DEQ(3, kernel_resize_out_from_in(kernel_resize_in_from_out(3, 2), 2));
}

FINISH

static KernelPossible possible_1_(ulong out, void *x) {
  KernelPossible possible;

  possible.first = (long)out;
  possible.last = (long)out;

  return possible;
}

static double weight_1_(ulong out, long in, void *x) {
  return 3.4;
}

static KernelPossible possible_2_(ulong out, void *x) {
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

static double weight_2_(ulong out, long in, void *x) {
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
      return 3;
    case 2:
      return 0;
    default:
      throw("impossible");
      return 0;
  }
}

static KernelPossible possible_3_(ulong out, void *x) {
  KernelPossible possible;

  possible.first = (long)out;
  possible.last = (long)out + 4;

  return possible;
}

static double weight_3_(ulong out, long in, void *x) {
  switch (in - (long)out) {
    case 0:
      return 0;
    case 1:
      return 1;
    case 2:
      return 0;
    case 3:
      return 3;
    case 4:
      return 0;
    default:
      throw("impossible");
      return 0;
  }
}

static double read_(ulong in, void *x) {
  Info_ *info;

  info = x;

  return info->in[in];
}

static void write_(ulong out, double v, void *x) {
  Info_ *info;

  info = x;

  info->out[out] = v;
}

static KernelPossible possible_tile_(ulong out, void *x) {
  KernelPossible possible;

  possible.first = (long)out / 2;
  possible.last = possible.first;

  return possible;
}

static double weight_tile_(ulong out, long in, void *x) {
  return 1;
}

static KernelPossible possible_bloom_(ulong out, void *x) {
  KernelPossible possible;

  possible.first = (long)out - 1;
  possible.last = (long)out + 1;

  return possible;
}

static double weight_bloom_(ulong out, long in, void *x) {
  if (out == in) {
    return 2;
  }

  if (out == in - 1 || out == in + 1) {
    return 1;
  }

  throw("impossible");
  return 0;
}
