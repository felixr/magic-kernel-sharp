/* Simple library for reading images from and writing images to standard PNG
 * files.
 */
#ifndef PNG_FILE_H
#define PNG_FILE_H

#ifdef __cplusplus
extern "C" {
#endif

#include "File.h"
#include "ByteImage.h"

ByteImage *byte_image_new_load_png(ByteImage **pthis, char *filename);
void byte_image_save_png(ByteImage *this, char *filename);

bool file_is_png(char *filename);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* _H */
