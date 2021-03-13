/* Internal representation of an RGBA image. Full intensity is defined to have
 * a value of 1.0. The A channel, if present, is premultiplied.
 */
#ifndef DOUBLE_IMAGE_H
#define DOUBLE_IMAGE_H

#ifdef __cplusplus
extern "C" {
#endif

#include "jpc.h"

typedef struct {
  ulong width;
  ulong height;
  double **r;
  double **g;
  double **b;
  double **a;
  double ***channels;
} DoubleImage;

/* Create a new image, specifying whether it has an alpha channel.
 */
DoubleImage *double_image_new(
    DoubleImage **pthis,
    ulong width,
    ulong height,
    bool alpha);

/* Create a new DoubleImage with no alpha channel, with all pixels set to black
 * (red, green, and blue set to 0).
 */
DoubleImage *double_image_new_rgb(
    DoubleImage **pthis,
    ulong width,
    ulong height);

/* Create a new DoubleImage with an alpha channel, with all pixels set to
 * opaque black (red, green, and blue set to 0; alpha set to 1).
 */
DoubleImage *double_image_new_rgba(
    DoubleImage **pthis,
    ulong width,
    ulong height);

/* Create a new DoubleImage that is a (deep) copy of another.
 */
DoubleImage *double_image_new_copy(DoubleImage **pthis, DoubleImage *other);

void double_image_delete(DoubleImage **pthis);
void double_image_print(DoubleImage *this, FILE *f, byte n);

/* Make the image fully opaque (the alpha for all pixels set to 1.0).
 */
void double_image_make_opaque(DoubleImage *this);

/* Set all pixels in an DoubleImage to the given values. The alpha value is
 * ignored if the image does not have an alpha channel.
 */
void double_image_clear(
    DoubleImage *this,
    double r,
    double g,
    double b,
    double a);

/* Whether the alpha channel contains any values that are not 1.0.
 */
bool double_image_contains_transparency(DoubleImage *this);


#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* _H */
