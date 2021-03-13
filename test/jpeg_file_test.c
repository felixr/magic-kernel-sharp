#include "jpeg_file_test.h"

START

TEST(JpegFile, Simple) {
  char *filename = null;
  ByteImagePixel pixel;
  ByteImage *bi = null;

  byte_image_new(&bi, 2, 2);
  pixel.r = pixel.g = pixel.b = 128;
  byte_image_clear(bi, pixel);
  byte_image_set_r(bi, 0, 0, 130);
  byte_image_set_g(bi, 0, 1, 124);
  byte_image_set_b(bi, 1, 0, 133);
  string_new_temp_filename(&filename);
  byte_image_save_jpeg(bi, filename, 100);
  byte_image_delete(&bi);
  EXPECT_TRUE(file_is_jpeg(filename));
  byte_image_new_load_jpeg(&bi, filename);
  EXPECT_ULEQ(129, byte_image_get_r(bi, 0, 0));
  EXPECT_ULEQ(127, byte_image_get_g(bi, 0, 0));
  EXPECT_ULEQ(33, (byte_image_get_b(bi, 0, 0) + 2) >> 2);
  EXPECT_ULEQ(128, byte_image_get_r(bi, 0, 1));
  EXPECT_ULEQ(126, byte_image_get_g(bi, 0, 1));
  EXPECT_ULEQ(33, (byte_image_get_b(bi, 0, 1) + 3) >> 2);
  EXPECT_ULEQ(131, byte_image_get_r(bi, 1, 0));
  EXPECT_ULEQ(129, byte_image_get_g(bi, 1, 0));
  EXPECT_ULEQ(33, byte_image_get_b(bi, 1, 0) >> 2);
  EXPECT_ULEQ(129, byte_image_get_r(bi, 1, 1));
  EXPECT_ULEQ(127, byte_image_get_g(bi, 1, 1));
  EXPECT_ULEQ(130, byte_image_get_b(bi, 1, 1));
  byte_image_delete(&bi);
  external_file_remove(filename);
  string_delete(&filename);
}

TEST(JpegFile, DefaultQuality) {
  jpeg_file_set_default_quality(47);
  EXPECT_ULEQ(47, jpeg_file_get_default_quality());
  EXPECT_ULEQ(47, jpeg_file_set_default_quality(89));
  EXPECT_ULEQ(89, jpeg_file_get_default_quality());
}

FINISH
