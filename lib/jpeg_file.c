#include "jpeg_file.h"
#include <jpeglib.h>

#define DEFAULT_QUALITY_ 80

typedef struct {
  struct jpeg_error_mgr pub;
  jmp_buf setjmp_buffer;
} Error_;

static byte quality_ = DEFAULT_QUALITY_;
static char error_message_[JMSG_LENGTH_MAX];

static void error_(j_common_ptr cinfo);

ByteImage *byte_image_new_load_jpeg(ByteImage **pthis, char *filename) {
  byte *p;
  byte *row = null;
  ulong height;
  ulong i;
  ulong j;
  ulong width;
  struct jpeg_decompress_struct cinfo;
  Error_ err;
  File *f = null;
  ByteImagePixel pixel;
  ByteImage *this;

  if (!file_is_jpeg(filename)) {
    throw("%s is not a recognized JPEG file", filename);
  }

  file_new(&f, filename, "r");

  cinfo.err = jpeg_std_error(&err.pub);
  err.pub.error_exit = error_;

  if (setjmp(err.setjmp_buffer) != 0) {
    jpeg_destroy_decompress(&cinfo);
    file_delete(&f);
    throw("JPEG decompressor error: %s", error_message_);
  }

  jpeg_create_decompress(&cinfo);
  jpeg_stdio_src(&cinfo, f->ef->ansifile);

  jpeg_read_header(&cinfo, 1);

  if (cinfo.num_components != 3) {
    jpeg_destroy_decompress(&cinfo);
    file_delete(&f);
    throw("JPEG file %s is not a color image", filename);
  }

  if (cinfo.jpeg_color_space != JCS_YCbCr) {
    jpeg_destroy_decompress(&cinfo);
    file_delete(&f);
    throw("JPEG file %s has a colorspace that is not YCbCr", filename);
  }

  width = (ulong)cinfo.image_width;
  height = (ulong)cinfo.image_height;

  this = byte_image_new(pthis, width, height);

  new(&row, width * 3);

  cinfo.out_color_space = JCS_RGB;

  pixel.a = 255;

  jpeg_start_decompress(&cinfo);

  for (j = 0; j < height; j++) {
    if (jpeg_read_scanlines(&cinfo, &row, 1) != 1) {
      deletev(&row);
      jpeg_destroy_decompress(&cinfo);
      file_delete(&f);
      throw("Failed to read row %lu of JPEG file %s", j, filename);
    }

    for (i = 0, p = row; i < width; i++) {
      pixel.r = *p++;
      pixel.g = *p++;
      pixel.b = *p++;

      byte_image_set(this, i, j, pixel);
    }
  }

  jpeg_finish_decompress(&cinfo);

  deletev(&row);
  jpeg_destroy_decompress(&cinfo);
  file_delete(&f);

  return this;
}

void byte_image_save_jpeg(ByteImage *this, char *filename, byte quality) {
  byte *p;
  byte *row = null;
  ulong height;
  ulong i;
  ulong j;
  ulong width;
  struct jpeg_compress_struct cinfo;
  Error_ err;
  ByteImagePixel pixel;
  File *f = null;

  if (quality > 100) {
    throw("Quality (%u) must be no greater than 100", quality);
  }

  file_new(&f, filename, "w");

  cinfo.err = jpeg_std_error(&err.pub);
  err.pub.error_exit = error_;

  if (setjmp(err.setjmp_buffer) != 0) {
    jpeg_destroy_compress(&cinfo);
    file_delete(&f);
    throw("JPEG compressor error: %s", error_message_);
  }

  width = this->width;
  height = this->height;

  jpeg_create_compress(&cinfo);
  jpeg_stdio_dest(&cinfo, f->ef->ansifile);

  cinfo.image_width = width;
  cinfo.image_height = height;
  cinfo.input_components = 3;
  cinfo.in_color_space = JCS_RGB;
  jpeg_set_defaults(&cinfo);
  jpeg_set_quality(&cinfo, (int)quality, TRUE);

  new(&row, width * 3);

  jpeg_start_compress(&cinfo, 1);

  for (j = 0; j < height; j++) {
    for (i = 0, p = row; i < width; i++) {
      pixel = byte_image_get(this, i, j);

      *p++ = pixel.r;
      *p++ = pixel.g;
      *p++ = pixel.b;
    }

    if (jpeg_write_scanlines(&cinfo, &row, 1) != 1) {
      deletev(&row);
      jpeg_destroy_compress(&cinfo);
      file_delete(&f);
      throw("Failed to write row %lu of JPEG file %s", j, filename);
    }
  }

  jpeg_finish_compress(&cinfo);

  deletev(&row);
  jpeg_destroy_compress(&cinfo);
  file_delete(&f);
}

bool file_is_jpeg(char *filename) {
  struct jpeg_decompress_struct cinfo;
  Error_ err;
  File *f = null;

  file_new(&f, filename, "r");

  cinfo.err = jpeg_std_error(&err.pub);
  err.pub.error_exit = error_;

  if (setjmp(err.setjmp_buffer) != 0) {
    jpeg_destroy_decompress(&cinfo);
    file_delete(&f);
    return false;
  }

  jpeg_create_decompress(&cinfo);
  jpeg_stdio_src(&cinfo, f->ef->ansifile);

  jpeg_read_header(&cinfo, 1);

  jpeg_destroy_decompress(&cinfo);
  file_delete(&f);
  return true;
}

byte jpeg_file_set_default_quality(byte default_quality) {
  byte current;

  if (default_quality > 100) {
    throw("Quality (%u) must be no greater than 100", default_quality);
  }

  current = quality_;
  quality_ = default_quality;
  return current;
}

byte jpeg_file_get_default_quality(void) {
  return quality_;
}

static void error_(j_common_ptr cinfo) {
  Error_ *err;

  err = (Error_ *)cinfo->err;
  cinfo->err->format_message(cinfo, error_message_);
  longjmp(err->setjmp_buffer, 1);
}
