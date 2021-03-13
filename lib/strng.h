/* See description in jpc.h.
 */
#ifndef STRNG_H
#define STRNG_H

#ifdef __cplusplus
extern "C" {
#endif

#include "jpc.h"
#include <stdarg.h>

/* Create a new empty string, correctly '\0'-terminated.
 */
char *string_new_empty(char **pthis);

/* Create a new string of length 1 that is a copy of a char.
 */
char *string_new_char(char **pthis, char c);

/* Create a new string that is a copy of another.
 */
char *string_new_copy(char **pthis, char *other);

/* Create a new string from a printf-like format specification.
 */
char *string_new_f(char **pthis, char *fmt, ...);

/* Create a new string from a printf-like format specification as va_list.
 */
char *string_new_vf(char **pthis, char *fmt, va_list ap1, va_list ap2);

/* Create a new string by concatenating two strings.
 */
char *string_new_concat(char **pthis, char *first, char *second);

/* Create a new string by concatenating any number of strings. Each vararg must
 * be of type char *. The last argument must be a null sentinel.
 */
char *string_new_concat_until_null(char **pthis, ...);

/* Create a new string by concatenating an array of strings.
 */
char *string_new_concat_array(char **pthis, ulong n, char **others);

/* Create a string by truncating another string to len characters. Fails with
 * error if len is greater than the length of the string.
 */
char *string_new_truncate(char **pthis, char *other, ulong len);

/* Create a string by shortening another string by n characters. Fails with
 * error if n is greater than the length of the string.
 */
char *string_new_shorten(char **pthis, char *other, ulong n);

/* Create a string by indenting another string by adding n spaces at the start
 * of every line.
 */
char *string_new_indent(char **pthis, char *other, ulong n);

/* Extracts the filename from a path.
 */
char *string_new_filename_from_path(char **pthis, char *path);

/* Same, but replacing the existing.
 */
char *string_renew_filename_from_path(char **pthis, char *path);

/* Standardize a string containing a directory by removing multiple consecutive
 * slashes and any trailing slash.
 */
char *string_renew_standardize_directory(char **pthis);

/* Extract the directory and filename from a path.
 */
void string_path_to_directory_filename(
    char *path,
    char **directory,
    char **filename);

/* Create a string that is a subset of another.
 */
char *string_new_substring(
    char **pthis,
    char *other,
    ulong first_index,
    ulong length);

/* Join an array of strings with the given string between them.
 */
char *string_new_join(char **pthis, ulong n, char **others, char *join);

/* Replace any of the characters in the first string with the replacement
 * character.
 */
char *string_new_replace_chars(
    char **pthis,
    char *other,
    char *find,
    char replace);

/* Create a new string, replacing the given char with the given string. To
 * escape the char, double it.
 */
char *string_new_replace_char_with_string(
    char **pthis,
    char *other,
    char find,
    char *replace);

/* Perform standard C-escaping on a string.
 */
char *string_new_escape(char **pthis, char *other);

/* Perform standard C-escaping on a single character.
 */
char *string_new_escape_char(char **pthis, char c);

/* Convenience function for deleting a string.
 */
void string_delete(char **pthis);

/* Renew the allocation of a string to be just sufficient for the actual length
 * of string contained within it.
 */
char *string_renew_trim(char **pthis);

/* Append any number of strings to an existing string, renewing its allocation.
 * Each vararg must be of type char *. The last argument must be a null
 * sentinel.
 */
char *string_renew_concat_until_null(char **pthis, ...);

/* Append a single string to another.
 */
char *string_renew_concat(char **pthis, char *other);

/* Append a single character to an existing string, renewing its allocation.
 */
char *string_renew_concat_char(char **pthis, char c);

/* Append a formatted string to a string, renewing its allocation.
 */
char *string_renew_concat_f(char **pthis, char *fmt, ...);

/* Append a formatted string to a string, from a va_list.
 */
char *string_renew_concat_vf(char **pthis, char *fmt, va_list ap1, va_list ap2);

/* Insert a string at the start of the string.
 */
char *string_renew_prepend(char **pthis, char *prefix);

/* Prepend with format.
 */
char *string_renew_prepend_f(char **pthis, char *fmt, ...);

/* If the string does not end with the given string, add it to the end.
 */
char *string_renew_ensure_ends_with(char **pthis, char *other);

/* Shorthand for deleting a string and creating a new one.
 */
char *string_renew_copy(char **pthis, char *other);

/* Shorten a string by n characters. Fails with error if n is greater than the
 * length of the string.
 */
char *string_renew_shorten(char **pthis, ulong n);

/* Indent a string by adding n spaces at the start of every line.
 */
char *string_renew_indent(char **pthis, ulong n);

/* Useful when a string is being used as a general object.
 */
void string_print(char *this, FILE *f, int n);

/* These functions are used by all _f functions, including those outside this
 * library, because vararg lists can't be rewound.
 */
ulong string_vf_get_max_len(char *fmt, va_list ap);
void string_vf_print_into(char *this, char *fmt, va_list ap, ulong max_len);

/* Whether the string contains any nonprintable characters.
 */
bool string_contains_nonprintable(char *this);

/* Whether the string contains the given character.
 */
bool string_contains_char(char *this, char c);

/* Useful for unit tests.
 */
ulong string_f_get_max_len(char *fmt, ...);

/* Whether a string starts or ends with a particular string.
 */
bool string_starts_with(char *this, char *other);
bool string_ends_with(char *this, char *other);

/* First occurrence of a character in a string, or -1 if not found.
 */
long string_first_index_of(char *this, char c);

/* n-th (zero-offset) occurrence of a character, or -1 if not found.
 */
long string_nth_index_of(char *this, ulong n, char c);

/* Last occurrence of a character in a string, or -1 if not found.
 */
long string_last_index_of(char *this, char c);

/* n-th-last (zero-offset) occurrence of a character, or -1 if not found.
 */
long string_nth_last_index_of(char *this, ulong n, char c);

/* More efficient than a general string comparison.
 */
bool string_is_empty(char *this);

/* Length of a string.
 */
ulong string_length(char *this);

/* Less obscure than a direct call to strcmp.
 */
bool string_equals(char *this, char *other);

/* Useful for qsortx.
 */
int string_compare(void *a, void *b, void *x);

/* Check that the substring [start, end) equals 'other'.
 */
bool string_substring_equals(char *this, ulong start, ulong end, char *other);

/* Whether the string contains the given string at the given index.
 */
bool string_contains_at(char *this, ulong index, char *other);

/* Get the strings obtained by splitting a string on a given character. Return
 * value is the number of splits.
 */
ulong string_split(char *this, char split_char, char ***psplits);

/* Check that string equals a previously set string, or copy it if it has not
 * yet been set. Returns true if the two strings agree, or if the string was
 * not previously set.
 */
bool string_check_or_set(char **pthis, char *other);

/* Put single quotes around a string, or else "(null)" if the pointer is null.
 */
char *string_quote_or_null(char **pthis, char *other);

/* Replace any of the characters in the first string with the replacement
 * character.
 */
char *string_replace_chars(char *this, char *find, char replace);

/* Create a new string that is a copy of a string, with line breaks inserted to
 * ensure that it fits within the given width. 'start' is the starting position
 * on the line (zero-offset), and 'indent' is how much to indent any lines
 * after the line break. Returns whether any wrapping occurred, rather than
 * this.
 */
bool string_new_copy_wrap(
    char **pthis,
    char *other,
    ulong start,
    ulong width,
    ulong indent);

/* Renew a string, wrapping it to fit.
 */
bool string_renew_wrap(
    char **pthis,
    ulong start,
    ulong width,
    ulong indent);

/* Append a broken string.
 */
bool string_renew_concat_wrap(
    char **pthis,
    char *other,
    ulong start,
    ulong width,
    ulong indent);

/* Create a string from printf-like format, wrapping it to fit.
 */
bool string_new_f_wrap(
    char **pthis,
    ulong start,
    ulong width,
    ulong indent,
    char *fmt,
    ...);

/* Append a wrapped string with printf.
 */
bool string_renew_concat_f_wrap(
    char **pthis,
    ulong start,
    ulong width,
    ulong indent,
    char *fmt,
    ...);

/* Make a new string that is a copy of the original string, but with any
 * decimal number of more than three digits, not following or followed by an
 * alphabetic character, replaced by its thousands-comma-separated equivalent.
 * The alphabetic restriction avoids most problems with e.g. hex numbers. This
 * function, and the next, are more convenient for "retrofitting" commas to the
 * output of log() and throw() without needing to create and destroy strings,
 * (since C89 doesn't have the thousands separator format mode, and this means
 * it wouldn't need to be explicitly used even if it did).
 */
char *string_new_commaize(char **pthis, char *other);

/* The same, in-place.
 */
char *string_renew_commaize(char **pthis);

/* Copy a string to an already-allocated block of memory. Used to avoid any
 * strcpy warnings. The length of the string (excluding final \0) must be no
 * greater than max_len. The pointer returns points to the final \0 written to
 * other, to allow writing of more to that buffer.
 */
char *string_copy(char *this, char *other, ulong max_len);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* _H */
