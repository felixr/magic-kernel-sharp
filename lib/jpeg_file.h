/* Simple library for reading images from and writing images to standard JPEG
 * files.
 */
#ifndef JPEG_FILE_H
#define JPEG_FILE_H

#ifdef __cplusplus
extern "C" {
#endif

#include "File.h"
#include "ByteImage.h"

ByteImage *byte_image_new_load_jpeg(ByteImage **pthis, char *filename);
void byte_image_save_jpeg(ByteImage *this, char *filename, byte quality);

bool file_is_jpeg(char *filename);

/* Set the default quality to be used for saving. Returns the previous value.
 */
byte jpeg_file_set_default_quality(byte default_quality);

/* Get the current default quality setting.
 */
byte jpeg_file_get_default_quality(void);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* _H */
