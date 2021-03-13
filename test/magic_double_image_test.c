/* We don't need to test too much here, because the components are already
 * tested in magic_test, DoubleImageTest, and double_image_kernel_test. This is
 * just to make sure it's hooked up.
 */
#include "magic_double_image_test.h"

START

TEST(MagicDoubleImage, Resize) {
  bool in;
  bool in_i;
  bool in_j;
  ulong i;
  ulong j;
  DoubleImage *din = null;
  DoubleImage *dout = null;

  double_image_new(&din, 5, 5, true);
  din->r[2][2] = 0.5;
  din->g[2][2] = 0.75;
  din->b[2][2] = 0.25;
  din->a[2][2] = 0.1;

  double_image_new_resize(
      &dout,
      din,
      11,
      11,
      2,
      MAGIC_RESIZER_NEAREST_NEIGHBOR,
      0,
      0);

  for (i = 0; i < 11; i++) {
    in_i = i == 4 || i == 5;

    for (j = 0; j < 11; j++) {
      in_j = j == 4 || j == 5;
      in = in_i && in_j;
      EXPECT_DEQ(in ? 0.5 : 0, dout->r[i][j]);
      EXPECT_DEQ(in ? 0.75 : 0, dout->g[i][j]);
      EXPECT_DEQ(in ? 0.25 : 0, dout->b[i][j]);
      EXPECT_DEQ(in ? 0.1 : 1, dout->a[i][j]);
    }
  }

  double_image_delete(&dout);
  double_image_delete(&din);
}

TEST(MagicDoubleImage, Blur) {
  ulong i;
  ulong j;
  double weight;
  double weights[] = {0, 0.125, 0.75, 0.125, 0};
  DoubleImage *din = null;
  DoubleImage *dout = null;

  double_image_new(&din, 5, 5, true);
  din->r[2][2] = 0.5;
  din->g[2][2] = 0.75;
  din->b[2][2] = 0.25;
  din->a[2][2] = 0.1;

  double_image_new_blur(&dout, din, 1);

  for (i = 0; i < 5; i++) {
    for (j = 0; j < 5; j++) {
      weight = weights[i] * weights[j];
      EXPECT_DEQ(0.5 * weight, dout->r[i][j]);
      EXPECT_DEQ(0.75 * weight, dout->g[i][j]);
      EXPECT_DEQ(0.25 * weight, dout->b[i][j]);
      EXPECT_NEAR(1 - 0.9 * weight, dout->a[i][j], 2e-16);
    }
  }

  double_image_delete(&dout);
  double_image_delete(&din);
}

TEST(MagicDoubleImage, Sharpen) {
  ulong i;
  ulong j;
  double weight;
  double weights[] = {0, -0.25, 1.5, -0.25, 0};
  DoubleImage *din = null;
  DoubleImage *dout = null;

  double_image_new(&din, 5, 5, true);
  din->r[2][2] = 0.5;
  din->g[2][2] = 0.75;
  din->b[2][2] = 0.25;
  din->a[2][2] = 0.1;

  double_image_new_sharpen(&dout, din, 1);

  for (i = 0; i < 5; i++) {
    for (j = 0; j < 5; j++) {
      weight = weights[i] * weights[j];
      EXPECT_DEQ(0.5 * weight, dout->r[i][j]);
      EXPECT_DEQ(0.75 * weight, dout->g[i][j]);
      EXPECT_DEQ(0.25 * weight, dout->b[i][j]);
      EXPECT_NEAR(1 - 0.9 * weight, dout->a[i][j], 3e-16);
    }
  }

  double_image_delete(&dout);
  double_image_delete(&din);
}

FINISH
