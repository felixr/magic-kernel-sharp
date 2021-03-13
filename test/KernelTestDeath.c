#include "KernelTestDeath.h"

static KernelPossible possible_(ulong out, void *x) {
  KernelPossible possible;

  possible.first = (long)out;
  possible.last = (long)out;

  return possible;
}

static KernelPossible possible_bad_(ulong out, void *x) {
  KernelPossible possible;

  possible.first = (long)out;
  possible.last = (long)out - 1;

  return possible;
}

static double weight_(ulong out, long in, void *x) {
  return 3.4;
}

static double weight_bad_1_(ulong out, long in, void *x) {
  return 0;
}

static double weight_bad_2_(ulong out, long in, void *x) {
  return -3.4;
}

START_DEATH

TEST(Kernel, Constructor) {
  Kernel *k = null;

  EXPECT_DEATH(
      kernel_new(&k, 0, 1, false, possible_, weight_, null),
      "n_in cannot be zero");
  kernel_delete(&k);
  EXPECT_DEATH(
      kernel_new(&k, 1, 0, false, possible_, weight_, null),
      "n_out cannot be zero");
  kernel_delete(&k);
  EXPECT_DEATH(
      kernel_new(&k, 1, 1, false, null, weight_, null),
      "'possible' function pointer cannot be null");
  kernel_delete(&k);
  EXPECT_DEATH(
      kernel_new(&k, 1, 1, false, possible_, null, null),
      "'weight' function pointer cannot be null");
  kernel_delete(&k);
  EXPECT_DEATH(
      kernel_new(&k, 1, 1, false, possible_bad_, weight_, null),
      "last (-1) cannot be less than first (0) for out = 0");
  kernel_delete(&k);
  EXPECT_DEATH(
      kernel_new(&k, 1, 1, false, possible_, weight_bad_1_, null),
      "Did not have any nonzero weights for out = 0");
  kernel_delete(&k);
  EXPECT_DEATH(
      kernel_new(&k, 1, 1, false, possible_, weight_bad_2_, null),
      "Sum of weights (-3.4) for out = 0 was negative, which is not supported");
  kernel_delete(&k);
  EXPECT_DEATH(kernel_new_identity(&k, 0), "n_in cannot be zero");
  kernel_delete(&k);
}

TEST(Kernel, Compose) {
  Kernel *k1 = null;
  Kernel *k2 = null;
  Kernel *k3 = null;

  kernel_new_identity(&k1, 1);
  kernel_new_identity(&k2, 2);
  EXPECT_DEATH(
      kernel_new_compose(&k3, k1, k2),
      "n_in (2) for the second kernel must equal n_out (1) "
      "of the first kernel");
  kernel_delete(&k3);
  kernel_delete(&k2);
  kernel_new_identity(&k2, 1);
  k2->periodic = true;
  EXPECT_DEATH(
      kernel_new_compose(&k3, k1, k2),
      "Periodic flag for first kernel (false) must match that of "
      "the second (true)");
  kernel_delete(&k3);
  kernel_delete(&k2);
  kernel_delete(&k1);
}

TEST(Kernel, Accessor) {
  Kernel *k = null;

  kernel_new_identity(&k, 1);
  EXPECT_DEATH(kernel_get(k, 0, 1), "in (1) must be less than n_in (1)");
  EXPECT_DEATH(kernel_get(k, 1, 0), "out (1) must be less than n_out (1)");
  EXPECT_DEATH(kernel_get(k, 1, 1), "in (1) must be less than n_in (1)");
  EXPECT_DEATH(kernel_get(k, 0, 2), "in (2) must be less than n_in (1)");
  EXPECT_DEATH(kernel_get(k, 2, 0), "out (2) must be less than n_out (1)");
  EXPECT_DEATH(kernel_get(k, 2, 2), "in (2) must be less than n_in (1)");
  kernel_delete(&k);

  kernel_new_identity(&k, 2);
  EXPECT_DEATH(kernel_get(k, 0, 2), "in (2) must be less than n_in (2)");
  EXPECT_DEATH(kernel_get(k, 2, 0), "out (2) must be less than n_out (2)");
  EXPECT_DEATH(kernel_get(k, 2, 2), "in (2) must be less than n_in (2)");
  EXPECT_DEATH(kernel_get(k, 0, 3), "in (3) must be less than n_in (2)");
  EXPECT_DEATH(kernel_get(k, 3, 0), "out (3) must be less than n_out (2)");
  EXPECT_DEATH(kernel_get(k, 3, 3), "in (3) must be less than n_in (2)");
  kernel_delete(&k);
}

FINISH_DEATH
