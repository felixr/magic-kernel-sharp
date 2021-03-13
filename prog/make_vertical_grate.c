#include "make_vertical_grate.h"

int main(int argc, char *argv[]) {
  byte by;
  byte quality;
  char *filename;
  ulong i;
  ulong j;
  ulong width;
  ulong height;
  ByteImagePixel p;
  ByteImage *out = null;

  args_description("Make a vertical grate image.");
  args_string("filename", "Filename of image file to be created");
  args_integer("width", "Width of image");
  args_integer("height", "Height of the image");
  args_flag("png", 'p', "Save the output file in PNG format");
  args_flag("jpeg", 'j', "Save the output file in JPEG format");
  args_optional_integer(
      "jpeg-quality",
      'q',
      jpeg_file_get_default_quality(),
      "The JPEG quality to use for the output JPEG file, between 0 and 100");
  args_parse(argc, argv);

  width = args_get_positive_integer("width");
  height = args_get_positive_integer("height");

  byte_image_new(&out, width, height);

  p.a = 255;

  for (i = 0; i < width; i++) {
    by = i % 2 == 0 ? 1 : 255;
    p.r = p.g = p.b = by;

    for (j = 0; j < height; j++) {
      byte_image_set(out, i, j, p);
    }
  }

  filename = args_get_string("filename");
  quality = args_get_bounded_integer("jpeg-quality", 0, 100);

  if (args_get_flag_veto("png", "jpeg")) {
    byte_image_save_png(out, filename);
  } else if (args_get_flag("jpeg")) {
    byte_image_save_jpeg(out, filename, quality);
  } else {
    byte_image_save_quality(out, filename, quality);
  }

  byte_image_delete(&out);

  return 0;
}
