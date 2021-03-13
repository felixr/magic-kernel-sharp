/* Text facilities (records and fields) for Files.
 */
#ifndef TEXT_FILE_H
#define TEXT_FILE_H

#ifdef __cplusplus
extern "C" {
#endif

#include "File.h"
#include "numbers.h"

#define HIVE_NULL_FIELD_STRING "\\N"

typedef struct {
  bool justReadCR;
  char separator;
  char *record;
  char **fields;
  ulong maxNumRecordChars;
  ulong maxNumFields;
  ulong numFields;
  ulong currentFieldIndex;
  ulong numRecordChars;
  File *file;
} TextFile;

/* Retries: see ExternalFile.h.
 */
byte text_file_set_retries(byte retries);
byte text_file_get_retries(void);

/* Constructor, defaulting to NSV (see below).
 */
TextFile *text_file_new(TextFile **pthis, char *filename, char *mode);

/* Constructor for CSV files, with record separator set to newline and field
 * separator set to comma.
 */
TextFile *text_file_new_csv(TextFile **pthis, char *filename, char *mode);

/* Constructor for TSV files, with record separator set to newline and field
 * separator set to tab.
 */
TextFile *text_file_new_tsv(TextFile **pthis, char *filename, char *mode);

/* Constructor for HSV (Hive format) text files, with record separator set to
 * newline and field separator set to 0x01 (ctrl-a).
 */
TextFile *text_file_new_hsv(TextFile **pthis, char *filename, char *mode);

/* Constructor for NSV (null-separated values) text files, with record
 * separator set to newline and field separator set to '\0' (NUL). This allows
 * this library to be used to read and write text files for which no separation
 * into fields is desired, as '\0' is not a valid character in any string (it
 * terminates all strings). However, writing a record with more than one field
 * to such a file will indeed use '\0' as the field separator, and such a file
 * can be read back in, correctly separated into its constituent fields, using
 * this library.
 */
TextFile *text_file_new_nsv(TextFile **pthis, char *filename, char *mode);

/* Constructor, specifying type as "csv", "tsv", "hsv", "nsv", "txt", or null.
 * The last two are equivalent to "nsv".
 */
TextFile *text_file_new_type(
    TextFile **pthis,
    char *filename,
    char *mode,
    char *filetype);

/* Constructors for stdin.
 */
TextFile *text_file_new_stdin(TextFile **pthis);
TextFile *text_file_new_stdin_csv(TextFile **pthis);
TextFile *text_file_new_stdin_tsv(TextFile **pthis);
TextFile *text_file_new_stdin_hsv(TextFile **pthis);

/* Constructors for stdout.
 */
TextFile *text_file_new_stdout(TextFile **pthis);
TextFile *text_file_new_stdout_csv(TextFile **pthis);
TextFile *text_file_new_stdout_tsv(TextFile **pthis);
TextFile *text_file_new_stdout_hsv(TextFile **pthis);

/* Constructors for stderr.
 */
TextFile *text_file_new_stderr(TextFile **pthis);
TextFile *text_file_new_stderr_csv(TextFile **pthis);
TextFile *text_file_new_stderr_tsv(TextFile **pthis);
TextFile *text_file_new_stderr_hsv(TextFile **pthis);

/* Constructors for memory files.
 */
TextFile *text_file_new_memory(TextFile **pthis);
TextFile *text_file_new_memory_csv(TextFile **pthis);
TextFile *text_file_new_memory_tsv(TextFile **pthis);
TextFile *text_file_new_memory_hsv(TextFile **pthis);

/* Constructor for a reference MemoryFile of another MemoryFile.
 */
TextFile *text_file_new_memory_reference(TextFile **pthis, TextFile *other);

/* Constructor to load an ExternalFile into a MemoryFile.
 */
TextFile *text_file_new_memory_copy(
    TextFile **pthis,
    char *other_filename,
    char *filetype);
TextFile *text_file_new_csv_memory_copy(
    TextFile **pthis,
    char *other_filename);
TextFile *text_file_new_tsv_memory_copy(
    TextFile **pthis,
    char *other_filename);
TextFile *text_file_new_hsv_memory_copy(
    TextFile **pthis,
    char *other_filename);

bool text_file_try_new(
    TextFile **pthis,
    char *filename,
    char *mode);
bool text_file_try_new_csv(
    TextFile **pthis,
    char *filename,
    char *mode);
bool text_file_try_new_tsv(
    TextFile **pthis,
    char *filename,
    char *mode);
bool text_file_try_new_hsv(
    TextFile **pthis,
    char *filename,
    char *mode);

void text_file_delete(TextFile **pthis);

char *text_file_get_type(TextFile *this);

void text_file_set_csv(TextFile *this);
void text_file_set_tsv(TextFile *this);
void text_file_set_hsv(TextFile *this);
void text_file_set_type(TextFile *this, char *filetype);
void text_file_set_separator(TextFile *this, char separator);

void text_file_print(TextFile *this, FILE *s, byte n, bool do_contents);

/* Write a field.
 */
void text_file_write(TextFile *this, char *s);

/* Write a formatted field.
 */
void text_file_write_f(TextFile *this, char *fmt, ...);

/* Write a field from a specific type.
 */
void text_file_write_bool(TextFile *this, bool b);
void text_file_write_byte(TextFile *this, byte b);
void text_file_write_ulong(TextFile *this, ulong u);
void text_file_write_long(TextFile *this, long l);
void text_file_write_double(TextFile *this, double d);

/* Write an empty field.
 */
void text_file_write_empty_field(TextFile *this);

/* Write the end-of-record character.
 */
void text_file_end_record(TextFile *this);

/* Write an empty record. For parity with read methods.
 */
void text_file_write_empty_record(TextFile *this);

/* Write a single-field record.
 */
void text_file_write_single(TextFile *this, char *s);

/* Write a specific type of single-field record.
 */
void text_file_write_single_ulong(TextFile *this, ulong u);
void text_file_write_single_long(TextFile *this, long l);
void text_file_write_single_double(TextFile *this, double d);

/* Write a label record, i.e. a record with one field containing a label. For
 * parity with read methods.
 */
void text_file_write_label(TextFile *this, char *label);

/* Write a labeled record, i.e. a record with two fields, the first being a
 * label.
 */
void text_file_write_labeled(TextFile *this, char *label, char *s);
void text_file_write_labeled_bool(TextFile *this, char *label, bool b);
void text_file_write_labeled_byte(TextFile *this, char *label, byte b);
void text_file_write_labeled_ulong(TextFile *this, char *label, ulong u);

/* Create, write, and close a TextFile with one sole record with one single
 * field in it.
 */
void text_file_write_sole(char *filename, char *s);

/* Create, write, and close a TextFile with a specific type of sole record.
 */
void text_file_write_sole_ulong(char *filename, ulong u);

/* Read a record. Returns the number of fields. The 'fields' pointer is *not* a
 * new array; it is only a reference to an array inside the TextFile.
 */
ulong text_file_read(TextFile *this, char ***fields);

/* Read a record. Returns the number of fields. The fields pointer in this case
 * is created as a new array with a copy of the fields.
 */
ulong text_file_read_copy(TextFile *this, char ***pfields);

/* Try to read a record. Returns whether successful.
 */
bool text_file_try_read(TextFile *this, ulong *num_fields, char ***fields);

/* Return the number of characters in the last record read, including the
 * record terminator.
 */
ulong text_file_get_num_record_chars(TextFile *this);

/* Read and discard a record.
 */
void text_file_read_discard(TextFile *this);

/* Read an empty record.
 */
void text_file_read_empty_record(TextFile *this);

/* Try to read a record from src; if we read one, write it out to dst.
 */
bool text_file_try_copy(TextFile *src, TextFile *dst);

/* Read a single-field record. Error exit if there is more than one field. An
 * empty record is considered to have one empty-string field, as these are
 * indistinguishable for text files.
 */
char *text_file_read_single(TextFile *this);
bool text_file_try_read_single(TextFile *this, char **s);

/* Read a specific type from a single-field record.
 */
ulong text_file_read_single_ulong(TextFile *this);
bool text_file_try_read_single_ulong(TextFile *this, ulong *u);

/* Read a label record, i.e., read the label and confirm that it is the
 * expected string.
 */
void text_file_read_label(TextFile *this, char *label);

/* Read a labeled field.
 */
char *text_file_read_labeled(TextFile *this, char *label);
bool text_file_read_labeled_bool(TextFile *this, char *label);
byte text_file_read_labeled_byte(TextFile *this, char *label);
ulong text_file_read_labeled_ulong(TextFile *this, char *label);

/* Open, read, and close a TextFile, reading one sole record with one single
 * field in it.
 */
char *text_file_read_sole(char *filename, char **s);

/* Open, read, and close a TextFile, reading a specific type of sole record.
 */
ulong text_file_read_sole_ulong(char *filename);

/* Low-level method: Merge consecutive fields by replacing the '\0' between
 * them with the specified separator. This only works because this class stores
 * the record as a single string, and simply replaces the original field
 * separator with '\0' and sets up pointers to the start of each field. The
 * original field pointers are unchanged, so the first will point to the merged
 * string, and successive ones will drop one field from the left each time.
 */
void text_file_merge_fields(
    TextFile *this,
    ulong first_field_index,
    ulong last_field_index,
    char merged_field_separator);

/* Rewind the TextFile.
 */
void text_file_rewind(TextFile *this);

/* The number of records in the TextFile.
 */
ulong text_file_num_records(TextFile *this);

/* Copy the contents of the source TextFile (from where it currently is) to the
 * destination (which may already have some contents in it).
 */
void text_file_copy_remaining(TextFile *src, TextFile *dst);

/* Whether two TextFiles have the same contents.
 */
bool text_file_contents_equal(TextFile *this, TextFile *other);

/* The filename of the TextFile, or null if it is a MemoryFile.
 */
char *text_file_get_filename(TextFile *this);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* _H */
