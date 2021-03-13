#include "subtract_image.h"

int main(int argc, char *argv[]) {
  byte quality;
  char *filename;
  ulong i;
  ulong j;
  ulong width;
  ulong height;
  double m;
  ByteImagePixel p1;
  ByteImagePixel p2;
  ByteImagePixel pdiff;
  ByteImage *out = null;
  ByteImage *in1 = null;
  ByteImage *in2 = null;

  args_description(
      "Subtract an image from another of the same size, representing the "
      "difference as an image, where mid-gray represents a zero difference.");
  args_string(
      "input-filename-1",
      "Image to be subtracted from");
  args_string(
      "input-filename-2",
      "Image to be subtracted");
  args_string("output-filename", "Difference image to be created");
  args_optional_real(
      "magnify",
      'm',
      1,
      "The difference from mid-gray in the output will be $ units of "
      "intensity for each unit of intensity difference of the input files");
  args_flag("png", 'p', "Save the output file in PNG format");
  args_flag("jpeg", 'j', "Save the output file in JPEG format");
  args_optional_integer(
      "jpeg-quality",
      'q',
      jpeg_file_get_default_quality(),
      "The JPEG quality to use for the output JPEG file, between 0 and 100");
  args_parse(argc, argv);

  byte_image_new_load(&in1, args_get_string("input-filename-1"));
  byte_image_new_load(&in2, args_get_string("input-filename-2"));

  if ((width = in1->width) != in2->width) {
    throw("Images have different widths (%lu v %lu)", width, in2->width);
  }

  if ((height = in1->height) != in2->height) {
    throw("Images have different heights (%lu v %lu)", height, in2-height);
  }

  if (byte_image_contains_transparency(in1) || byte_image_contains_transparency(in2)) {
    throw("Cannot use this program on images with transparency");
  }

  m = args_get_positive_real("magnify");

  byte_image_new(&out, width, height);

  pdiff.a = 255;

  for (j = 0; j < height; j++) {
    for (i = 0; i < width; i++) {
      p1 = byte_image_get(in1, i, j);
      p2 = byte_image_get(in2, i, j);

      pdiff.r = byte_image_bound(m * ((short)p1.r - (short)p2.r) + 128);
      pdiff.g = byte_image_bound(m * ((short)p1.g - (short)p2.g) + 128);
      pdiff.b = byte_image_bound(m * ((short)p1.b - (short)p2.b) + 128);

      byte_image_set(out, i, j, pdiff);
    }
  }

  byte_image_delete(&in2);
  byte_image_delete(&in1);

  filename = args_get_string("output-filename");
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
