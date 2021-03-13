/* Useful utilities for numbers. numbersTest.cpp has instructive examples.
 */
#ifndef NUMBERS_H
#define NUMBERS_H

#ifdef __cplusplus
extern "C" {
#endif

#include "jpc.h"

/* The maximum number of useful significant figures for a double. Used for
 * quantization purposes only.
 */
#define DOUBLE_MAX_SIG_FIGS 15

/* Try parsing a string for various numeric types.
 */
bool try_parse_bool(bool *b, char *s);
bool try_parse_byte(byte *b, char *s);
bool try_parse_ushort(ushort *u, char *s);
bool try_parse_uint(uint *u, char *s);
bool try_parse_ulong(ulong *u, char *s);
bool try_parse_short(short *h, char *s);
bool try_parse_int(int *i, char *s);
bool try_parse_long(long *l, char *s);
bool try_parse_double(double *d, char *s);

/* Parse a string for various numeric types. Exit with error if invalid.
 */
bool parse_bool(char *s);
byte parse_byte(char *s);
ushort parse_ushort(char *s);
uint parse_uint(char *s);
ulong parse_ulong(char *s);
short parse_short(char *s);
int parse_int(char *s);
long parse_long(char *s);
double parse_double(char *s);

/* Number of decimal digits that a ulong has.
 */
byte ulong_digits(ulong u);

/* Number of significant figures in a string representation of a double.
 */
byte double_significant_figures(char *s);

/* Quantize a long to the specified number of significant digits. Truncates so
 * that repeated quantizations are equivalent to a single quantization.
 */
long quantize_long(long l, byte num_significant_figures);

/* Minimum and maximum values that a quantized long could have been.
 */
long quantized_long_min_value(long l, byte num_significant_figures);
long quantized_long_max_value(long l, byte num_significant_figures);

/* Quantize a double to the specified number of significant figures. Again,
 * this truncates, so that repeated quantizations are equivalent to a single
 * quantization; but due to floating point roundoff, repeated quantizations
 * require care. The function therefore requires the previous number of
 * significant figures to be supplied (specify 0 if the value has not been
 * previously quantized).
 */
double quantize_double(
    double x,
    byte old_num_significant_figures,
    byte new_num_significant_figures);

/* Minimum and maximum values that a quantized double could have been.
 */
double quantized_double_min_value(double x, byte num_significant_figures);
double quantized_double_max_value(double x, byte num_significant_figures);

/* Strings for serialization.
 */
char *string_new_long(char **pthis, long l);
char *string_new_ulong(char **pthis, ulong u);
char *string_new_double(char **pthis, double d);
char *string_new_double_pair(char **pthis, double d1, double d2);
char *string_new_double_triple(char **pthis, double d1, double d2, double d3);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* _H */
