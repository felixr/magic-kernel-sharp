/* Generic RGBA images with eight bits per channel per pixel. The alpha
 * channel, if used, is "straight," i.e. not premultipled, as this is the
 * standard for eight-bit image file formats. Loading from or saving to PNG and
 * JPEG files is done in png_file and jpeg_file respectively. Libraries that
 * filter or resize these images first convert them to DoubleImages.
 */
#ifndef BYTE_IMAGE_H
#define BYTE_IMAGE_H

#ifdef __cplusplus
extern "C" {
#endif

#include "DoubleImage.h"

typedef struct {
  byte r;
  byte g;
  byte b;
  byte a;
} ByteImagePixel;

typedef struct {
  byte y;
  byte cb;
  byte cr;
  byte a;
} ByteImageYccaPixel;

typedef struct {
  ulong width;
  ulong height;
  ulong numPixels;
  ByteImagePixel *pixels;
  ByteImagePixel **rows;
} ByteImage;

/* Create a new image with all pixels set to opaque black (red, green, and blue
 * set to 0; alpha set to 255).
 */
ByteImage *byte_image_new(ByteImage **pthis, ulong width, ulong height);

/* Create a new ByteImage from a DoubleImage.
 */
ByteImage *byte_image_new_from_double(ByteImage **pthis, DoubleImage *other);

void byte_image_delete(ByteImage **pthis);

/* Create a new DoubleImage from a ByteImage.
 */
DoubleImage *double_image_new_from_byte(DoubleImage **pthis, ByteImage *other);

/* Make the image fully opaque (the alpha for all pixels set to 255).
 */
void byte_image_make_opaque(ByteImage *this);

/* Set all pixels in an image to the given pixel value.
 */
void byte_image_clear(ByteImage *this, ByteImagePixel pixel);

/* Accessors.
 */
ByteImagePixel byte_image_get(ByteImage *this, ulong i, ulong j);
byte byte_image_get_r(ByteImage *this, ulong i, ulong j);
byte byte_image_get_g(ByteImage *this, ulong i, ulong j);
byte byte_image_get_b(ByteImage *this, ulong i, ulong j);
byte byte_image_get_a(ByteImage *this, ulong i, ulong j);

void byte_image_set(ByteImage *this, ulong i, ulong j, ByteImagePixel pixel);
void byte_image_set_r(ByteImage *this, ulong i, ulong j, byte r);
void byte_image_set_g(ByteImage *this, ulong i, ulong j, byte g);
void byte_image_set_b(ByteImage *this, ulong i, ulong j, byte b);
void byte_image_set_a(ByteImage *this, ulong i, ulong j, byte a);

/* Whether the image contains any transparency. If it doesn't, many operations
 * are simplified.
 */
bool byte_image_contains_transparency(ByteImage *this);

/* Pixel functions.
 */
ByteImagePixel byte_image_pixel_from_rgba_hex(uint hex);
ByteImagePixel byte_image_pixel_opaque_from_rgb_hex(uint hex);
ByteImagePixel byte_image_pixel_opaque_black(void);
bool byte_image_pixel_equals(ByteImagePixel x, ByteImagePixel y);
ByteImagePixel byte_image_pixel_from_hsl(double h, double s, double l);
byte byte_image_bound(double v);

/* YCCA pixel conversions.
 */
ByteImageYccaPixel byte_image_ycca_pixel_from_rgba(ByteImagePixel pixel);
ByteImagePixel byte_image_pixel_from_ycca(ByteImageYccaPixel ycca);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* _H */
