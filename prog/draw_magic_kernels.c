#include "draw_magic_kernels.h"

#define LEFT_ -3.1
#define RIGHT_ +3.1
#define TOP_ +1.03
#define BOTTOM_ -0.03

#define MAJOR_ 8
#define MINOR_ 3
#define OVERSAMPLE_ 10

static ulong width_;
static ulong height_;

static ulong x_to_i_(double x) {
  long i;

  i =  (long)round((x - LEFT_) / (RIGHT_ - LEFT_) * (width_ - 1));

  if (i < 0 || i >= width_) {
    throw("i (%ld) outside allowed range [0, %lu)", i, width_);
  }

  return (ulong)i;
}

static ulong y_to_j_(double y) {
  long j;

  j = (long)round((1 - (y - BOTTOM_) / (TOP_ - BOTTOM_)) * (height_ - 1));

  if (j < 0 || j >= height_) {
    throw("j (%ld) outside allowed range [0, %lu)", j, height_);
  }

  return (ulong)j;
}

int main(int argc, char *argv[]) {
  byte a;
  byte quality;
  byte tick;
  char *filename;
  uint colors[] = {
    0x000000,
    0x0000ff,
    0x00ff00,
    0xff0000,
    0x00ffff,
    0xff00ff,
    0xffff00
  };
  ulong i0;
  ulong i;
  ulong i_mh;
  ulong i_ph;
  ulong j0;
  ulong j1;
  ulong j;
  double dx;
  double x;
  double y;
  ByteImagePixel pixel;
  ByteImage *image = null;

  args_description(
      "Draw the six magic kernels, to allow a visual verification of the "
      "curves represented in the library");
  args_string("filename", "Image file to be created");
  args_optional_integer("width", 'w', 1600, "Width of the image");
  args_optional_integer("height", 'h', 900, "Height of the image");
  args_flag("png", 'p', "Save the output file in PNG format");
  args_flag("jpeg", 'j', "Save the output file in JPEG format");
  args_optional_integer(
      "jpeg-quality",
      'q',
      jpeg_file_get_default_quality(),
      "The JPEG quality to use for the output JPEG file, between 0 and 100");
  args_parse(argc, argv);

  width_ = args_get_positive_integer("width");
  height_ = args_get_positive_integer("height");

  byte_image_new(&image, width_, height_);

  pixel = byte_image_pixel_opaque_from_rgb_hex(0xffffff);

  j0 = y_to_j_(0);

  for (i = 0; i < width_; i++) {
    byte_image_set(image, i, j0, pixel);
  }

  i0 = x_to_i_(0);

  for (j = 0; j < height_; j++) {
    byte_image_set(image, i0, j, pixel);
  }

  dx = (RIGHT_ - LEFT_) / (OVERSAMPLE_ * maximum(width_, height_));

  for (a = 1; a <= 6; a++) {
    pixel = byte_image_pixel_opaque_from_rgb_hex(colors[a]);

    for (x = -(double)a / 2; x <= +(double)a / 2; x += dx) {
      y = magic_kernel(a, x);
      i = x_to_i_(x);
      j = y_to_j_(y);
      byte_image_set(image, i, j, pixel);
    }

    if (a == 1) {
      i_mh = x_to_i_(-0.5);
      i_ph = x_to_i_(+0.5);
      j1 = y_to_j_(1);

      for (j = j1; j <= j0; j++) {
        byte_image_set(image, i_mh, j, pixel);
        byte_image_set(image, i_ph, j, pixel);
      }
    }
  }

  pixel = byte_image_pixel_opaque_from_rgb_hex(0xffffff);

  j0 = y_to_j_(0);

  for (x = -3; x <= +3; x += 0.5) {
    i = x_to_i_(x);

    tick = x == round(x) ? MAJOR_ : MINOR_;

    for (j = j0 - tick; j <= j0 + tick; j++) {
      byte_image_set(image, i, j, pixel);
    }
  }

  i0 = x_to_i_(0);

  for (y = 0; y <= 1; y += 0.25) {
    j = y_to_j_(y);

    tick = y == round(y) ? MAJOR_ : MINOR_;

    for (i = i0 - tick; i <= i0 + tick; i++) {
      byte_image_set(image, i, j, pixel);
    }
  }

  filename = args_get_string("filename");
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
