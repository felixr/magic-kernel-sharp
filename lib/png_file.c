#include "png_file.h"
#include <png.h>

ByteImage *byte_image_new_load_png(ByteImage **pthis, char *filename) {
  byte bit_depth;
  byte color_type;
  byte header[8];
  byte *p;
  byte *png_row;
  byte **png_rows = null;
  ulong width;
  ulong height;
  ulong i;
  ulong j;
  ulong bytes_per_row;
  png_structp png;
  png_infop info;
  png_infop end;
  File *f = null;
  FILE *ansifile;
  ByteImagePixel *pixel;
  ByteImagePixel *row;
  ByteImagePixel **rows;
  ByteImage *this;

  file_new(&f, filename, "r");
  ansifile = f->ef->ansifile;

  file_read_bytes(f, header, 8);
  if (png_sig_cmp(header, 0, 8) != 0) {
    throw("File '%s' not recognized as a PNG file", filename);
  }

  if ((png = png_create_read_struct(
          PNG_LIBPNG_VER_STRING,
          null,
          null,
          null)) == null) {
    throw("Failed to create PNG file");
  }

  if ((info = png_create_info_struct(png)) == null) {
    throw("Failed to create info");
  }

  if ((end = png_create_info_struct(png)) == null) {
    throw("Failed to create end");
  }

  if (setjmp(png_jmpbuf(png)) != 0) {
    throw("Failed to read header");
  }

  png_init_io(png, ansifile);
  png_set_sig_bytes(png, 8);

  png_read_info(png, info);

  width = png_get_image_width(png, info);
  height = png_get_image_height(png, info);

  bit_depth = png_get_bit_depth(png, info);

  color_type = png_get_color_type(png, info);

  png_set_expand(png);

  if (bit_depth == 16) {
    png_set_strip_16(png);
  }

  if (bit_depth < 8) {
    png_set_packing(png);
  }

  if (color_type == PNG_COLOR_TYPE_GRAY
      || color_type == PNG_COLOR_TYPE_GRAY_ALPHA) {
    png_set_gray_to_rgb(png);
  }

  if (color_type == PNG_COLOR_TYPE_RGB || color_type == PNG_COLOR_TYPE_GRAY) {
    png_set_filler(png, 0xff, PNG_FILLER_AFTER);
  }

  this = byte_image_new(pthis, width, height);

  png_set_interlace_handling(png);
  png_read_update_info(png, info);

  new(&png_rows, height);

  bytes_per_row = width * 4;

  for (j = 0; j < this->height; j++) {
    new(&png_rows[j], bytes_per_row);
  }

  if (setjmp(png_jmpbuf(png)) != 0) {
    throw("Failed to read image");
  }

  png_read_image(png, png_rows);

  rows = this->rows;

  for (j = 0; j < height; j++) {
    row = rows[j];
    png_row = png_rows[j];
    for (p = png_row, i = 0; i < width; i++) {
      pixel = &row[i];
      pixel->r = *p++;
      pixel->g = *p++;
      pixel->b = *p++;
      pixel->a = *p++;
    }
  }

  delete(&png_rows, height, value_array_delete);

  png_read_end(png, end);

  png_destroy_read_struct(&png, &info, &end);

  file_delete(&f);

  return this;
}

void byte_image_save_png(ByteImage *this, char *filename) {
  byte *p;
  byte *png_row;
  byte **png_rows = null;
  ulong width;
  ulong height;
  ulong i;
  ulong j;
  ulong bytes_per_row;
  ByteImagePixel *pixel;
  ByteImagePixel *row;
  ByteImagePixel **rows;
  FILE *ansifile;
  png_structp png;
  png_infop info;

  width = this->width;
  height = this->height;

  if ((ansifile = fopen(filename, "w")) == null) {
    throw("Could not open '%s' for writing", filename);
  }

  if ((png = png_create_write_struct(
          PNG_LIBPNG_VER_STRING,
          null,
          null,
          null)) == null) {
    throw("Failed to create PNG file");
  }

  if ((info = png_create_info_struct(png)) == null) {
    throw("Failed to create info");
  }

  if (setjmp(png_jmpbuf(png)) != 0) {
    throw("Error in initialization");
  }
  png_init_io(png, ansifile);

  if (setjmp(png_jmpbuf(png)) != 0) {
    throw("Failed to write header");
  }

  png_set_IHDR(
      png,
      info,
      width,
      height,
      8,
      6,
      PNG_INTERLACE_NONE,
      PNG_COMPRESSION_TYPE_BASE,
      PNG_FILTER_TYPE_BASE);

  png_write_info(png, info);

  if (setjmp(png_jmpbuf(png)) != 0) {
    throw("Failed to write image");
  }

  new(&png_rows, height);

  bytes_per_row = width * 4;

  for (j = 0; j < height; j++) {
    new(&png_rows[j], bytes_per_row);
  }

  rows = this->rows;

  for (j = 0; j < height; j++) {
    row = rows[j];
    png_row = png_rows[j];
    for (p = png_row, i = 0; i < width; i++) {
      pixel = &row[i];
      *p++ = pixel->r;
      *p++ = pixel->g;
      *p++ = pixel->b;
      *p++ = pixel->a;
    }
  }

  png_write_image(png, png_rows);

  delete(&png_rows, height, value_array_delete);

  if (setjmp(png_jmpbuf(png)) != 0) {
    throw("Failed to end writing of image");
  }

  png_write_end(png, null);

  png_destroy_write_struct(&png, &info);

  fclose(ansifile);
}

bool file_is_png(char *filename) {
  bool is_png;
  byte header[8];
  File *f = null;
  FILE *ansifile;

  file_new(&f, filename, "r");
  ansifile = f->ef->ansifile;

  file_read_bytes(f, header, 8);
  is_png = png_sig_cmp(header, 0, 8) == 0;

  file_delete(&f);

  return is_png;
}
