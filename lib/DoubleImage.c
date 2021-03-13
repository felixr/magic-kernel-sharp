#include "DoubleImage.h"

DoubleImage *double_image_new(
    DoubleImage **pthis,
    ulong width,
    ulong height,
    bool alpha) {
  if (alpha) {
    return double_image_new_rgba(pthis, width, height);
  }

  return double_image_new_rgb(pthis, width, height);
}

DoubleImage *double_image_new_rgb(
    DoubleImage **pthis,
    ulong width,
    ulong height) {
  DoubleImage *this;

  if (width == 0) {
    throw("Width must be nonzero");
  }
  if (height== 0) {
    throw("Height must be nonzero");
  }

  this = newo(pthis);

  this->width = width;
  this->height = height;

  newm(&this->r, width, height);
  newm(&this->g, width, height);
  newm(&this->b, width, height);

  new(&this->channels, 4);
  this->channels[0] = this->r;
  this->channels[1] = this->g;
  this->channels[2] = this->b;

  return this;
}

DoubleImage *double_image_new_rgba(
    DoubleImage **pthis,
    ulong width,
    ulong height) {
  DoubleImage *this;

  this = double_image_new_rgb(pthis, width, height);

  newm(&this->a, width, height);
  this->channels[3] = this->a;

  double_image_make_opaque(this);

  return this;
}

DoubleImage *double_image_new_copy(DoubleImage **pthis, DoubleImage *other) {
  bool alpha;
  byte channel;
  ulong i;
  ulong j;
  DoubleImage *this;

  alpha = other->a != null;

  this = double_image_new(pthis, other->width, other->height, alpha);

  for (channel = 0; channel < (alpha ? 4 : 3); channel++) {
    for (i = 0; i < this->width; i++) {
      for (j = 0; j < this->height; j++) {
        this->channels[channel][i][j] = other->channels[channel][i][j];
      }
    }
  }

  return this;
}

void double_image_delete(DoubleImage **pthis) {
  byte channel;
  DoubleImage *this;

  if ((this = *pthis) == null) {
    return;
  }

  for (channel = 0; channel < 4; channel++) {
    deletemv(&this->channels[channel], this->width);
  }

  deletev(&this->channels);
  deleteo(pthis);
}

void double_image_print(DoubleImage *this, FILE *f, byte n) {
  byte channel;
  char *names[] = {"Red", "Green", "Blue", "Alpha"};
  ulong i;
  ulong j;

  fprintfn(f, n++, "[DoubleImage]");
  fprintfn(f, n, "width: %lu", this->width);
  fprintfn(f, n, "height: %lu", this->height);

  for (channel = 0; channel < 4; channel++) {
    fprintfn(f, n++, "%s channel:", names[channel]);

    for (j = 0; j < this->height; j++) {
      fprintfn(f, n++, "Row %lu:", j);

      for (i = 0; i < this->width; i++) {
        fprintfn(f, n, "[%lu] %g", i, this->channels[channel][i][j]);
      }

      n--;
    }

    n--;
  }
}

void double_image_make_opaque(DoubleImage *this) {
  ulong i;
  ulong j;

  if (this->a == null) {
    throw("Image does not have alpha channel");
  }

  for (i = 0; i < this->width; i++) {
    for (j = 0; j < this->height; j++) {
      this->a[i][j] = 1;
    }
  }
}

void double_image_clear(
    DoubleImage *this,
    double r,
    double g,
    double b,
    double a) {
  ulong i;
  ulong j;

  for (i = 0; i < this->width; i++) {
    for (j = 0; j < this->height; j++) {
      this->r[i][j] = r;
      this->g[i][j] = g;
      this->b[i][j] = b;

      if (this->a != null) {
        this->a[i][j] = a;
      }
    }
  }
}

bool double_image_contains_transparency(DoubleImage *this) {
  ulong i;
  ulong j;

  if (this->a == null) {
    return false;
  }

  for (i = 0; i < this->width; i++) {
    for (j = 0; j < this->height; j++) {
      if (this->a[i][j] != 1) {
        return true;
      }
    }
  }

  return false;
}
