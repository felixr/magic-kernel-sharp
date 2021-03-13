#include "magic_test_death.h"

START_DEATH

TEST(Magic, MagicKernel) {
  EXPECT_DEATH(magic_kernel(0, 0), "a (0) must be in [1, 6]");
  EXPECT_DEATH(magic_kernel(7, 0), "a (7) must be in [1, 6]");
}

TEST(Magic, LanczosKernel) {
  EXPECT_DEATH(lanczos_kernel(0, 0), "a cannot be zero");
}




#ifdef DOG

TEST(Magic, MagicKernelSharpDownsizer) {
  /*
  Kernel *k = null;
  */

  /*
  EXPECT_DEATH(
      kernel_new_resize(
        &k,
        22,
        10,
        0.5,
        0,
        false,
        false,
        MAGIC_RESIZER_MAGIC_KERNEL_SHARP),
      "n_out (10) too small for n_in (22) and k (1/2): must be exactly 11");
      */
}

TEST(Magic, MagicKernelSharpUpsizer) {






}

TEST(Magic, ContinuumMagicKernelDownsizer) {







}

TEST(Magic, ContinuumMagicKernelUpsizer) {







}

TEST(Magic, Sharpen) {
  /*)
  Kernel *k = null;

  EXPECT_DEATH(
      kernel_new_sharpen(&k, 10, 0, false),
      "Sharpening (0) must be positive");
  EXPECT_DEATH(
      kernel_new_sharpen(&k, 10, -0.1, false),
      "Sharpening (-0.1) must be positive");
      */
}

TEST(Magic, Blur) {
  /*
  Kernel *k = null;

  EXPECT_DEATH(
      kernel_new_blur(&k, 10, 0, false),
      "Blur (0) must be positive");
  EXPECT_DEATH(
      kernel_new_blur(&k, 10, -0.1, false),
      "Blur (-0.1) must be positive");
      */
}

TEST(Magic, OriginalMagicKernelDoubler) {






}

TEST(Magic, OriginalMagicKernelHalver) {







}

TEST(Magic, NearestNeighborUpsizer) {
  /*
  Kernel *k = null;

  EXPECT_DEATH(
      kernel_new_resize(
        &k,
        0,
        1,
        2,
        0,
        false,
        true,
        MAGIC_RESIZER_NEAREST_NEIGHBOR),
      "n_in cannot be zero");
  EXPECT_DEATH(
      kernel_new_resize(
        &k,
        1,
        0,
        2,
        0,
        false,
        true,
        MAGIC_RESIZER_NEAREST_NEIGHBOR),
      "n_out cannot be zero");
  EXPECT_DEATH(
      kernel_new_resize(
        &k,
        1,
        1,
        0,
        0,
        false,
        true,
        MAGIC_RESIZER_NEAREST_NEIGHBOR),
      "k (0) must be positive");
  EXPECT_DEATH(
      kernel_new_resize(
        &k,
        1,
        1,
        1,
        0,
        false,
        true,
        MAGIC_RESIZER_NEAREST_NEIGHBOR),
      "k cannot be 1");

  EXPECT_DEATH(
      kernel_new_resize(
        &k,
        1,
        4,
        2,
        0,
        false,
        true,
        MAGIC_RESIZER_NEAREST_NEIGHBOR),
      "n_in (1) too small for n_out (4) and k (2): must be exactly 2");
  EXPECT_DEATH(
      kernel_new_resize(
        &k,
        2,
        1,
        2,
        0,
        false,
        true,
        MAGIC_RESIZER_NEAREST_NEIGHBOR),
      "n_in (2) too large for n_out (1) and k (2): must be no greater than 1");
  EXPECT_DEATH(
      kernel_new_resize(
        &k,
        2,
        2,
        2,
        0,
        false,
        true,
        MAGIC_RESIZER_NEAREST_NEIGHBOR),
      "n_in (2) too large for n_out (2) and k (2): must be exactly 1");
  EXPECT_DEATH(
      kernel_new_resize(
        &k,
        2,
        6,
        2,
        0,
        false,
        true,
        MAGIC_RESIZER_NEAREST_NEIGHBOR),
      "n_in (2) too small for n_out (6) and k (2): must be exactly 3");
  EXPECT_DEATH(
      kernel_new_resize(
        &k,
        3,
        4,
        2,
        0,
        false,
        true,
        MAGIC_RESIZER_NEAREST_NEIGHBOR),
      "n_in (3) too large for n_out (4) and k (2): must be exactly 2");
  EXPECT_DEATH(
      kernel_new_resize(
        &k,
        3,
        8,
        2,
        0,
        false,
        true,
        MAGIC_RESIZER_NEAREST_NEIGHBOR),
      "n_in (3) too small for n_out (8) and k (2): must be exactly 4");

  EXPECT_DEATH(
      kernel_new_resize(
        &k,
        1,
        6,
        3,
        0,
        false,
        true,
        MAGIC_RESIZER_NEAREST_NEIGHBOR),
      "n_in (1) too small for n_out (6) and k (3): must be exactly 2");
  EXPECT_DEATH(
      kernel_new_resize(
        &k,
        2,
        3,
        3,
        0,
        false,
        true,
        MAGIC_RESIZER_NEAREST_NEIGHBOR),
      "n_in (2) too large for n_out (3) and k (3): must be exactly 1");
  */
}

TEST(Magic, NearestNeighborDownsizer) {
  /*
  Kernel *k = null;

  EXPECT_DEATH(
      kernel_new_resize(
        &k,
        0,
        1,
        0.5,
        0,
        false,
        true,
        MAGIC_RESIZER_NEAREST_NEIGHBOR),
      "n_in cannot be zero");
  EXPECT_DEATH(
      kernel_new_resize(
        &k,
        1,
        0,
        0.5,
        0,
        false,
        true,
        MAGIC_RESIZER_NEAREST_NEIGHBOR),
      "n_out cannot be zero");
  EXPECT_DEATH(
      kernel_new_resize(
        &k,
        1,
        1,
        0,
        0,
        false,
        true,
        MAGIC_RESIZER_NEAREST_NEIGHBOR),
      "k (0) must be positive");
  EXPECT_DEATH(
      kernel_new_resize(
        &k,
        1,
        1,
        1,
        0,
        false,
        true,
        MAGIC_RESIZER_NEAREST_NEIGHBOR),
      "k cannot be 1");

  EXPECT_DEATH(
      kernel_new_resize(
        &k,
        4,
        1,
        0.5,
        0,
        false,
        true,
        MAGIC_RESIZER_NEAREST_NEIGHBOR),
      "n_out (1) too small for n_in (4) and k (1/2): must be exactly 2");
  EXPECT_DEATH(
      kernel_new_resize(
        &k,
        1,
        2,
        0.5,
        0,
        false,
        true,
        MAGIC_RESIZER_NEAREST_NEIGHBOR),
      "n_out (2) too large for n_in (1) and k (1/2): must be exactly 1");
  EXPECT_DEATH(
      kernel_new_resize(
        &k,
        2,
        2,
        0.5,
        0,
        false,
        true,
        MAGIC_RESIZER_NEAREST_NEIGHBOR),
      "n_out (2) too large for n_in (2) and k (1/2): must be exactly 1");
  EXPECT_DEATH(
      kernel_new_resize(
        &k,
        6,
        2,
        0.5,
        0,
        false,
        true,
        MAGIC_RESIZER_NEAREST_NEIGHBOR),
      "n_out (2) too small for n_in (6) and k (1/2): must be exactly 3");
  EXPECT_DEATH(
      kernel_new_resize(
        &k,
        4,
        3,
        0.5,
        0,
        false,
        true,
        MAGIC_RESIZER_NEAREST_NEIGHBOR),
      "n_out (3) too large for n_in (4) and k (1/2): must be exactly 2");
  EXPECT_DEATH(
      kernel_new_resize(
        &k,
        8,
        3,
        0.5,
        0,
        false,
        true,
        MAGIC_RESIZER_NEAREST_NEIGHBOR),
      "n_out (3) too small for n_in (8) and k (1/2): must be exactly 4");

  EXPECT_DEATH(
      kernel_new_resize(
        &k,
        6,
        1,
        1.0 / 3,
        0,
        false,
        true,
        MAGIC_RESIZER_NEAREST_NEIGHBOR),
      "n_out (1) too small for n_in (6) and k (1/3): must be exactly 2");
  EXPECT_DEATH(
      kernel_new_resize(
        &k,
        3,
        2,
        1.0 / 3,
        0,
        false,
        true,
        MAGIC_RESIZER_NEAREST_NEIGHBOR),
      "n_out (2) too large for n_in (3) and k (1/3): must be exactly 1");
  EXPECT_DEATH(
      kernel_new_resize(
        &k,
        9,
        2,
        1.0 / 3,
        0,
        false,
        true,
        MAGIC_RESIZER_NEAREST_NEIGHBOR),
      "n_out (2) too small for n_in (9) and k (1/3): must be exactly 3");

  EXPECT_DEATH(
      kernel_new_resize(
        &k,
        6,
        3,
        1.0 / 3,
        0,
        false,
        true,
        MAGIC_RESIZER_NEAREST_NEIGHBOR),
      "n_out (3) too large for n_in (6) and k (1/3): must be exactly 2");
  */
}

TEST(Magic, SharpMagicKernelUpsizer) {







}

#endif



FINISH_DEATH
