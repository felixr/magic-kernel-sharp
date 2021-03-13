#include "copy_image.h"

int main(int argc, char *argv[]) {
  byte quality;
  char *filename;
  ByteImage *image = null;

  args_description("Copy an image");
  args_string("input-filename", "Filename of the input file");
  args_string("output-filename", "Filename of the output file");
  args_flag("png", 'p', "Save the output file in PNG format");
  args_flag("jpeg", 'j', "Save the output file in JPEG format");
  args_optional_integer(
      "jpeg-quality",
      'q',
      jpeg_file_get_default_quality(),
      "The JPEG quality to use for the output JPEG file, between 0 and 100");
  args_parse(argc, argv);

  byte_image_new_load(&image, args_get_string("input-filename"));

  filename = args_get_string("output-filename");
  quality = args_get_bounded_integer("jpeg-quality", 0, 100);

  if (args_get_flag_veto("png", "jpeg")) {
    byte_image_save_png(image, filename);
  } else if (args_get_flag("jpeg")) {
    byte_image_save_jpeg(image, filename, quality);
  } else {
    byte_image_save_quality(image, filename, quality);
  }

  byte_image_delete(&image);

  return 0;
}
