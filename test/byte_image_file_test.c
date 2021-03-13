#include "byte_image_file_test.h"

START

TEST(ByteImageFile, Simple) {
  char *filename = null;
  ByteImage *bi = null;

  byte_image_new(&bi, 1, 1);
  string_new_temp_filename_extension(&filename, "png");
  byte_image_save(bi, filename);
  byte_image_delete(&bi);
  EXPECT_STREQ("png", byte_image_filetype(filename));
  byte_image_new_load(&bi, filename);
  byte_image_delete(&bi);
  external_file_remove(filename);
  string_delete(&filename);

  byte_image_new(&bi, 1, 1);
  string_new_temp_filename_extension(&filename, "jpg");
  byte_image_save(bi, filename);
  byte_image_delete(&bi);
  EXPECT_STREQ("jpeg", byte_image_filetype(filename));
  byte_image_new_load(&bi, filename);
  byte_image_delete(&bi);
  external_file_remove(filename);
  string_delete(&filename);

  byte_image_new(&bi, 1, 1);
  string_new_temp_filename_extension(&filename, "jpg");
  byte_image_save_quality(bi, filename, 80);
  byte_image_delete(&bi);
  EXPECT_STREQ("jpeg", byte_image_filetype(filename));
  byte_image_new_load(&bi, filename);
  byte_image_delete(&bi);
  external_file_remove(filename);
  string_delete(&filename);

  byte_image_new(&bi, 1, 1);
  string_new_temp_filename_extension(&filename, "jpg");
  byte_image_save_type_quality(bi, filename, "jpeg", 80);
  byte_image_delete(&bi);
  EXPECT_STREQ("jpeg", byte_image_filetype(filename));
  byte_image_new_load(&bi, filename);
  byte_image_delete(&bi);
  external_file_remove(filename);
  string_delete(&filename);
}

FINISH
