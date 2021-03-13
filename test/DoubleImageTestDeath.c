#include "DoubleImageTestDeath.h"

START_DEATH

TEST(DoubleImage, Simple) {
  DoubleImage *di = null;

  EXPECT_DEATH(double_image_new(&di, 0, 1, false), "Width must be nonzero");
  double_image_delete(&di);

  EXPECT_DEATH(double_image_new(&di, 1, 0, false), "Height must be nonzero");
  double_image_delete(&di);

  double_image_new(&di, 1, 1, false);
  EXPECT_DEATH(
      double_image_make_opaque(di),
      "Image does not have alpha channel");
  double_image_delete(&di);
}

FINISH_DEATH
