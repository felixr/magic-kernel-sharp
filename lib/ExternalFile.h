#ifndef EXTERNAL_FILE_H
#define EXTERNAL_FILE_H

#ifdef __cplusplus
extern "C" {
#endif

#include "bits.h"
#include <zlib.h>

typedef struct {
  boolbit wrap: 1;
  boolbit read: 1;
  boolbit write: 1;
  boolbit append: 1;
  boolbit update: 1;
  boolbit text: 1;
  char *filename;
  char *mode;
  char *partialFilename;
  FILE *ansifile;
  gzFile gzfile;
} ExternalFile;

#include "File.h"

/* Retries are useful with some filesystems that fail on first attempt to open
 * a file. This sets a global number of retries that is used for all external
 * file operations, with exponential backoff. Setter returns the previous value.
 */
byte external_file_set_retries(byte retries);
byte external_file_get_retries(void);

/* Constructor for any ANSI mode. Shortcut: to use stdin, stdout or sterrr, set
 * 'filename' to null and 'mode' to 'stdin', 'stdout' or 'stderr'.
 * Alternatively, if filename is '-', then stdin or stdout will be used,
 * depending on whether mode is read or write.
 */
ExternalFile *external_file_new(
    ExternalFile **pthis,
    char *filename,
    char *mode);
bool external_file_try_new(
    ExternalFile **pthis,
    char *filename,
    char *mode);

/* Constructor that wraps an existing ANSI FILE. Does not close it when the
 * File instance is destroyed. Useful for stdin, stdout, stderr (called from
 * the above shortcuts).
 */
ExternalFile *external_file_new_wrap_ansifile(
    ExternalFile **pthis,
    FILE *ansifile,
    char *mode);

void external_file_delete(ExternalFile **pthis);

void external_file_print(
    ExternalFile *this,
    FILE *s,
    byte n,
    bool do_contents);

bool external_file_is_gzipped(ExternalFile *this);

void external_file_write_byte(ExternalFile *this, byte b);

bool external_file_try_read_byte(ExternalFile *this, byte *b);
byte external_file_read_byte(ExternalFile *this);

ulong external_file_get_size(ExternalFile *this);

ulong external_file_get_position(ExternalFile *this);
bool external_file_is_eof(ExternalFile *this);

void external_file_set_position(ExternalFile *this, ulong position);
void external_file_rewind(ExternalFile *this);
void external_file_fast_forward(ExternalFile *this);

void external_file_flush(ExternalFile *this);

bool external_file_contents_equal_bytes(
    ExternalFile *this,
    byte *bytes,
    ulong len);
void external_file_assert_contents_equal_bytes(
    ExternalFile *this,
    byte *bytes,
    ulong len);

/* These act on filenames, rather than ExternalFiles, as they're intended to be
 * called before opening an ExternalFile.
 */
bool external_file_exists(char *filename);
bool external_file_exists_wait(char *filename, byte max_seconds);
bool external_file_partial_exists(char *filename);
bool external_file_either_exists(char *filename);
bool external_file_is_empty(char *filename);
bool external_file_is_writeable(char *filename);
void external_file_create(char *filename);
bool external_file_try_remove(char *filename);
void external_file_remove(char *filename);
void external_file_remove_partial(char *filename);
void external_file_remove_both(char *filename);
void external_file_rename(char *orig_filename, char *new_filename);


/* From just a filename, read the entire contents of an external file into a
 * new byte blob. Returns the length of the file (and blob) in bytes.
 */
ulong external_file_read_all(char *filename, byte **pblob);

/* From just a filename, read the entire contents of an external file into a
 * string.
 */
char *external_file_read_all_into_string(char *filename, char **ps);

/* The status of an external file. Returns -1 if neither the file nor its
 * partial file exist; 0 if the partial file exists; +1 if the file exists.
 * These represent the three stages of processing (not started; processing;
 * completed).
 */
int external_file_status(char *filename);

/* The status of a set of external files. Returns -1 if none of the files or
 * their partial files exist; +1 if all of the files exist; 0 otherwise.
 */
int external_files_status(ulong num_filenames, char **filenames);

/* Useful functions for unit tests to verify the contents of ANSI FILEs.
 */
bool ansifile_contents_equal_bytes(
    FILE *file,
    byte *expected_contents,
    ulong len);
bool ansifile_contents_equal_bytes_from_filename(
    char *filename,
    byte *expected_contents,
    ulong len);
bool ansifile_contents_equal_string(
    FILE *file,
    char *expected_contents);
bool ansifile_contents_equal_string_from_filename(
    char *filename,
    char *expected_contents);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* _H */
