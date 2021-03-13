/* Integrate MemoryFile and ExternalFile into a common interface.
 */
#ifndef FILE_H
#define FILE_H

#ifdef __cplusplus
extern "C" {
#endif

#include "ExternalFile.h"
#include "MemoryFile.h"

typedef struct {
  MemoryFile *mf;
  ExternalFile *ef;
} File;

/* Retries: see ExternalFile.h. Ignored for memory files.
 */
byte file_set_retries(byte retries);
byte file_get_retries(void);

/* Constructor. If filename and mode are null, it's a MemoryFile.
 */
File *file_new(File **pthis, char *filename, char *mode);
bool file_try_new(File **pthis, char *filename, char *mode);

/* Constructors where the filename is created from a format string.
 */
File *file_new_f(File **pthis, char *mode, char *fmt, ...);
bool file_try_new_f(File **pthis, char *mode, char *fmt, ...);
File *file_new_vf(
    File **pthis,
    char *mode,
    char *fmt,
    va_list ap1,
    va_list ap2);
bool file_try_new_vf(
    File **pthis,
    char *mode,
    char *fmt,
    va_list ap1,
    va_list ap2);

/* Explicit constructors for a MemoryFile or an ExternalFile.
 */
File *file_new_memory_file(File **pthis);
File *file_new_external_file(
    File **pthis,
    char *filename,
    char *mode);
bool file_try_new_external_file(
    File **pthis,
    char *filename,
    char *mode);

/* Constructor, taking over an existing MemoryFile or an ExternalFile.
 */
File *file_new_from_memory_file(File **pthis, MemoryFile **pmf);
File *file_new_from_external_file(File **pthis, ExternalFile **pef);

/* Constructor for a reference MemoryFile of another MemoryFile.
 */
File *file_new_memory_reference(File **pthis, File *other);

/* Constructor, wrapping an ansifile.
 */
File *file_new_wrap_ansifile(File **pthis, FILE *ansifile, char *mode);

/* Constructor, loading an ExternalFile into a MemoryFile.
 */
File *file_new_memory_copy(
    File **pthis,
    char *other_filename);

/* Constructor, copying from a filename.
 */
File *file_new_copy(
    File **pthis,
    char *filename,
    char *mode,
    char *other_filename);

/* Constructor, copying another file.
 */
File *file_new_copy_other(
    File **pthis,
    char *filename,
    char *mode,
    File *other);

void file_delete(File **pthis);

void file_print(File *this, FILE *s, byte n, bool do_contents);

/* Accessors.
 */
bool file_is_memory_file(File *this);
bool file_is_external_file(File *this);

char *file_get_filename(File *this);

void file_write_byte(File *this, byte b);
void file_write_bytes(File *this, byte *bs, ulong n);

void file_write_char(File *this, char c);
void file_write_string(File *this, char *s);

bool file_try_read_byte(File *this, byte *b);
byte file_read_byte(File *this);
void file_read_bytes(File *this, byte *bs, ulong n);

ulong file_get_position(File *this);
bool file_is_eof(File *this);

void file_set_position(File *this, ulong position);
void file_rewind(File *this);
void file_fast_forward(File *this);
void file_backup_one_byte(File *this);

ulong file_get_size(File *this);

void file_flush(File *this);

/* Utilities.
 */
bool file_contents_equal_bytes(File *this, byte *bytes, ulong len);
void file_assert_contents_equal_bytes(File *this, byte *bytes, ulong len);

void file_copy_other(File *this, File *other);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* _H */
