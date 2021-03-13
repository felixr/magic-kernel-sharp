#include "strng.h"
#include <ctype.h>
#include <math.h>
#include <stdarg.h>
#include <string.h>

static int exponent_(double d);

char *string_new_empty(char **pthis) {
  char *this;

  this = newo(pthis);
  *this = '\0';

  return this;
}

char *string_new_char(char **pthis, char c) {
  char *this;

  this = new(pthis, 2);
  this[0] = c;
  this[1] = '\0';

  return this;
}

char *string_new_copy(char **pthis, char *other) {
  char *this;
  ulong len;

  len = string_length(other);
  this = new(pthis, len + 1);
  string_copy(other, this, len);

  return this;
}

char *string_new_f(char **pthis, char *fmt, ...) {
  char *this;
  va_list ap1;
  va_list ap2;

  va_start(ap1, fmt);
  va_start(ap2, fmt);
  this = string_new_vf(pthis, fmt, ap1, ap2);
  va_end(ap1);
  va_end(ap2);

  return this;
}

char *string_new_vf(char **pthis, char *fmt, va_list ap1, va_list ap2) {
  char *this;
  ulong max_len;

  max_len = string_vf_get_max_len(fmt, ap1);

  this = new(pthis, max_len + 1);

  string_vf_print_into(this, fmt, ap2, max_len);

  this = string_renew_trim(pthis);

  return this;
}

char *string_new_concat(char **pthis, char *first, char *second) {
  return string_new_concat_until_null(pthis, first, second, null);
}

char *string_new_concat_until_null(char **pthis, ...) {
  char *this;
  char *s;
  char *p;
  char *e;
  ulong len;
  va_list ap;

  len = 0;

  va_start(ap, pthis);
  while ((s = va_arg(ap, char *)) != null) {
    len += string_length(s);
  }
  va_end(ap);

  this = new(pthis, len + 1);

  va_start(ap, pthis);
  for (p = this, e = p + len; (s = va_arg(ap, char *)) != null;) {
    p = string_copy(s, p, e - p);
  }
  va_end(ap);

  return this;
}

char *string_new_concat_array(char **pthis, ulong n, char **others) {
  char *this;
  char *e;
  char *p;
  ulong len;
  ulong u;

  len = 0;

  for (u = 0; u < n; u++) {
    len += string_length(others[u]);
  }

  this = new(pthis, len + 1);

  for (u = 0, p = this, e = p + len; u < n; u++) {
    p = string_copy(others[u], p, e - p);
  }

  return this;
}

char *string_new_truncate(char **pthis, char *other, ulong len) {
  ulong other_len;
  char *this;

  other_len = string_length(other);

  if (len > other_len) {
    throw(
        "Cannot truncate string '%s' to a length of %lu character%s",
        other,
        len,
        plural_s(len));
  }

  this = new(pthis, len + 1);
  memcpy(this, other, len);
  this[len] = '\0';

  return this;
}

char *string_new_shorten(char **pthis, char *other, ulong n) {
  ulong other_len;
  ulong len;
  char *this;

  other_len = string_length(other);

  if (n > other_len) {
    throw(
        "Cannot shorten string '%s' by %lu character%s",
        other,
        n,
        plural_s(n));
  }

  len = other_len - n;
  this = new(pthis, len + 1);
  memcpy(this, other, len);
  this[len] = '\0';

  return this;
}

char *string_new_indent(char **pthis, char *other, ulong n) {
  string_new_copy(pthis, other);
  return string_renew_indent(pthis, n);
}

char *string_new_filename_from_path(char **pthis, char *path) {
  long l;

  l = string_last_index_of(path, '/');
  return string_new_copy(pthis, path + (l < 0 ? 0 : l + 1));
}

char *string_renew_filename_from_path(char **pthis, char *path) {
  string_delete(pthis);
  return string_new_filename_from_path(pthis, path);
}

char *string_renew_standardize_directory(char **pthis) {
  bool had;
  char c;
  char *p;
  char *q;
  char *this;
  char *s = null;

  this = *pthis;
  new(&s, string_length(this) + 1);

  for (p = this, q = s, had = false; (c = *p) != '\0'; p++) {
    if (had) {
      if (c == '/') {
        continue;
      }

      had = false;
    } else if (c == '/') {
      had = true;
    }

    *q++ = c;
  }

  if (had && q - s > 1) {
    q--;
  }

  *q = '\0';

  string_renew_trim(&s);
  string_delete(pthis);
  donate(&s, pthis);
  return *pthis;
}

void string_path_to_directory_filename(
    char *path,
    char **directory,
    char **filename) {
  long l;

  l = string_last_index_of(path, '/');

  if (l < 0) {
    string_new_empty(directory);
    string_new_copy(filename, path);
  } else {
    string_new_truncate(directory, path, maximum(l, 1));
    string_new_copy(filename, path + l + 1);
  }

  string_renew_standardize_directory(directory);
}

char *string_new_substring(
    char **pthis,
    char *other,
    ulong first_index,
    ulong length) {
  char *p;
  char *q;
  char *e;
  char *this;

  if (first_index + length > string_length(other)) {
    throw("First index %lu, length %lu invalid for '%s' (length %lu)",
        first_index,
        length,
        other,
        string_length(other));
  }

  this = new(pthis, length + 1);

  for (
      p = this, q = other + first_index, e = q + length;
      q < e;
      p++, q++) {
    *p = *q;
  }
  *p = '\0';
  return this;
}

char *string_new_join(char **pthis, ulong n, char **others, char *join) {
  char *this;
  char *e;
  char *p;
  ulong u;
  ulong len;
  ulong join_len;

  if (n == 0) {
    return string_new_empty(pthis);
  }

  join_len = n > 1 ? string_length(join) : 0;

  for (u = 0, len = (n - 1) * join_len; u < n; u++) {
    len += string_length(others[u]);
  }

  this = new(pthis, len + 1);

  for (u = 0, p = this, e = p + len; u < n; u++) {
    if (u > 0) {
      p = string_copy(join, p, e - p);
    }
    p = string_copy(others[u], p, e - p);
  }

  return this;
}

char *string_new_replace_chars(
    char **pthis,
    char *other,
    char *find,
    char replace) {
  char *this;

  this = string_new_copy(pthis, other);
  string_replace_chars(this, find, replace);
  return this;
}

char *string_new_replace_char_with_string(
    char **pthis,
    char *other,
    char find,
    char *replace) {
  char *this;
  char c;
  char *p;
  char *q;
  char *r;
  char *t;
  ulong len;

  string_new_empty(pthis);

  for (p = other;; p = q) {
    for (q = p; (c = *q) != '\0' && c != find; q++) {
    }
    this = *pthis;
    for (r = this; *r != '\0'; r++) {
    }
    len = r - this;
    renewup(pthis, len + 1, len + q - p + 1);
    for (r = *pthis + len, t = p; t < q; t++, r++) {
      *r = *t;
    }
    *r = '\0';
    if (c == '\0') {
      break;
    }
    q++;
    if (*q == find) {
      string_renew_concat_char(pthis, find);
      q++;
    } else {
      string_renew_concat(pthis, replace);
    }
  }

  return *pthis;
}

char *string_new_escape(char **pthis, char *other) {
  char c;
  char *p;

  string_new_empty(pthis);
  for (p = other; (c = *p) != '\0'; p++) {
    switch(c) {
      case '\a':
        string_renew_concat(pthis, "\\a");
        break;
      case '\b':
        string_renew_concat(pthis, "\\b");
        break;
      case '\f':
        string_renew_concat(pthis, "\\f");
        break;
      case '\n':
        string_renew_concat(pthis, "\\n");
        break;
      case '\r':
        string_renew_concat(pthis, "\\r");
        break;
      case '\t':
        string_renew_concat(pthis, "\\t");
        break;
      case '\v':
        string_renew_concat(pthis, "\\v");
        break;
      case '\\':
        string_renew_concat(pthis, "\\\\");
        break;
      case '\'':
        string_renew_concat(pthis, "\\'");
        break;
      case '"':
        string_renew_concat(pthis, "\\\"");
        break;
      default:
        if (iscntrl(c) || (byte)c >= 0x80) {
          string_renew_concat_f(pthis, "\\x%02x", (byte)c);
        } else {
          string_renew_concat_char(pthis, c);
        }
        break;
    }
  }
  return *pthis;
}

char *string_new_escape_char(char **pthis, char c) {
  char s[2];

  s[0] = c;
  s[1] = '\0';
  return string_new_escape(pthis, s);
}

void string_delete(char **pthis) {
  deletev(pthis);
}

void string_print(char *this, FILE *f, int n) {
  char c;
  char *p;
  char *s1 = null;
  char *s2 = null;
  char *s3 = null;
  char *s4 = null;

  fprintfn(f, n++, "[String]");
  if (string_contains_nonprintable(this)) {
    fprintfn(f, n, "Length: %lu", string_length(this));
    fprintfn(f, n, "String contains nonprintable characters");
    string_new_empty(&s1);
    string_new_empty(&s2);
    string_new_escape(&s3, this);
    string_new_empty(&s4);
    for (p = this; (c = *p) != '\0'; p++) {
      if (isprint(c)) {
        string_renew_concat_char(&s1, c);
        string_renew_concat_char(&s2, c);
      } else {
        string_renew_concat_char(&s2, '.');
      }
      if (p != this) {
        string_renew_concat_char(&s4, ' ');
      }
      string_renew_concat_f(&s4, "%02x", (byte)c);
    }
    fprintfn(f, n, "Nonprintable removed: '%s'", s1);
    fprintfn(f, n, "Nonprintable replaced by dots: '%s'", s2);
    fprintfn(f, n, "Escaped: '%s'", s3);
    fprintfn(f, n, "Hex: %s", s4);
    string_delete(&s4);
    string_delete(&s3);
    string_delete(&s2);
    string_delete(&s1);
  } else {
    fprintfn(f, n, "Value: '%s'", this);
  }
}

bool string_contains_nonprintable(char *this) {
  char c;
  char *p;

  for (p = this; (c = *p) != '\0'; p++) {
    if (!isprint(c)) {
      return true;
    }
  }
  return false;
}

bool string_contains_char(char *this, char c) {
  char d;
  char *p;

  for (p = this; (d = *p) != '\0'; p++) {
    if (d == c) {
      return true;
    }
  }
  return false;
}

char *string_renew_trim(char **pthis) {
  ulong len;

  len = string_length(*pthis);
  renewv(pthis, len + 1, len + 1);

  return *pthis;
}

char *string_renew_concat_until_null(char **pthis, ...) {
  char *this;
  char *s;
  char *p;
  char *e;
  ulong old_len;
  ulong len;
  va_list ap;

  old_len = string_length(*pthis);

  va_start(ap, pthis);
  for (len = old_len; (s = va_arg(ap, char *)) != null;) {
    len += string_length(s);
  }
  va_end(ap);

  this = renewup(pthis, old_len + 1, len + 1);

  va_start(ap, pthis);
  for (p = this + old_len, e = this + len; (s = va_arg(ap, char*)) != null;) {
    p = string_copy(s, p, e - p);
  }
  va_end(ap);

  return this;
}

char *string_renew_concat(char **pthis, char *other) {
  return string_renew_concat_until_null(pthis, other, null);
}

char *string_renew_concat_char(char **pthis, char c) {
  char *this;
  ulong len;

  len = string_length(*pthis);
  this = renewup(pthis, len + 1, len + 2);
  this[len] = c;
  this[len + 1] = '\0';

  return this;
}

char *string_renew_concat_f(char **pthis, char *fmt, ...) {
  char *this;
  va_list ap1;
  va_list ap2;

  va_start(ap1, fmt);
  va_start(ap2, fmt);
  this = string_renew_concat_vf(pthis, fmt, ap1, ap2);
  va_end(ap1);
  va_end(ap2);

  return this;
}

char *string_renew_concat_vf(
    char **pthis,
    char *fmt,
    va_list ap1,
    va_list ap2) {
  char *this;
  ulong old_len;
  ulong len;

  old_len = string_length(*pthis);

  len = string_vf_get_max_len(fmt, ap1);

  this = renewup(pthis, old_len + 1, old_len + len + 1);

  string_vf_print_into(this + old_len, fmt, ap2, len);

  return this;
}

char *string_renew_prepend(char **pthis, char *prefix) {
  char *src;
  char *dst;
  char *this;
  ulong old_len;
  ulong new_len;
  ulong prefix_len;

  old_len = string_length(*pthis);
  prefix_len = string_length(prefix);
  new_len = old_len + prefix_len;

  this = renewup(pthis, old_len + 1, new_len + 1);

  for (src = this + old_len, dst = this + new_len; src >= this; src--, dst--) {
    *dst = *src;
  }
  for (src = prefix + prefix_len - 1; src >= prefix; src--, dst--) {
    *dst = *src;
  }

  return this;
}

char *string_renew_prepend_f(char **pthis, char *fmt, ...) {
  char *final = null;
  char *prepend = null;
  ulong len;
  va_list ap;

  va_start(ap, fmt);
  len = string_vf_get_max_len(fmt, ap);
  va_end(ap);

  new(&prepend, len + 1);

  va_start(ap, fmt);
  string_vf_print_into(prepend, fmt, ap, len);
  va_end(ap);

  string_new_concat_until_null(&final, prepend, *pthis, null);

  string_delete(pthis);
  string_delete(&prepend);

  return donate(&final, pthis);
}

char *string_renew_ensure_ends_with(char **pthis, char *other) {
  if (string_ends_with(*pthis, other)) {
    return *pthis;
  }
  return string_renew_concat(pthis, other);
}

char *string_renew_copy(char **pthis, char *other) {
  string_delete(pthis);
  return string_new_copy(pthis, other);
}

char *string_renew_shorten(char **pthis, ulong n) {
  ulong len;

  len = string_length(*pthis);
  if (n > len) {
    throw(
        "Cannot shorten string '%s' by %lu character%s",
        *pthis,
        n,
        plural_s(n));
  }
  renewv(pthis, len + 1, len + 1 - n);
  (*pthis)[len - n] = '\0';
  return *pthis;
}

char *string_renew_indent(char **pthis, ulong n) {
  char c;
  char *this;
  char *p;
  char *src;
  char *dst;
  ulong num_lines;
  ulong old_len;
  ulong new_len;
  ulong u;

  if (n == 0) {
    return *pthis;
  }

  for (p = *pthis, num_lines = 1; (c = *p) != '\0'; p++) {
    if (c == '\n') {
      num_lines++;
    }
  }

  old_len = p - *pthis;
  new_len = old_len + n * num_lines;

  this = renewup(pthis, old_len + 1, new_len + 1);

  for (src = this + old_len, dst = this + new_len; src >= this; src--) {
    if ((c = *src) == '\n') {
      for (u = 0; u < n; u++) {
        *dst-- = ' ';
      }
    }
    *dst-- = c;
  }
  for (u = 0; u < n; u++) {
    *dst-- = ' ';
  }

  if (dst + 1 != this) {
    throw("impossible");
  }

  return this;
}

ulong string_vf_get_max_len(char *fmt, va_list ap) {
  bool done_left;
  bool done_sign;
  bool done_space;
  bool done_zero;
  bool done_hash;
  bool finished_flags;
  bool short_integer;
  bool long_integer;
  bool long_double;
  char c;
  char *p;
  int exponent;
  int precision;
  ulong min_width;
  ulong max_len;
  ulong width = 0;
  double d;
  long double ld;

  max_len = 0;
  for (p = fmt; (c = *p) != '\0'; p++) {
    min_width = 0;
    if (c == '%') {
      if ((c = *++p) == '%') {
        width = 1;
      } else {
        done_left = false;
        done_sign = false;
        done_space = false;
        done_zero = false;
        done_hash = false;
        finished_flags = false;
        for (;;) {
          switch (c) {
            case '-':
              if (done_left) {
                throw("'-' was already specified in format string");
              }
              done_left = true;
              break;
            case '+':
              if (done_sign) {
                throw("'+' was already specified in format string");
              }
              if (done_space) {
                throw("Can't specify both '+' and ' ' in format string");
              }
              done_sign = true;
              break;
            case ' ':
              if (done_space) {
                throw("' ' was already specified in format string");
              }
              if (done_sign) {
                throw("Can't specify both '+' and ' ' in format string");
              }
              done_space = true;
              break;
            case '0':
              if (done_zero) {
                throw("'0' was already specified in format string");
              }
              done_zero = true;
              break;
            case '#':
              if (done_hash) {
                throw("'#' was already specified in format string");
              }
              done_hash = true;
              break;
            default:
              finished_flags = true;
              break;
          }
          if (finished_flags) {
            break;
          }
          c = *++p;
        }
        if (c == '*') {
          min_width = va_arg(ap, int);
          c = *++p;
        } else if (isdigit(c)) {
          min_width = 0;
          while (isdigit(c)) {
            min_width *= 10;
            min_width += c - '0';
            c = *++p;
          }
        }
        if (c == '.') {
          c = *++p;
        }
        precision = -1;
        if (c == '*') {
          precision = va_arg(ap, int);
          c = *++p;
        } else if (isdigit(c)) {
          precision = 0;
          while (isdigit(c)) {
            precision *= 10;
            precision += c - '0';
            c = *++p;
          }
        }
        short_integer = false;
        long_integer = false;
        long_double = false;
        if (c == 'h') {
          short_integer = true;
          c = *++p;
        } else if (c == 'l') {
          long_integer = true;
          c = *++p;
        } else if (c == 'L') {
          long_double = true;
          c = *++p;
        }
        switch (c) {
          case 'd':
          case 'i':
          case 'u':
            if (long_double) {
              throw("Format 'L%c' is invalid", c);
            }
            if (short_integer) {
              width = 6;
              va_arg(ap, int);
            } else if (long_integer) {
              width = 21;
              va_arg(ap, long);
            } else {
              width = 11;
              va_arg(ap, int);
            }
            break;
          case 'o':
            if (long_double) {
              throw("Format 'L%c' is invalid", c);
            }
            if (short_integer) {
              width = 7;
              va_arg(ap, int);
            } else if (long_integer) {
              width = 23;
              va_arg(ap, long);
            } else {
              width = 12;
              va_arg(ap, int);
            }
            if (done_hash) {
              width++;
            }
            break;
          case 'x':
          case 'X':
            if (long_double) {
              throw("Format 'L%c' is invalid", c);
            }
            if (short_integer) {
              width = 4;
              va_arg(ap, int);
            } else if (long_integer) {
              width = 16;
              va_arg(ap, long);
            } else {
              width = 8;
              va_arg(ap, int);
            }
            if (done_hash) {
              width += 2;
            }
            break;
          case 'c':
            width = 1;
            va_arg(ap, int);
            break;
          case 's':
            width = string_length(va_arg(ap, char *));
            if (precision >= 0 && width > precision) {
              width = precision;
            }
            break;
          case 'f':
            if (short_integer) {
              throw("Format 'h%c' is invalid", c);
            }
            if (long_integer) {
              throw("Format 'l%c' is invalid", c);
            }
            if (precision < 0) {
              precision = 6;
            }
            if (long_double) {
              ld = va_arg(ap, long double);
              exponent = exponent_(ld);
              width = precision + 2 + (exponent > 0 ? exponent + 1 : 1);
            } else {
              d = va_arg(ap, double);
              exponent = exponent_(d);
              width = precision + 2 + (exponent > 0 ? exponent + 1 : 1);
            }
            break;
          case 'e':
          case 'E':
            if (precision < 0) {
              precision = 6;
            }
            width = 9 + precision;
            if (long_double) {
              va_arg(ap, long double);
            } else {
              va_arg(ap, double);
            }
            break;
          case 'g':
          case 'G':
            if (precision < 0) {
              precision = 6;
            }
            if (long_double) {
              ld = va_arg(ap, long double);
              exponent = exponent_(ld);
            } else {
              d = va_arg(ap, double);
              exponent = exponent_(d);
            }
            if (exponent < -4 || exponent >= precision) {
              width = 9 + precision;
            } else {
              width = precision + 2 + (exponent > 0 ? exponent + 1 : 1)
                + (exponent < -2 ? -2 - exponent : 0);
            }
            break;
          case 'p':
            width = 32;
            va_arg(ap, void *);
            break;
          case 'n':
            width = 0;
            min_width = 0;
            va_arg(ap, int *);
            break;
          default:
            throw(
                "Character '%c' at index %lu of format string '%s' "
                "is invalid",
                c,
                p - fmt,
                fmt);
        }
      }
      if (width < min_width) {
        width = min_width;
      }
      max_len += width;
    } else {
      max_len++;
    }
  }

  return max_len;
}

void string_vf_print_into(char *this, char *fmt, va_list ap, ulong max_len) {
  ulong len_done;

  if ((len_done = vsprintf(this, fmt, ap)) > max_len) {
    throw(
        "Length asked to be printed %lu; exceeded allocated %lu.",
        len_done,
        max_len);
  }
}

ulong string_f_get_max_len(char *fmt, ...) {
  ulong max_len;
  va_list ap;

  va_start(ap, fmt);
  max_len = string_vf_get_max_len(fmt, ap);
  va_end(ap);

  return max_len;
}

bool string_starts_with(char *this, char *other) {
  char *t;
  char *o;

  for (t = this, o = other; *t != '\0' && *o != '\0' && *t == *o; t++, o++) {
  }

  return *o == '\0';
}

bool string_ends_with(char *this, char *other) {
  char *t;
  char *o;

  for (t = this + string_length(this) - 1, o = other + string_length(other) - 1;
      t >= this && o >= other && *t == *o;
      t--, o--) {
  }

  return o < other;
}

long string_first_index_of(char *this, char c) {
  return string_nth_index_of(this, 0, c);
}

long string_nth_index_of(char *this, ulong n, char c) {
  char *p;
  char *e;
  ulong k;

  for (k = 0, p = this, e = p + string_length(this); p < e; p++) {
    if (*p == c) {
      if (k++ == n) {
        return p - this;
      }
    }
  }
  return -1;
}


long string_last_index_of(char *this, char c) {
  return string_nth_last_index_of(this, 0, c);
}

long string_nth_last_index_of(char *this, ulong n, char c) {
  char *p;
  ulong k;

  for (k = 0, p = this + string_length(this) - 1; p >= this; p--) {
    if (*p == c) {
      if (k++ == n) {
        return p - this;
      }
    }
  }
  return -1;
}

bool string_is_empty(char *this) {
  return *this == '\0';
}

ulong string_length(char *this) {
  return strlen(this);
}

bool string_equals(char *this, char *other) {
  return strcmp(this, other) == 0;
}

int string_compare(void *a, void *b, void *x) {
  return strcmp((char *)a, (char *)b);
}

bool string_substring_equals(char *this, ulong start, ulong end, char *other) {
  char c;
  char d;
  char *p;
  char *pe;
  char *o;

  for (
      p = this + start, pe = this + end, o = other;
      (c = *p) != '\0' && p != pe;
      p++, o++) {
    if ((d = *o) == '\0') {
      return false;
    }

    if (d != c) {
      return false;
    }
  }

  if (*o != '\0') {
    return false;
  }

  return true;
}

bool string_contains_at(char *this, ulong index, char *other) {
  char c;
  char *p;
  char *q;
  ulong len_other;

  len_other = string_length(other);
  if (index + len_other > string_length(this)) {
    return false;
  }
  for (p = this + index, q = other; (c = *q) != '\0'; p++, q++) {
    if (*p != c) {
      return false;
    }
  }
  return true;
}

ulong string_split(char *this, char split_char, char ***psplits) {
  char c;
  char *p;
  char *s;
  char **splits;
  ulong n;
  ulong u;

  for (n = 0, p = this; (c = *p) != '\0'; p++) {
    if (c == split_char) {
      n++;
    }
  }
  n++;
  splits = new(psplits, n);
  for (u = 0, s = p = this; (c = *p) != '\0'; p++) {
    if (c == split_char) {
      string_new_substring(&splits[u++], s, 0, p - s);
      s = p + 1;
    }
  }
  string_new_substring(&splits[u++], s, 0, p - s);
  if (u != n) {
    throw("Impossible");
  }
  return n;
}

bool string_check_or_set(char **pthis, char *other) {
  char *this;

  if ((this = *pthis) == null) {
    string_new_copy(pthis, other);
    return true;
  }
  return string_equals(this, other);
}

char *string_quote_or_null(char **pthis, char *other) {
  if (other == null) {
    return string_new_copy(pthis, "(null)");
  }
  return string_new_f(pthis, "'%s'", other);
}

char *string_replace_chars(char *this, char *find, char replace) {
  char *p;
  char *f;

  for (p = this; *p != '\0'; p++) {
    for (f = find; *f != '\0'; f++) {
      if (*p == *f) {
        *p = replace;
        break;
      }
    }
  }
  return this;
}

bool string_new_copy_wrap(
    char **pthis,
    char *other,
    ulong start,
    ulong width,
    ulong indent) {
  bool wrapped;
  char *this;
  char c;
  char *last_q;
  char *p;
  char *q;
  char *t;
  char *u;
  char **ss = null;
  ulong len;
  ulong n;
  ulong pos;
  ulong start_pos;

  if (width == 0) {
    throw("Width cannot be zero");
  }

  if (start >= width) {
    throw("Start %lu not valid for width %lu", start, width);
  }

  if (indent >= width) {
    throw("Indent %lu not valid for width %lu", indent, width);
  }

  if (*other == '\0') {
    string_new_empty(pthis);
    return false;
  }

  for (p = other; (c = *p) != '\0'; p++) {
    if (c == '\n') {
      throw("Cannot wrap strings with newlines");
    }
  }

  new(&ss, n = 0);
  wrapped = false;

  for (p = other, start_pos = start;; start_pos = indent) {
    q = p;

    do {
      last_q = q;

      while (*q == ' ') {
        q++;
      }

      for (; (c = *q) != '\0' && c != ' '; q++) {
      }

      len = q - p;

      if (len == 0) {
        throw("impossible");
      }

      pos = start_pos + len;

      if (c == '\0' || c == '\n') {
        break;
      }

      q++;
    } while (pos <= width);

    if (pos > width) {
      if (last_q == p) {
        q = p + width - start_pos;
      } else {
        q = last_q - 1;
      }

      len = q - p;
      c = *q;
    }

    renewup(&ss, n, n + 1);
    u = new(&ss[n], len + 1);
    n++;

    if (q == p) {
      throw("zero length");
    }

    for (t = p; t != q; t++, u++) {
      *u = *t;
    }

    *u = '\0';

    if (c == '\0') {
      break;
    }

    p = q;

    while (*p == ' ') {
      p++;
    }

    if (*p == '\0') {
      break;
    }

    renewup(&ss, n, n + 1);
    string_new_f(&ss[n], "\n%*s", indent, "");
    wrapped = true;
    n++;
  }

  this = string_new_concat_array(pthis, n, ss);
  delete(&ss, n, string_delete);
  return wrapped;
}

bool string_renew_wrap(
    char **pthis,
    ulong start,
    ulong width,
    ulong indent) {
  bool wrapped;
  char *tmp = null;

  wrapped = string_new_copy_wrap(&tmp, *pthis, start, width, indent);
  string_delete(pthis);
  donate(&tmp, pthis);
  return wrapped;
}

bool string_renew_concat_wrap(
    char **pthis,
    char *other,
    ulong start,
    ulong width,
    ulong indent) {
  bool wrapped;
  char *tmp = null;

  wrapped = string_new_copy_wrap(&tmp, other, start, width, indent);
  string_renew_concat(pthis, tmp);
  string_delete(&tmp);
  return wrapped;
}

bool string_new_f_wrap(
    char **pthis,
    ulong start,
    ulong width,
    ulong indent,
    char *fmt,
    ...) {
  va_list ap1;
  va_list ap2;

  va_start(ap1, fmt);
  va_start(ap2, fmt);
  string_new_vf(pthis, fmt, ap1, ap2);
  va_end(ap1);
  va_end(ap2);

  return string_renew_wrap(pthis, start, width, indent);
}

bool string_renew_concat_f_wrap(
    char **pthis,
    ulong start,
    ulong width,
    ulong indent,
    char *fmt,
    ...) {
  bool wrapped;
  char *s = null;
  va_list ap1;
  va_list ap2;

  va_start(ap1, fmt);
  va_start(ap2, fmt);
  string_new_vf(&s, fmt, ap1, ap2);
  va_end(ap1);
  va_end(ap2);

  wrapped = string_renew_wrap(&s, start, width, indent);
  string_renew_concat(pthis, s);
  string_delete(&s);
  return wrapped;
}

char *string_new_commaize(char **pthis, char *other) {
  bool veto;
  char c;
  char *p;
  char *q;
  char *r;
  char *start;
  ulong u;

  string_new_empty(pthis);
  u = 0;

  for (p = other, veto = false; *p != '\0';) {
    for (q = p; (c = *q) != '\0' && (veto || !isdigit(c)); q++) {
      veto = isalpha(c) || c == '.' || c == '_' ||
        (veto && (isdigit(c) || c == '-'));
    }
    renewup(pthis, u + 1, u + (q - p) + 1);
    for (r = *pthis + u; p != q; p++, r++, u++) {
      *r = *p;
    }
    *r = '\0';
    for (; (c = *q) != '\0' && isdigit(c); q++) {
    }
    if (*p == '0' || isalpha(c) || c == '_' || q - p < 5) {
      renewup(pthis, u + 1, u + (q - p) + 1);
      for (r = *pthis + u; p != q; p++, r++, u++) {
        *r = *p;
      }
    } else {
      renewup(pthis, u + 1, u + (q - p) + (q - p - 1) / 3 + 1);
      start = p;
      for (r = *pthis + u; p != q; p++, r++, u++) {
        if (p != start && (q - p) % 3 == 0) {
          *r++ = ',';
          u++;
        }
        *r = *p;
      }
    }
    *r = '\0';
  }

  return *pthis;
}

char *string_renew_commaize(char **pthis) {
  char *s = null;

  string_new_commaize(&s, *pthis);
  string_delete(pthis);
  return donate(&s, pthis);
}

char *string_copy(char *this, char *other, ulong max_len) {
  char *s;
  char *d;
  char *e;

  for (s = this, e = s + max_len, d = other; s < e && *s != '\0'; s++, d++) {
    *d = *s;
  }
  if (*s != '\0') {
    throw("max_len %lu but string length %lu", max_len, string_length(this));
  }
  *d = '\0';
  return d;
}

static int exponent_(double d) {
  if (d == 0) {
    return 0;
  }
  return (int)floor(log10(fabs(d)));
}
