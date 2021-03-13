#include "numbers.h"
#include <ctype.h>
#include <errno.h>
#include <limits.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

static void run_off_(char **pendp);
static void quantized_double_check_(double d, byte num_significant_figures);
static long quantized_long_min_value_(
    long l,
    byte num_significant_figures,
    bool negative);
static long quantized_long_max_value_(
    long l,
    byte num_significant_figures,
    bool negative);

bool try_parse_bool(bool *b, char *s) {
  if (string_equals(s, "true")) {
    *b = true;
    return true;
  }
  if (string_equals(s, "false")) {
    *b = false;
    return true;
  }
  if (string_equals(s, "True")) {
    *b = true;
    return true;
  }
  if (string_equals(s, "False")) {
    *b = false;
    return true;
  }
  if (string_equals(s, "1")) {
    *b = true;
    return true;
  }
  if (string_equals(s, "0")) {
    *b = false;
    return true;
  }
  return false;
}

bool try_parse_byte(byte *b, char *s) {
  ulong ul;

  if (!try_parse_ulong(&ul, s)) {
    return false;
  }
  if (ul > UCHAR_MAX) {
    return false;
  }
  *b = (byte)ul;
  return true;
}

bool try_parse_ushort(ushort *u, char *s) {
  ulong ul;

  if (!try_parse_ulong(&ul, s)) {
    return false;
  }
  if (ul > USHRT_MAX) {
    return false;
  }
  *u = (ushort)ul;
  return true;
}

bool try_parse_uint(uint *u, char *s) {
  ulong ul;

  if (!try_parse_ulong(&ul, s)) {
    return false;
  }
  if (ul > UINT_MAX) {
    return false;
  }
  *u = (uint)ul;
  return true;
}

bool try_parse_ulong(ulong *u, char *s) {
  char c;
  char *p;
  char *endp;
  ulong result;

  /* strtoul() accepts negative numbers. We don't want them.
   */
  for (p = s; (c = *p) != '\0'; p++) {
    if (c == '-') {
      return false;
    }
  }

  errno = 0;
  result = strtoul(s, &endp, 10);
  run_off_(&endp);
  if (errno != 0 || *endp != '\0') {
    return false;
  }
  *u = result;
  return true;
}

bool try_parse_double(double *d, char *s) {
  char *endp;
  double result;

  errno = 0;
  result = strtod(s, &endp);
  run_off_(&endp);
  if (errno != 0 || *endp != '\0') {
    return false;
  }
  *d = result;
  return true;
}

bool try_parse_short(short *h, char *s) {
  long l;

  if (!try_parse_long(&l, s)) {
    return false;
  }
  if (l > SHRT_MAX || l < SHRT_MIN) {
    return false;
  }
  *h = (short)l;
  return true;
}

bool try_parse_int(int *i, char *s) {
  long l;

  if (!try_parse_long(&l, s)) {
    return false;
  }
  if (l > INT_MAX || l < INT_MIN) {
    return false;
  }
  *i = (int)l;
  return true;
}

bool try_parse_long(long *l, char *s) {
  char *endp;
  long result;

  errno = 0;
  result = strtol(s, &endp, 0);
  run_off_(&endp);
  if (errno != 0 || *endp != '\0') {
    return false;
  }
  *l = result;
  return true;
}

byte parse_bool(char *s) {
  bool result = false;

  if (!try_parse_bool(&result, s)) {
    throw("Couldn't parse '%s' as a bool", s);
  }
  return result;
}

byte parse_byte(char *s) {
  byte result = 0;

  if (!try_parse_byte(&result, s)) {
    throw("Couldn't parse '%s' as a byte", s);
  }
  return result;
}

ushort parse_ushort(char *s) {
  ushort result = 0;

  if (!try_parse_ushort(&result, s)) {
    throw("Couldn't parse '%s' as a ushort", s);
  }
  return result;
}

uint parse_uint(char *s) {
  uint result = 0;

  if (!try_parse_uint(&result, s)) {
    throw("Couldn't parse '%s' as a uint", s);
  }
  return result;
}

ulong parse_ulong(char *s) {
  ulong result = 0;

  if (!try_parse_ulong(&result, s)) {
    throw("Couldn't parse '%s' as a ulong", s);
  }
  return result;
}

short parse_short(char *s) {
  short result = 0;

  if (!try_parse_short(&result, s)) {
    throw("Couldn't parse '%s' as a short", s);
  }
  return result;
}

int parse_int(char *s) {
  int result = 0;

  if (!try_parse_int(&result, s)) {
    throw("Couldn't parse '%s' as an int", s);
  }
  return result;
}

long parse_long(char *s) {
  long result = 0;

  if (!try_parse_long(&result, s)) {
    throw("Couldn't parse '%s' as a long", s);
  }
  return result;
}

double parse_double(char *s) {
  double result = 0.0;

  if (!try_parse_double(&result, s)) {
    throw("Couldn't parse '%s' as a double", s);
  }
  return result;
}

byte ulong_digits(ulong u) {
  char s[32];

  sprintf(s, "%lu", u);
  return strlen(s);
}

byte double_significant_figures(char *s) {
  bool significant = false;
  byte n;
  char c;
  char *p;

  /* Make sure it represents a double.
   */
  parse_double(s);

  /* Count the number of digits from the first significant (non-zero) digit.
   */
  n = 0;
  for (p = s; (c = *p) != '\0'; p++) {
    if (c == 'e' || c == 'E') {
      break;
    }
    if (isdigit(c)) {
      if (c != '0') {
        significant = true;
      }
      if (significant) {
        n++;
      }
    }
  }
  return n;
}

long quantize_long(long l, byte num_significant_figures) {
  char s[32];
  char *p;

  if (num_significant_figures == 0) {
    throw("Number of significant figures cannot be zero");
  }

  sprintf(s, "%ld", l);

  for (p = s + num_significant_figures + (l < 0 ? 1 : 0); *p != '\0'; p++) {
    *p = '0';
  }

  return atol(s);
}

static long quantized_long_min_value_(
    long l,
    byte num_significant_figures,
    bool negative) {
  if (l < 0) {
    throw("impossible %ld", l);
  }
  if (quantize_long(l, num_significant_figures) != l) {
    throw(
        "%s%ld was not quantized to %u significant figure%s",
        negative ? "-" : "",
        l,
        num_significant_figures,
        plural_s(num_significant_figures));
  }
  return l;
}

long quantized_long_min_value(long l, byte num_significant_figures) {
  if (num_significant_figures == 0) {
    throw("Number of significant figures cannot be zero");
  }
  if (l < 0) {
    return -quantized_long_max_value_(-l, num_significant_figures, true);
  }
  return quantized_long_min_value_(l, num_significant_figures, false);
}

static long quantized_long_max_value_(
    long l,
    byte num_significant_figures,
    bool negative) {
  char s[32];
  char *p;

  if (l < 0) {
    throw("impossible %ld", l);
  }
  sprintf(s, "%ld", l);

  if (string_length(s) <= num_significant_figures) {
    return l;
  }

  for (p = s + num_significant_figures; *p != '\0'; p++) {
    if (*p != '0') {
      throw(
          "%s%ld was not quantized to %u significant figure%s",
          negative ? "-" : "",
          l,
          num_significant_figures,
          plural_s(num_significant_figures));
    }
    *p = '9';
  }

  if (!try_parse_long(&l, s)) {
    return LONG_MAX;
  }
  return atol(s);
}

long quantized_long_max_value(long l, byte num_significant_figures) {
  if (num_significant_figures == 0) {
    throw("Number of significant figures cannot be zero");
  }
  if (l < 0) {
    return -quantized_long_min_value_(-l, num_significant_figures, true);
  }
  return quantized_long_max_value_(l, num_significant_figures, false);
}

double quantize_double(
    double d,
    byte old_num_significant_figures,
    byte new_num_significant_figures) {
  char s[40];
  char *p;
  char *q;
  int c;

  if (old_num_significant_figures == 0 ||
      old_num_significant_figures > DOUBLE_MAX_SIG_FIGS) {
    old_num_significant_figures = DOUBLE_MAX_SIG_FIGS;
    if (new_num_significant_figures > DOUBLE_MAX_SIG_FIGS) {
      new_num_significant_figures = DOUBLE_MAX_SIG_FIGS;
    }
  }

  if (new_num_significant_figures == 0) {
    throw("Number of significant figures cannot be zero");
  }
  if (new_num_significant_figures > old_num_significant_figures) {
    throw(
        "Number of significant figures cannot be increased from %u to %u",
        old_num_significant_figures,
        new_num_significant_figures);
  }

  if (new_num_significant_figures < DOUBLE_MAX_SIG_FIGS &&
      new_num_significant_figures == old_num_significant_figures) {
    return d;
  }

  /* Print the value in scientific notation with previous precision.
   */
  sprintf(s, "%.*e", old_num_significant_figures - 1, d);

  /* Adjust for punctuation: 1 for the decimal point after the leading digit,
   * and an extra 1 if there is a minus sign.
   */
  c = (d < 0 ? 2 : 1);

  /* Copy the exponent back.
   */
  for (p = s + old_num_significant_figures + c,
      q = s + new_num_significant_figures + c;
      *q != '\0';
      p++, q++) {
    *q = *p;
  }
  *p = '\0';

  return atof(s);
}

double quantized_double_min_value(double d, byte num_significant_figures) {
  if (d >= 0) {
    quantized_double_check_(d, num_significant_figures);
    return d;
  }
  quantized_double_check_(d, num_significant_figures);
  return -quantized_double_max_value(-d, num_significant_figures);
}

double quantized_double_max_value(double d, byte num_significant_figures) {
  if (d <= 0) {
    quantized_double_check_(d, num_significant_figures);
    return d;
  }
  quantized_double_check_(d, num_significant_figures);
  return d + pow(10, floor(log10(d)) - num_significant_figures + 1);
}

void assert_ulongs_equal(ulong u, ulong v) {
  if (u != v) {
    throw("Failed assertion that %lu == %lu", u, v);
  }
}

/* Runs off any whitespace at the end of the parsed string.
 */
static void run_off_(char **pendp) {
  char *endp;

  endp = *pendp;
  while (isspace(*endp)) {
    endp++;
  }
  *pendp = endp;
}

static void quantized_double_check_(double d, byte num_significant_figures) {
  char *s1 = null;
  char *s2 = null;
  double check;

  if (num_significant_figures == 0) {
    throw("Number of significant figures cannot be zero");
  }

  check = quantize_double(
      d,
      num_significant_figures + 1,
      num_significant_figures);
  if (d != check) {
    string_new_double(&s1, d);
    string_new_double(&s2, check);
    throw(
        "%s is not quantized to %u significant figures; should be %s",
        s1,
        num_significant_figures,
        s2);
    string_delete(&s2);
    string_delete(&s1);
  }
}

char *string_new_long(char **pthis, long l) {
  return string_new_f(pthis, "%ld", l);
}

char *string_new_ulong(char **pthis, ulong u) {
  return string_new_f(pthis, "%lu", u);
}

char *string_new_double(char **pthis, double d) {
  return string_new_f(pthis, "%.17e", d);
}

char *string_new_double_pair(char **pthis, double d1, double d2) {
  char *s1 = null;
  char *s2 = null;

  string_new_double(&s1, d1);
  string_new_double(&s2, d2);
  string_new_f(pthis, "%s|%s", s1, s2);
  string_delete(&s2);
  string_delete(&s1);

  return *pthis;
}

char *string_new_double_triple(char **pthis, double d1, double d2, double d3) {
  char *s1 = null;
  char *s2 = null;
  char *s3 = null;

  string_new_double(&s1, d1);
  string_new_double(&s2, d2);
  string_new_double(&s3, d3);
  string_new_f(pthis, "%s|%s|%s", s1, s2, s3);
  string_delete(&s3);
  string_delete(&s2);
  string_delete(&s1);

  return *pthis;
}
