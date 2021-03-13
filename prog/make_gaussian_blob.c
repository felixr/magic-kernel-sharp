#include "make_gaussian_blob.h"
#include <math.h>

int main(int argc, char *argv[]) {
  byte by;
  byte quality;
  char *filename;
  ulong i;
  ulong j;
  ulong width;
  ulong height;
  double center_x;
  double center_y;
  double norm;
  double r;
  double radius;
  double x;
  double y;
  ByteImagePixel p;
  ByteImage *out = null;

  args_description("Make a Gaussian blob image.");
  args_string("filename", "Filename of image file to be created");
  args_integer("width", "Width of image");
  args_integer("height", "Height of the image");
  args_real("radius", "Radius of the blob");
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
  radius = args_get_positive_real("radius");

  byte_image_new(&out, width, height);

  p.a = 255;
  center_x = ((double)width - 1) / 2;
  center_y = ((double)height - 1) / 2;
  norm = 255 / normal(0);

  for (i = 0; i < width; i++) {
    for (j = 0; j < height; j++) {
      x = (double)i - center_x;
      y = (double)j - center_y;
      r = sqrt(x * x + y * y);

      by = (byte)round(normal(r / radius) * norm);
      p.r = p.g = p.b = by;
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
