#include "blur_image.h"

int main(int argc, char *argv[]) {
  byte quality;
  char *filename;
  ByteImage *in = null;
  ByteImage *out = null;
  DoubleImage *din = null;
  DoubleImage *dout = null;

  args_description("Blur an image");
  args_string("input-filename", "Input image to be blurred");
  args_real("blur", "The amount of blur to be applied");
  args_string("output-filename", "Blurred output image to be created");
  args_optional_real("scale", 's', 1, "The amount to scale the intensity by");
  args_flag("png", 'p', "Save the output file in PNG format");
  args_flag("jpeg", 'j', "Save the output file in JPEG format");
  args_optional_integer(
      "jpeg-quality",
      'q',
      jpeg_file_get_default_quality(),
      "The JPEG quality to use for the output JPEG file, between 0 and 100");
  args_parse(argc, argv);

  if (args_get_positive_real("scale") != 1) {
    throw("Scale not reimplemented yet");
  }

  byte_image_new_load(&in, args_get_string("input-filename"));
  double_image_new_from_byte(&din, in);
  byte_image_delete(&in);
  double_image_new_blur(
      &dout,
      din,
      args_get_positive_real("blur"));
  double_image_delete(&din);
  byte_image_new_from_double(&out, dout);
  double_image_delete(&dout);

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
