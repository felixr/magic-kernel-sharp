#include "ByteImage.h"

static void check_(ByteImage *this, ulong i, ulong j);
static ByteImagePixel get_(ByteImage *this, ulong i, ulong j);
static byte get_r_(ByteImage *this, ulong i, ulong j);
static byte get_g_(ByteImage *this, ulong i, ulong j);
static byte get_b_(ByteImage *this, ulong i, ulong j);
static byte get_a_(ByteImage *this, ulong i, ulong j);
static void set_(ByteImage *this, ulong i, ulong j, ByteImagePixel pixel);
static void set_r_(ByteImage *this, ulong i, ulong j, byte r);
static void set_g_(ByteImage *this, ulong i, ulong j, byte g);
static void set_b_(ByteImage *this, ulong i, ulong j, byte b);
static void set_a_(ByteImage *this, ulong i, ulong j, byte a);
static double hue_to_rgb_(double p, double q, double t);

ByteImage *byte_image_new(ByteImage **pthis, ulong width, ulong height) {
  ulong num_pixels;
  ulong j;
  ByteImagePixel *row;
  ByteImagePixel *pixels;
  ByteImagePixel **rows;
  ByteImage *this;

  if (width == 0) {
    throw("Width must be nonzero");
  }
  if (height== 0) {
    throw("Height must be nonzero");
  }

  this = newo(pthis);

  this->width = width;
  this->height = height;
  num_pixels = this->numPixels = width * height;

  pixels = new(&this->pixels, num_pixels);

  rows = new(&this->rows, height);
  for (j = 0, row = pixels; j < height; j++, row += width) {
    rows[j] = row;
  }

  byte_image_make_opaque(this);

  return this;
}

ByteImage *byte_image_new_from_double(ByteImage **pthis, DoubleImage *other) {
  bool alpha;
  ulong height;
  ulong i;
  ulong j;
  ulong width;
  double al;
  double **r;
  double **g;
  double **b;
  double **a;
  ByteImagePixel pixel;
  ByteImage *this;

  width = other->width;
  height = other->height;

  r = other->r;
  g = other->g;
  b = other->b;
  a = other->a;

  alpha = a != null;

  this = byte_image_new(pthis, width, height);

  if (alpha) {
    for (i = 0; i < width; i++) {
      for (j = 0; j < height; j++) {
        al = a[i][j];

        if (al == 0) {
          pixel.r = 0;
          pixel.g = 0;
          pixel.b = 0;
          pixel.a = 0;
        } else {
          pixel.r = byte_image_bound(r[i][j] / al * 255);
          pixel.g = byte_image_bound(g[i][j] / al * 255);
          pixel.b = byte_image_bound(b[i][j] / al * 255);
          pixel.a = byte_image_bound(al * 255);
        }

        byte_image_set(this, i, j, pixel);
      }
    }
  } else {
    pixel.a = 255;

    for (i = 0; i < width; i++) {
      for (j = 0; j < height; j++) {
        pixel.r = byte_image_bound(r[i][j] * 255);
        pixel.g = byte_image_bound(g[i][j] * 255);
        pixel.b = byte_image_bound(b[i][j] * 255);
        byte_image_set(this, i, j, pixel);
      }
    }
  }

  return this;
}

DoubleImage *double_image_new_from_byte(DoubleImage **pthis, ByteImage *other) {
  bool alpha;
  ulong height;
  ulong i;
  ulong j;
  ulong width;
  double al;
  double **r;
  double **g;
  double **b;
  double **a;
  ByteImagePixel pixel;
  DoubleImage *this;

  width = other->width;
  height = other->height;

  alpha = byte_image_contains_transparency(other);

  if (alpha) {
    this = double_image_new_rgba(pthis, width, height);
  } else {
    this = double_image_new_rgb(pthis, width, height);
  }

  r = this->r;
  g = this->g;
  b = this->b;

  if (alpha) {
    a = this->a;

    for (i = 0; i < width; i++) {
      for (j = 0; j < height; j++) {
        pixel = byte_image_get(other, i, j);
        al = pixel.a / 255.0;
        a[i][j] = al;
        r[i][j] = pixel.r * al / 255;
        g[i][j] = pixel.g * al / 255;
        b[i][j] = pixel.b * al / 255;
      }
    }
  } else {
    for (i = 0; i < width; i++) {
      for (j = 0; j < height; j++) {
        pixel = byte_image_get(other, i, j);
        r[i][j] = pixel.r / 255.0;
        g[i][j] = pixel.g / 255.0;
        b[i][j] = pixel.b / 255.0;
      }
    }
  }

  return this;
}

void byte_image_delete(ByteImage **pthis) {
  ByteImage *this;

  if ((this = *pthis) == null) {
    return;
  }

  deletev(&this->rows);
  deletev(&this->pixels);

  deleteo(pthis);
}

void byte_image_make_opaque(ByteImage *this) {
  ulong i;
  ulong j;

  for (i = 0; i < this->width; i++) {
    for (j = 0; j < this->height; j++) {
      set_a_(this, i, j, 255);
    }
  }
}

void byte_image_clear(ByteImage *this, ByteImagePixel pixel) {
  ulong i;
  ulong j;

  for (i = 0; i < this->width; i++) {
    for (j = 0; j < this->height; j++) {
      set_(this, i, j, pixel);
    }
  }
}

bool byte_image_contains_transparency(ByteImage *this) {
  ulong i;
  ulong j;

  for (i = 0; i < this->width; i++) {
    for (j = 0; j < this->height; j++) {
      if (get_(this, i, j).a != 255) {
        return true;
      }
    }
  }

  return false;
}

ByteImagePixel byte_image_get(ByteImage *this, ulong i, ulong j) {
  check_(this, i, j);
  return get_(this, i, j);
}

byte byte_image_get_r(ByteImage *this, ulong i, ulong j) {
  check_(this, i, j);
  return get_r_(this, i, j);
}

byte byte_image_get_g(ByteImage *this, ulong i, ulong j) {
  check_(this, i, j);
  return get_g_(this, i, j);
}

byte byte_image_get_b(ByteImage *this, ulong i, ulong j) {
  check_(this, i, j);
  return get_b_(this, i, j);
}

byte byte_image_get_a(ByteImage *this, ulong i, ulong j) {
  check_(this, i, j);
  return get_a_(this, i, j);
}

void byte_image_set(ByteImage *this, ulong i, ulong j, ByteImagePixel pixel) {
  check_(this, i, j);
  set_(this, i, j, pixel);
}

void byte_image_set_r(ByteImage *this, ulong i, ulong j, byte r) {
  check_(this, i, j);
  set_r_(this, i, j, r);
}

void byte_image_set_g(ByteImage *this, ulong i, ulong j, byte g) {
  check_(this, i, j);
  set_g_(this, i, j, g);
}

void byte_image_set_b(ByteImage *this, ulong i, ulong j, byte b) {
  check_(this, i, j);
  set_b_(this, i, j, b);
}

void byte_image_set_a(ByteImage *this, ulong i, ulong j, byte a) {
  check_(this, i, j);
  set_a_(this, i, j, a);
}

ByteImagePixel byte_image_pixel_from_rgba_hex(uint hex) {
  ByteImagePixel pixel;

  pixel.r = (hex & 0xff000000) >> 24;
  pixel.g = (hex & 0x00ff0000) >> 16;
  pixel.b = (hex & 0x0000ff00) >> 8;
  pixel.a = hex & 0x000000ff;

  return pixel;
}

ByteImagePixel byte_image_pixel_opaque_from_rgb_hex(uint hex) {
  ByteImagePixel pixel;

  if (hex > 0xffffff) {
    throw("Hex (0x%lx) too large: maximum for RGB is 0xffffff", hex);
  }

  pixel.r = (hex & 0xff0000) >> 16;
  pixel.g = (hex & 0x00ff00) >> 8;
  pixel.b = hex & 0x0000ff;
  pixel.a = 0xff;

  return pixel;
}

ByteImagePixel byte_image_pixel_opaque_black(void) {
  return byte_image_pixel_opaque_from_rgb_hex(0x000000);
}

bool byte_image_pixel_equals(ByteImagePixel x, ByteImagePixel y) {
  return x.r == y.r && x.g == y.g && x.b == y.b && x.a == y.a;
}

ByteImagePixel byte_image_pixel_from_hsl(double h, double s, double l) {
  double r;
  double g;
  double b;
  double p;
  double q;
  ByteImagePixel pixel;

  if (s == 0) {
    r = g = b = l;
  } else {
    q = l < 0.5 ? l * (1 + s) : l + s - l * s;
    p = 2 * l - q;
    r = hue_to_rgb_(p, q, h + 1.0 / 3);
    g = hue_to_rgb_(p, q, h);
    b = hue_to_rgb_(p, q, h - 1.0 / 3);
  }

  pixel.r = (byte)round(r * 255);
  pixel.g = (byte)round(g * 255);
  pixel.b = (byte)round(b * 255);
  pixel.a = 255;

  return pixel;
}

byte byte_image_bound(double v) {
  return v < 0.5 ? 0 : v >= 254.5 ? 255 : round(v);
}

ByteImageYccaPixel byte_image_ycca_pixel_from_rgba(ByteImagePixel pixel) {
  double r;
  double g;
  double b;
  ByteImageYccaPixel ycca;

  r = pixel.r;
  g = pixel.g;
  b = pixel.b;

  ycca.y = byte_image_bound(0.299 * r + 0.587 * g + 0.114 * b);
  ycca.cb = byte_image_bound(128 - 0.168736 * r - 0.331264 * g + 0.5 * b);
  ycca.cr = byte_image_bound(128 + 0.5 * r - 0.418688 * g - 0.081312 * b);
  ycca.a = pixel.a;

  return ycca;
}

ByteImagePixel byte_image_pixel_from_ycca(ByteImageYccaPixel ycca) {
  double y;
  double cb;
  double cr;
  ByteImagePixel pixel;

  y = ycca.y;
  cb = ycca.cb - 128.0;
  cr = ycca.cr - 128.0;

  pixel.r = byte_image_bound(y + 1.402 * cr);
  pixel.g = byte_image_bound(y - 0.344136 * cb - 0.714136 * cr);
  pixel.b = byte_image_bound(y + 1.772 * cb);
  pixel.a = ycca.a;

  return pixel;
}

static ByteImagePixel get_(ByteImage *this, ulong i, ulong j) {
  return this->rows[j][i];
}

static byte get_r_(ByteImage *this, ulong i, ulong j) {
  return this->rows[j][i].r;
}

static byte get_g_(ByteImage *this, ulong i, ulong j) {
  return this->rows[j][i].g;
}

static byte get_b_(ByteImage *this, ulong i, ulong j) {
  return this->rows[j][i].b;
}

static byte get_a_(ByteImage *this, ulong i, ulong j) {
  return this->rows[j][i].a;
}

static void set_(ByteImage *this, ulong i, ulong j, ByteImagePixel pixel) {
  this->rows[j][i] = pixel;
}

static void set_r_(ByteImage *this, ulong i, ulong j, byte r) {
  this->rows[j][i].r = r;
}

static void set_g_(ByteImage *this, ulong i, ulong j, byte g) {
  this->rows[j][i].g = g;
}

static void set_b_(ByteImage *this, ulong i, ulong j, byte b) {
  this->rows[j][i].b = b;
}

static void set_a_(ByteImage *this, ulong i, ulong j, byte a) {
  check_(this, i, j);
  this->rows[j][i].a = a;
}

static void check_(ByteImage *this, ulong i, ulong j) {
  if (i >= this->width) {
    throw("i (%lu) must be less than width (%lu)", i, this->width);
  }
  if (j >= this->height) {
    throw("j (%lu) must be less than height (%lu)", j, this->height);
  }
}

static double hue_to_rgb_(double p, double q, double t) {
  if (t < 0) {
    t += 1;
  }
  if (t > 1) {
    t -= 1;
  }
  if (t < 1.0 / 6) {
    return p + (q - p) * 6 * t;
  }
  if (t < 1.0 / 2) {
    return q;
  }
  if (t < 2.0 / 3) {
    return p + (q - p) * (2.0 / 3 - t) * 6;
  }
  return p;
}
