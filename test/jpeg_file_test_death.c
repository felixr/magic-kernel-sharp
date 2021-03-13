#include "jpeg_file_test_death.h"

START_DEATH

TEST(JpegFile, Death) {
  char *filename = null;
  ByteImage *bi = null;

  EXPECT_DEATH(
      jpeg_file_set_default_quality(101),
      "Quality (101) must be no greater than 100");

  byte_image_new(&bi, 2, 2);
  string_new_temp_filename(&filename);
  EXPECT_DEATH(
      byte_image_save_jpeg(bi, filename, 101),
      "Quality (101) must be no greater than 100");
  byte_image_delete(&bi);
}

FINISH_DEATH
