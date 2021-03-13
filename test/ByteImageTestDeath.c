#include "ByteImageTestDeath.h"

START_DEATH

TEST(ByteImage, Simple) {
  ByteImagePixel pixel;
  ByteImage *bi = null;

  EXPECT_DEATH(byte_image_new(&bi, 0, 1), "Width must be nonzero");
  byte_image_delete(&bi);

  EXPECT_DEATH(byte_image_new(&bi, 1, 0), "Height must be nonzero");
  byte_image_delete(&bi);

  byte_image_new(&bi, 1, 1);
  EXPECT_DEATH(byte_image_get(bi, 1, 0), "i (1) must be less than width (1)");
  EXPECT_DEATH(byte_image_get(bi, 0, 1), "j (1) must be less than height (1)");
  pixel.r = pixel.g = pixel.b = pixel.a = 128;
  EXPECT_DEATH(
      byte_image_set(bi, 1, 0, pixel),
      "i (1) must be less than width (1)");
  EXPECT_DEATH(
      byte_image_set(bi, 0, 1, pixel),
      "j (1) must be less than height (1)");
  byte_image_delete(&bi);
}

TEST(ByteImage, Pixel) {
  EXPECT_DEATH(
      byte_image_pixel_opaque_from_rgb_hex(0x1000000),
      "Hex (0x1000000) too large: maximum for RGB is 0xffffff");
  EXPECT_DEATH(
      byte_image_pixel_opaque_from_rgb_hex(0xffffffff),
      "Hex (0xffffffff) too large: maximum for RGB is 0xffffff");
}

FINISH_DEATH
