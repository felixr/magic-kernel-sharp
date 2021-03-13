#include "byte_image_file.h"

ByteImage *byte_image_new_load(ByteImage **pthis, char *filename) {
  char *filetype;

  filetype = byte_image_filetype(filename);

  if (filetype == null) {
    throw("%s cannot be recognized as a PNG or JPEG file", filename);
  }

  if (string_equals(filetype, BYTE_IMAGE_FILETYPE_PNG)) {
    return byte_image_new_load_png(pthis, filename);
  }

  if (string_equals(filetype, BYTE_IMAGE_FILETYPE_JPEG)) {
    return byte_image_new_load_jpeg(pthis, filename);
  }

  throw("impossible");
  return null;
}

void byte_image_save(ByteImage *this, char *filename) {
  byte_image_save_quality(this, filename, jpeg_file_get_default_quality());
}

void byte_image_save_quality(ByteImage *this, char *filename, byte quality) {
  if (string_ends_with(filename, ".png")
      || string_ends_with(filename, ".PNG")) {
    byte_image_save_png(this, filename);
    return;
  }

  if (string_ends_with(filename, ".jpg")
      || string_ends_with(filename, ".JPG")
      || string_ends_with(filename, ".jpeg")
      || string_ends_with(filename, ".JPEG")
      || string_ends_with(filename, ".jpe")
      || string_ends_with(filename, ".JPE")
      || string_ends_with(filename, ".jif")
      || string_ends_with(filename, ".JIF")
      || string_ends_with(filename, ".jfif")
      || string_ends_with(filename, ".JFIF")
      || string_ends_with(filename, ".jfi")
      || string_ends_with(filename, ".JFI")) {
    byte_image_save_jpeg(this, filename, quality);
    return;
  }

  throw("Could not infer the filetype desired from filename '%s'", filename);
}

void byte_image_save_type_quality(
    ByteImage *this,
    char *filename,
    char *filetype,
    byte quality) {
  if (string_equals(filetype, BYTE_IMAGE_FILETYPE_PNG)) {
    byte_image_save_png(this, filename);
  } else if (string_equals(filetype, BYTE_IMAGE_FILETYPE_JPEG)) {
    byte_image_save_jpeg(this, filename, quality);
  } else {
    throw("Filetype '%s' not recognized", filetype);
  }
}

char *byte_image_filetype(char *filename) {
  if (file_is_png(filename)) {
    return BYTE_IMAGE_FILETYPE_PNG;
  }
  if (file_is_jpeg(filename)) {
    return BYTE_IMAGE_FILETYPE_JPEG;
  }
  return null;
}
