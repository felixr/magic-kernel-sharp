#include "DoubleImageTest.h"

START

TEST(DoubleImage, Constructor) {
  DoubleImage *di = null;

  double_image_new(&di, 3, 4, false);
  EXPECT_ULEQ(3, di->width);
  EXPECT_ULEQ(4, di->height);
  EXPECT_NULL(di->a);
  double_image_delete(&di);

  double_image_new(&di, 3, 4, true);
  EXPECT_ULEQ(3, di->width);
  EXPECT_ULEQ(4, di->height);
  EXPECT_NONNULL(di->a);
  double_image_delete(&di);

  double_image_new_rgb(&di, 3, 4);
  EXPECT_ULEQ(3, di->width);
  EXPECT_ULEQ(4, di->height);
  EXPECT_NULL(di->a);
  double_image_delete(&di);

  double_image_new_rgba(&di, 3, 4);
  EXPECT_ULEQ(3, di->width);
  EXPECT_ULEQ(4, di->height);
  EXPECT_NONNULL(di->a);
  double_image_delete(&di);
}

TEST(DoubleImage, Access) {
  ulong i;
  ulong j;
  DoubleImage *di = null;
  DoubleImage *di2 = null;

  double_image_new_rgb(&di, 2, 3);
  di->r[0][0] = 1.5;
  di->g[1][2] = 2.6;
  di->b[0][1] = -1.3;

  for (i = 0; i < 2; i++) {
    for (j = 0; j < 3; j++) {
      EXPECT_DEQ(i == 0 && j == 0 ? 1.5 : 0, di->r[i][j]);
      EXPECT_DEQ(i == 1 && j == 2 ? 2.6 : 0, di->g[i][j]);
      EXPECT_DEQ(i == 0 && j == 1 ? -1.3 : 0, di->b[i][j]);
    }
  }
  double_image_delete(&di);

  double_image_new_rgba(&di, 2, 3);
  di->r[0][0] = 1.5;
  di->g[1][2] = 2.6;
  di->b[0][1] = -1.3;
  di->a[0][1] = 0.3;

  for (i = 0; i < 2; i++) {
    for (j = 0; j < 3; j++) {
      EXPECT_DEQ(i == 0 && j == 0 ? 1.5 : 0, di->r[i][j]);
      EXPECT_DEQ(i == 1 && j == 2 ? 2.6 : 0, di->g[i][j]);
      EXPECT_DEQ(i == 0 && j == 1 ? -1.3 : 0, di->b[i][j]);
      EXPECT_DEQ(i == 0 && j == 1 ? 0.3 : 1, di->a[i][j]);
    }
  }

  double_image_new_copy(&di2, di);
  di2->r[0][0] = 1.5;
  di2->g[1][2] = 2.6;
  di2->b[0][1] = -1.3;
  di2->a[0][1] = 0.3;

  for (i = 0; i < 2; i++) {
    for (j = 0; j < 3; j++) {
      EXPECT_DEQ(i == 0 && j == 0 ? 1.5 : 0, di2->r[i][j]);
      EXPECT_DEQ(i == 1 && j == 2 ? 2.6 : 0, di2->g[i][j]);
      EXPECT_DEQ(i == 0 && j == 1 ? -1.3 : 0, di2->b[i][j]);
      EXPECT_DEQ(i == 0 && j == 1 ? 0.3 : 1, di2->a[i][j]);
    }
  }

  double_image_delete(&di2);
  double_image_delete(&di);
}

TEST(DoubleImage, Default) {
  ulong i;
  ulong j;
  DoubleImage *di = null;

  double_image_new_rgba(&di, 2, 3);

  for (i = 0; i < 2; i++) {
    for (j = 0; j < 3; j++) {
      EXPECT_DEQ(0, di->r[i][j]);
      EXPECT_DEQ(0, di->g[i][j]);
      EXPECT_DEQ(0, di->b[i][j]);
      EXPECT_DEQ(1, di->a[i][j]);
    }
  }

  double_image_delete(&di);
}

TEST(DoubleImage, MakeOpaque) {
  ulong i;
  ulong j;
  DoubleImage *di = null;

  double_image_new_rgba(&di, 2, 3);

  for (i = 0; i < 2; i++) {
    for (j = 0; j < 3; j++) {
      di->a[i][j] = i + j;
    }
  }

  double_image_make_opaque(di);

  for (i = 0; i < 2; i++) {
    for (j = 0; j < 3; j++) {
      EXPECT_DEQ(1, di->a[i][j]);
    }
  }

  double_image_delete(&di);
}

TEST(DoubleImage, Clear) {
  ulong i;
  ulong j;
  DoubleImage *di = null;

  double_image_new_rgb(&di, 2, 3);

  for (i = 0; i < 2; i++) {
    for (j = 0; j < 3; j++) {
      di->r[i][j] = i + j;
      di->g[i][j] = i - j;
      di->b[i][j] = i * j;
    }
  }

  double_image_clear(di, 1, 2, 3, 4);

  for (i = 0; i < 2; i++) {
    for (j = 0; j < 3; j++) {
      EXPECT_DEQ(1, di->r[i][j]);
      EXPECT_DEQ(2, di->g[i][j]);
      EXPECT_DEQ(3, di->b[i][j]);
    }
  }

  double_image_delete(&di);

  double_image_new_rgba(&di, 2, 3);

  for (i = 0; i < 2; i++) {
    for (j = 0; j < 3; j++) {
      di->r[i][j] = i + j;
      di->g[i][j] = i - j;
      di->b[i][j] = i * j;
      di->a[i][j] = (double)i / (j + 1);
    }
  }

  double_image_clear(di, 1, 2, 3, 4);

  for (i = 0; i < 2; i++) {
    for (j = 0; j < 3; j++) {
      EXPECT_DEQ(1, di->r[i][j]);
      EXPECT_DEQ(2, di->g[i][j]);
      EXPECT_DEQ(3, di->b[i][j]);
      EXPECT_DEQ(4, di->a[i][j]);
    }
  }

  double_image_delete(&di);
}

TEST(DoubleImage, ContainsTransparency) {
  DoubleImage *di = null;

  double_image_new_rgb(&di, 2, 3);
  EXPECT_FALSE(double_image_contains_transparency(di));
  double_image_delete(&di);

  double_image_new_rgba(&di, 2, 3);
  EXPECT_FALSE(double_image_contains_transparency(di));
  di->a[1][2] = 0.3;
  EXPECT_TRUE(double_image_contains_transparency(di));
  double_image_delete(&di);
}

FINISH
