#include "byte_image_file_test_death.h"

START_DEATH

TEST(ByteImageFile, Death) {
  char *e = null;
  char *filename = null;
  ByteImage *bi = null;
  File *f = null;

  string_new_temp_filename(&filename);
  file_new(&f, filename, "w");
  file_write_string(f, "This is not really an image file");
  file_delete(&f);
  string_new_f(&e, "%s cannot be recognized as a PNG or JPEG file", filename);
  EXPECT_DEATH(byte_image_new_load(&bi, filename), e);
  string_delete(&e);
  external_file_remove(filename);
  string_delete(&filename);

  byte_image_new(&bi, 1, 1);
  string_new_temp_filename(&filename);
  string_new_f(
      &e,
      "Could not infer the filetype desired from filename '%s'",
      filename);
  EXPECT_DEATH(byte_image_save(bi, filename), e);
  EXPECT_DEATH(byte_image_save_quality(bi, filename, 89), e);
  string_delete(&e);
  string_new_f(&e, "Filetype 'dog' not recognized", filename);
  EXPECT_DEATH(byte_image_save_type_quality(bi, filename, "dog", 89), e);
  string_delete(&e);
  string_delete(&filename);
  byte_image_delete(&bi);
}

FINISH_DEATH
