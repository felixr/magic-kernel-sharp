#include "png_file_test.h"

START

TEST(PngFile, Simple) {
  char *filename = null;
  ByteImage *bi = null;

  byte_image_new(&bi, 2, 2);
  byte_image_set_r(bi, 0, 0, 89);
  byte_image_set_g(bi, 0, 1, 125);
  byte_image_set_b(bi, 1, 0, 46);
  byte_image_set_a(bi, 1, 1, 129);
  string_new_temp_filename(&filename);
  byte_image_save_png(bi, filename);
  byte_image_delete(&bi);
  EXPECT_TRUE(file_is_png(filename));
  byte_image_new_load_png(&bi, filename);
  EXPECT_ULEQ(89, byte_image_get_r(bi, 0, 0));
  EXPECT_ULEQ(0, byte_image_get_g(bi, 0, 0));
  EXPECT_ULEQ(0, byte_image_get_b(bi, 0, 0));
  EXPECT_ULEQ(255, byte_image_get_a(bi, 0, 0));
  EXPECT_ULEQ(0, byte_image_get_r(bi, 0, 1));
  EXPECT_ULEQ(125, byte_image_get_g(bi, 0, 1));
  EXPECT_ULEQ(0, byte_image_get_b(bi, 0, 1));
  EXPECT_ULEQ(255, byte_image_get_a(bi, 0, 1));
  EXPECT_ULEQ(0, byte_image_get_r(bi, 1, 0));
  EXPECT_ULEQ(0, byte_image_get_g(bi, 1, 0));
  EXPECT_ULEQ(46, byte_image_get_b(bi, 1, 0));
  EXPECT_ULEQ(255, byte_image_get_a(bi, 1, 0));
  EXPECT_ULEQ(0, byte_image_get_r(bi, 1, 1));
  EXPECT_ULEQ(0, byte_image_get_g(bi, 1, 1));
  EXPECT_ULEQ(0, byte_image_get_b(bi, 1, 1));
  EXPECT_ULEQ(129, byte_image_get_a(bi, 1, 1));
  byte_image_delete(&bi);
  external_file_remove(filename);
  string_delete(&filename);
}

FINISH
