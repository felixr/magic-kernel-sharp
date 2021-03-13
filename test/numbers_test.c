#include "numbers_test.h"
#include <limits.h>

START

TEST(Parse, TryByte) {
  byte b;

  EXPECT_TRUE(try_parse_byte(&b, "0"));
  EXPECT_ULEQ(0, b);
  EXPECT_TRUE(try_parse_byte(&b, " +12"));
  EXPECT_ULEQ(12, b);
  EXPECT_TRUE(try_parse_byte(&b, "034 "));
  EXPECT_ULEQ(34, b);
  EXPECT_TRUE(try_parse_byte(&b, "\t +255\r "));
  EXPECT_ULEQ(0xff, b);
  EXPECT_FALSE(try_parse_byte(&b, "-1"));
  EXPECT_FALSE(try_parse_byte(&b, "+ 1"));
  EXPECT_FALSE(try_parse_byte(&b, "1+"));
  EXPECT_FALSE(try_parse_byte(&b, "256"));
  EXPECT_FALSE(try_parse_byte(&b, "12334875643875643876587346565536"));
  EXPECT_FALSE(try_parse_byte(&b, "12 34"));
  EXPECT_FALSE(try_parse_byte(&b, "12.7"));
  EXPECT_FALSE(try_parse_byte(&b, "4."));
  EXPECT_FALSE(try_parse_byte(&b, "5e2"));
}

TEST(Parse, TryUshort) {
  ushort u;

  EXPECT_TRUE(try_parse_ushort(&u, "0"));
  EXPECT_ULEQ(0, u);
  EXPECT_TRUE(try_parse_ushort(&u, " +12"));
  EXPECT_ULEQ(12, u);
  EXPECT_TRUE(try_parse_ushort(&u, "345 "));
  EXPECT_ULEQ(345, u);
  EXPECT_TRUE(try_parse_ushort(&u, "\t +6789\r "));
  EXPECT_ULEQ(6789, u);
  EXPECT_TRUE(try_parse_ushort(&u, " \f65535 \n\n "));
  EXPECT_ULEQ(0xffff, u);
  EXPECT_FALSE(try_parse_ushort(&u, "-1"));
  EXPECT_FALSE(try_parse_ushort(&u, "+ 1"));
  EXPECT_FALSE(try_parse_ushort(&u, "1+"));
  EXPECT_FALSE(try_parse_ushort(&u, "65536"));
  EXPECT_FALSE(try_parse_ushort(&u, "12334875643875643876587346565536"));
  EXPECT_FALSE(try_parse_ushort(&u, "12 34"));
  EXPECT_FALSE(try_parse_ushort(&u, "12.7"));
  EXPECT_FALSE(try_parse_ushort(&u, "4."));
  EXPECT_FALSE(try_parse_ushort(&u, "5e3"));
}

TEST(Parse, TryUint) {
  uint u;

  EXPECT_TRUE(try_parse_uint(&u, "0"));
  EXPECT_ULEQ(0, u);
  EXPECT_TRUE(try_parse_uint(&u, " +12"));
  EXPECT_ULEQ(12, u);
  EXPECT_TRUE(try_parse_uint(&u, "345 "));
  EXPECT_ULEQ(345, u);
  EXPECT_TRUE(try_parse_uint(&u, "\t +6789\r "));
  EXPECT_ULEQ(6789, u);
  EXPECT_TRUE(try_parse_uint(&u, " \f4294967295 \n\n "));
  EXPECT_ULEQ(0xffffffff, u);
  EXPECT_FALSE(try_parse_uint(&u, "-1"));
  EXPECT_FALSE(try_parse_uint(&u, "+ 1"));
  EXPECT_FALSE(try_parse_uint(&u, "1+"));
  EXPECT_FALSE(try_parse_uint(&u, "4294967296"));
  EXPECT_FALSE(try_parse_uint(&u, "12334875643875643876587346565536"));
  EXPECT_FALSE(try_parse_uint(&u, "12 34"));
  EXPECT_FALSE(try_parse_uint(&u, "12.7"));
  EXPECT_FALSE(try_parse_uint(&u, "4."));
  EXPECT_FALSE(try_parse_uint(&u, "5e3"));
}

TEST(Parse, TryUlong) {
  ulong u;

  EXPECT_TRUE(try_parse_ulong(&u, "0"));
  EXPECT_ULEQ(0, u);
  EXPECT_TRUE(try_parse_ulong(&u, " +12"));
  EXPECT_ULEQ(12, u);
  EXPECT_TRUE(try_parse_ulong(&u, "345 "));
  EXPECT_ULEQ(345, u);
  EXPECT_TRUE(try_parse_ulong(&u, "\t +6789\r "));
  EXPECT_ULEQ(6789, u);
  EXPECT_TRUE(try_parse_ulong(&u, " \f18446744073709551615 \n\n "));
  EXPECT_ULEQ(0xffffffffffffffff, u);
  EXPECT_FALSE(try_parse_ulong(&u, "-1"));
  EXPECT_FALSE(try_parse_ulong(&u, "+ 1"));
  EXPECT_FALSE(try_parse_ulong(&u, "1+"));
  EXPECT_FALSE(try_parse_ulong(&u, "18446744073709551616"));
  EXPECT_FALSE(try_parse_ulong(&u, "12334875643875643876587346565536"));
  EXPECT_FALSE(try_parse_ulong(&u, "12 34"));
  EXPECT_FALSE(try_parse_ulong(&u, "12.7"));
  EXPECT_FALSE(try_parse_ulong(&u, "4."));
  EXPECT_FALSE(try_parse_ulong(&u, "5e3"));
}

TEST(Parse, TryShort) {
  short s;

  EXPECT_TRUE(try_parse_short(&s, "0"));
  EXPECT_ULEQ(0, s);
  EXPECT_TRUE(try_parse_short(&s, " -12"));
  EXPECT_ULEQ(-12, s);
  EXPECT_TRUE(try_parse_short(&s, "+345 "));
  EXPECT_ULEQ(345, s);
  EXPECT_TRUE(try_parse_short(&s, "\t -6789\r "));
  EXPECT_ULEQ(-6789, s);
  EXPECT_TRUE(try_parse_short(&s, " \f32767 \n\n "));
  EXPECT_ULEQ(0x7fff, s);
  EXPECT_TRUE(try_parse_short(&s, " \f -32768 \n\n "));
  EXPECT_ULEQ(-0x8000, s);
  EXPECT_FALSE(try_parse_short(&s, "-+1"));
  EXPECT_FALSE(try_parse_short(&s, "- 1"));
  EXPECT_FALSE(try_parse_short(&s, "1-"));
  EXPECT_FALSE(try_parse_short(&s, "32768"));
  EXPECT_FALSE(try_parse_short(&s, "12334875643875643876587346565536"));
  EXPECT_FALSE(try_parse_short(&s, "12 34"));
  EXPECT_FALSE(try_parse_short(&s, "12.7"));
  EXPECT_FALSE(try_parse_short(&s, "4."));
  EXPECT_FALSE(try_parse_short(&s, "5e3"));
}

TEST(Parse, TryInt) {
  int i;

  EXPECT_TRUE(try_parse_int(&i, "0"));
  EXPECT_ULEQ(0, i);
  EXPECT_TRUE(try_parse_int(&i, " -12"));
  EXPECT_ULEQ(-12, i);
  EXPECT_TRUE(try_parse_int(&i, "+345 "));
  EXPECT_ULEQ(345, i);
  EXPECT_TRUE(try_parse_int(&i, "\t -6789\r "));
  EXPECT_ULEQ(-6789, i);
  EXPECT_TRUE(try_parse_int(&i, " \f2147483647 \n\n "));
  EXPECT_ULEQ(0x7fffffff, i);
  EXPECT_TRUE(try_parse_int(&i, " \f -2147483648 \n\n "));
  EXPECT_LEQ(-0x80000000L, i);
  EXPECT_FALSE(try_parse_int(&i, "-+1"));
  EXPECT_FALSE(try_parse_int(&i, "- 1"));
  EXPECT_FALSE(try_parse_int(&i, "1-"));
  EXPECT_FALSE(try_parse_int(&i, "2147483648"));
  EXPECT_FALSE(try_parse_int(&i, "12334875643875643876587346565536"));
  EXPECT_FALSE(try_parse_int(&i, "12 34"));
  EXPECT_FALSE(try_parse_int(&i, "12.7"));
  EXPECT_FALSE(try_parse_int(&i, "4."));
  EXPECT_FALSE(try_parse_int(&i, "5e3"));
}

TEST(Parse, TryLong) {
  long l;

  EXPECT_TRUE(try_parse_long(&l, "0"));
  EXPECT_ULEQ(0, l);
  EXPECT_TRUE(try_parse_long(&l, " -12"));
  EXPECT_ULEQ(-12, l);
  EXPECT_TRUE(try_parse_long(&l, "+345 "));
  EXPECT_ULEQ(345, l);
  EXPECT_TRUE(try_parse_long(&l, "\t -6789\r "));
  EXPECT_ULEQ(-6789, l);
  EXPECT_TRUE(try_parse_long(&l, " \f9223372036854775807 \n\n "));
  EXPECT_ULEQ(0x7fffffffffffffff, l);
  EXPECT_TRUE(try_parse_long(&l, " \f -9223372036854775808 \n\n "));
  EXPECT_ULEQ(-0x8000000000000000, l);
  EXPECT_FALSE(try_parse_long(&l, "-+1"));
  EXPECT_FALSE(try_parse_long(&l, "- 1"));
  EXPECT_FALSE(try_parse_long(&l, "1-"));
  EXPECT_FALSE(try_parse_long(&l, "9223372036854775808"));
  EXPECT_FALSE(try_parse_long(&l, "12334875643875643876587346565536"));
  EXPECT_FALSE(try_parse_long(&l, "12 34"));
  EXPECT_FALSE(try_parse_long(&l, "12.7"));
  EXPECT_FALSE(try_parse_long(&l, "4."));
  EXPECT_FALSE(try_parse_long(&l, "5e3"));
}

TEST(Parse, TryDouble) {
  double d;

  EXPECT_TRUE(try_parse_double(&d, "0"));
  EXPECT_ULEQ(0, d);
  EXPECT_TRUE(try_parse_double(&d, " 0."));
  EXPECT_ULEQ(0, d);
  EXPECT_TRUE(try_parse_double(&d, "0.0 "));
  EXPECT_ULEQ(0, d);
  EXPECT_TRUE(try_parse_double(&d, "\t+0 "));
  EXPECT_ULEQ(0, d);
  EXPECT_TRUE(try_parse_double(&d, " \f -0 \n\r\f "));
  EXPECT_ULEQ(0, d);
  EXPECT_TRUE(try_parse_double(&d, "0e1"));
  EXPECT_ULEQ(0, d);
  EXPECT_TRUE(try_parse_double(&d, "0e+1"));
  EXPECT_ULEQ(0, d);
  EXPECT_TRUE(try_parse_double(&d, "+0e-1"));
  EXPECT_ULEQ(0, d);
  EXPECT_TRUE(try_parse_double(&d, "0.E1"));
  EXPECT_ULEQ(0, d);
  EXPECT_TRUE(try_parse_double(&d, " 0.0e13"));
  EXPECT_ULEQ(0, d);
  EXPECT_TRUE(try_parse_double(&d, " -12"));
  EXPECT_ULEQ(-12, d);
  EXPECT_TRUE(try_parse_double(&d, "+345 "));
  EXPECT_ULEQ(345, d);
  EXPECT_TRUE(try_parse_double(&d, "1.2345"));
  EXPECT_DEQ(1.2345, d);
  EXPECT_TRUE(try_parse_double(&d, " -12.34E-3"));
  EXPECT_DEQ(-0.01234, d);
  EXPECT_TRUE(try_parse_double(&d, "123456789012345678e7"));
  EXPECT_NEAR(1.234567890123457e24, d, 3e8);
  EXPECT_TRUE(try_parse_double(&d, "1.7976931348623157e+308"));
  EXPECT_DEQ(1.7976931348623157e+308, d);
  EXPECT_TRUE(try_parse_double(&d, "-1.7976931348623157e+308"));
  EXPECT_DEQ(-1.7976931348623157e+308, d);
  EXPECT_FALSE(try_parse_double(&d, "1.797693134862317e+308"));
  EXPECT_FALSE(try_parse_double(&d, "-1.797693134862317e+308"));
  EXPECT_FALSE(try_parse_double(&d, "-+1"));
  EXPECT_FALSE(try_parse_double(&d, "+-1"));
  EXPECT_FALSE(try_parse_double(&d, "1g3"));
  EXPECT_FALSE(try_parse_double(&d, "1e-+2"));
  EXPECT_FALSE(try_parse_double(&d, "1e+-2"));
  EXPECT_FALSE(try_parse_double(&d, "1.34a"));
  EXPECT_FALSE(try_parse_double(&d, "1.23.4"));
}

TEST(Parse, Byte) {
  EXPECT_ULEQ(0, parse_byte("0"));
  EXPECT_ULEQ(12, parse_byte(" +12"));
  EXPECT_ULEQ(34, parse_byte("34 "));
  EXPECT_ULEQ(0xff, parse_byte("\t +255\r "));
}

TEST(Parse, Ushort) {
  EXPECT_ULEQ(0, parse_ushort("0"));
  EXPECT_ULEQ(12, parse_ushort(" +12"));
  EXPECT_ULEQ(345, parse_ushort("345 "));
  EXPECT_ULEQ(6789, parse_ushort("\t +6789\r "));
  EXPECT_ULEQ(0xffff, parse_ushort(" \f65535 \n\n "));
}

TEST(Parse, Uint) {
  EXPECT_ULEQ(0, parse_uint("0"));
  EXPECT_ULEQ(12, parse_uint(" +12"));
  EXPECT_ULEQ(345, parse_uint("345 "));
  EXPECT_ULEQ(6789, parse_uint("\t +6789\r "));
  EXPECT_ULEQ(0xffffffff, parse_uint(" \f4294967295 \n\n "));
}

TEST(Parse, Ulong) {
  EXPECT_ULEQ(0, parse_ulong("0"));
  EXPECT_ULEQ(12, parse_ulong(" +12"));
  EXPECT_ULEQ(345, parse_ulong("345 "));
  EXPECT_ULEQ(6789, parse_ulong("\t +6789\r "));
  EXPECT_ULEQ(0xffffffffffffffff, parse_ulong(" \f18446744073709551615 \n\n "));
}

TEST(Parse, Short) {
  EXPECT_ULEQ(0, parse_short("0"));
  EXPECT_ULEQ(-12, parse_short(" -12"));
  EXPECT_ULEQ(345, parse_short("+345 "));
  EXPECT_ULEQ(-6789, parse_short("\t -6789\r "));
  EXPECT_ULEQ(0x7fff, parse_short(" \f32767 \n\n "));
  EXPECT_ULEQ(-0x8000, parse_short(" \f -32768 \n\n "));
}

TEST(Parse, Int) {
  EXPECT_ULEQ(0, parse_int("0"));
  EXPECT_ULEQ(-12, parse_int(" -12"));
  EXPECT_ULEQ(345, parse_int("+345 "));
  EXPECT_ULEQ(-6789, parse_int("\t -6789\r "));
  EXPECT_ULEQ(0x7fffffff, parse_int(" \f2147483647 \n\n "));
  EXPECT_ULEQ(-0x80000000L, parse_int(" \f -2147483648 \n\n "));
}

TEST(Parse, Long) {
  EXPECT_ULEQ(0, parse_long("0"));
  EXPECT_ULEQ(-12, parse_long(" -12"));
  EXPECT_ULEQ(345, parse_long("+345 "));
  EXPECT_ULEQ(-6789, parse_long("\t -6789\r "));
  EXPECT_ULEQ(0x7fffffffffffffff, parse_long(" \f9223372036854775807 \n\n "));
  EXPECT_ULEQ(
      -0x8000000000000000,
      parse_long(" \f -9223372036854775808 \n\n "));
}

TEST(Parse, Double) {
  EXPECT_ULEQ(0, parse_double("0"));
  EXPECT_ULEQ(0, parse_double(" 0."));
  EXPECT_ULEQ(0, parse_double("0.0 "));
  EXPECT_ULEQ(0, parse_double("\t+0 "));
  EXPECT_ULEQ(0, parse_double(" \f -0 \n\r\f "));
  EXPECT_ULEQ(0, parse_double("0e1"));
  EXPECT_ULEQ(0, parse_double("0e+1"));
  EXPECT_ULEQ(0, parse_double("0e-1"));
  EXPECT_ULEQ(0, parse_double("0.E1"));
  EXPECT_ULEQ(0, parse_double(" 0.0E13"));
  EXPECT_ULEQ(-12, parse_double("\t-12"));
  EXPECT_ULEQ(345, parse_double("+345  "));
  EXPECT_DEQ(-0.01234, parse_double(" -12.34E-3"));
  EXPECT_NEAR(1.234567890123457e24, parse_double("123456789012345678e7"), 3e8);
  EXPECT_DEQ(
      1.7976931348623157e+308,
      parse_double("1.7976931348623157E+308"));
  EXPECT_DEQ(
      -1.7976931348623157e+308,
      parse_double("-1.7976931348623157e+308"));
}

TEST(Digits, Ulong) {
  EXPECT_ULEQ(1, ulong_digits(0));
  EXPECT_ULEQ(1, ulong_digits(1));
  EXPECT_ULEQ(2, ulong_digits(99));
  EXPECT_ULEQ(5, ulong_digits(12345));
  EXPECT_ULEQ(5, ulong_digits(12345));
  EXPECT_ULEQ(20, ulong_digits(0xffffffffffffffff));
}

TEST(Digits, Double) {
  EXPECT_ULEQ(0, double_significant_figures("0"));
  EXPECT_ULEQ(1, double_significant_figures("1"));
  EXPECT_ULEQ(1, double_significant_figures("1."));
  EXPECT_ULEQ(1, double_significant_figures("  001."));
  EXPECT_ULEQ(2, double_significant_figures("1.2 "));
  EXPECT_ULEQ(2, double_significant_figures("-1.2e34"));
  EXPECT_ULEQ(0, double_significant_figures("0e12"));
  EXPECT_ULEQ(12, double_significant_figures("0001023.40067890e-56"));
}

TEST(Quantize, Long) {
  EXPECT_LEQ(0, quantize_long(0, 1));
  EXPECT_LEQ(0, quantize_long(0, 5));
  EXPECT_LEQ(1, quantize_long(1, 1));
  EXPECT_LEQ(1, quantize_long(1, 5));
  EXPECT_LEQ(-1, quantize_long(-1, 1));
  EXPECT_LEQ(-1, quantize_long(-1, 5));
  EXPECT_LEQ(9, quantize_long(9, 1));
  EXPECT_LEQ(9, quantize_long(9, 5));
  EXPECT_LEQ(-9, quantize_long(-9, 1));
  EXPECT_LEQ(-9, quantize_long(-9, 5));
  EXPECT_LEQ(10, quantize_long(10, 1));
  EXPECT_LEQ(10, quantize_long(10, 5));
  EXPECT_LEQ(-10, quantize_long(-10, 1));
  EXPECT_LEQ(-10, quantize_long(-10, 5));
  EXPECT_LEQ(10, quantize_long(19, 1));
  EXPECT_LEQ(19, quantize_long(19, 5));
  EXPECT_LEQ(-10, quantize_long(-19, 1));
  EXPECT_LEQ(-19, quantize_long(-19, 5));
  EXPECT_LEQ(100, quantize_long(100, 1));
  EXPECT_LEQ(100, quantize_long(100, 2));
  EXPECT_LEQ(100, quantize_long(100, 3));
  EXPECT_LEQ(100, quantize_long(198, 1));
  EXPECT_LEQ(190, quantize_long(198, 2));
  EXPECT_LEQ(198, quantize_long(198, 17));
  EXPECT_LEQ(-100, quantize_long(-100, 1));
  EXPECT_LEQ(-100, quantize_long(-100, 2));
  EXPECT_LEQ(-100, quantize_long(-100, 3));
  EXPECT_LEQ(-100, quantize_long(-198, 1));
  EXPECT_LEQ(-190, quantize_long(-198, 2));
  EXPECT_LEQ(-198, quantize_long(-198, 17));
  EXPECT_LEQ(9223372036854775807, quantize_long(9223372036854775807, 20));
  EXPECT_LEQ(9223372036854775807, quantize_long(9223372036854775807, 19));
  EXPECT_LEQ(9223372036854775800, quantize_long(9223372036854775807, 18));
  EXPECT_LEQ(9223372036854775800, quantize_long(9223372036854775807, 17));
  EXPECT_LEQ(9223372036854775000, quantize_long(9223372036854775807, 16));
  EXPECT_LEQ(9223372036854770000, quantize_long(9223372036854775807, 15));
  EXPECT_LEQ(9223372036000000000, quantize_long(9223372036854775807, 10));
  EXPECT_LEQ(9223300000000000000, quantize_long(9223372036854775807, 5));
  EXPECT_LEQ(9223000000000000000, quantize_long(9223372036854775807, 4));
  EXPECT_LEQ(9220000000000000000, quantize_long(9223372036854775807, 3));
  EXPECT_LEQ(9200000000000000000, quantize_long(9223372036854775807, 2));
  EXPECT_LEQ(9000000000000000000, quantize_long(9223372036854775807, 1));
  EXPECT_LEQ(-9223372036854775807, quantize_long(-9223372036854775807, 20));
  EXPECT_LEQ(-9223372036854775807, quantize_long(-9223372036854775807, 19));
  EXPECT_LEQ(-9223372036854775800, quantize_long(-9223372036854775807, 18));
  EXPECT_LEQ(-9223372036854775800, quantize_long(-9223372036854775807, 17));
  EXPECT_LEQ(-9223372036854775000, quantize_long(-9223372036854775807, 16));
  EXPECT_LEQ(-9223372036854770000, quantize_long(-9223372036854775807, 15));
  EXPECT_LEQ(-9223372036000000000, quantize_long(-9223372036854775807, 10));
  EXPECT_LEQ(-9223300000000000000, quantize_long(-9223372036854775807, 5));
  EXPECT_LEQ(-9223000000000000000, quantize_long(-9223372036854775807, 4));
  EXPECT_LEQ(-9220000000000000000, quantize_long(-9223372036854775807, 3));
  EXPECT_LEQ(-9200000000000000000, quantize_long(-9223372036854775807, 2));
  EXPECT_LEQ(-9000000000000000000, quantize_long(-9223372036854775807, 1));
}

TEST(Quantize, LongMinMax) {
  EXPECT_LEQ(0, quantized_long_min_value(0, 1));
  EXPECT_LEQ(0, quantized_long_max_value(0, 1));
  EXPECT_LEQ(0, quantized_long_min_value(0, 5));
  EXPECT_LEQ(0, quantized_long_max_value(0, 5));
  EXPECT_LEQ(1, quantized_long_min_value(1, 1));
  EXPECT_LEQ(1, quantized_long_max_value(1, 1));
  EXPECT_LEQ(1, quantized_long_min_value(1, 5));
  EXPECT_LEQ(1, quantized_long_max_value(1, 5));
  EXPECT_LEQ(-1, quantized_long_min_value(-1, 1));
  EXPECT_LEQ(-1, quantized_long_max_value(-1, 1));
  EXPECT_LEQ(-1, quantized_long_min_value(-1, 5));
  EXPECT_LEQ(-1, quantized_long_max_value(-1, 5));
  EXPECT_LEQ(10, quantized_long_min_value(10, 1));
  EXPECT_LEQ(19, quantized_long_max_value(10, 1));
  EXPECT_LEQ(19, quantized_long_min_value(19, 2));
  EXPECT_LEQ(19, quantized_long_min_value(19, 2));
  EXPECT_LEQ(10, quantized_long_min_value(10, 2));
  EXPECT_LEQ(10, quantized_long_max_value(10, 2));
  EXPECT_LEQ(10, quantized_long_min_value(10, 5));
  EXPECT_LEQ(10, quantized_long_max_value(10, 5));
  EXPECT_LEQ(-19, quantized_long_min_value(-10, 1));
  EXPECT_LEQ(-10, quantized_long_max_value(-10, 1));
  EXPECT_LEQ(-19, quantized_long_min_value(-19, 2));
  EXPECT_LEQ(-19, quantized_long_min_value(-19, 2));
  EXPECT_LEQ(-10, quantized_long_min_value(-10, 2));
  EXPECT_LEQ(-10, quantized_long_max_value(-10, 2));
  EXPECT_LEQ(-10, quantized_long_min_value(-10, 5));
  EXPECT_LEQ(-10, quantized_long_max_value(-10, 5));
  EXPECT_LEQ(90, quantized_long_min_value(90, 1));
  EXPECT_LEQ(99, quantized_long_max_value(90, 1));
  EXPECT_LEQ(99, quantized_long_min_value(99, 2));
  EXPECT_LEQ(99, quantized_long_min_value(99, 2));
  EXPECT_LEQ(90, quantized_long_min_value(90, 2));
  EXPECT_LEQ(90, quantized_long_max_value(90, 2));
  EXPECT_LEQ(90, quantized_long_min_value(90, 5));
  EXPECT_LEQ(90, quantized_long_max_value(90, 5));
  EXPECT_LEQ(-99, quantized_long_min_value(-90, 1));
  EXPECT_LEQ(-90, quantized_long_max_value(-90, 1));
  EXPECT_LEQ(-99, quantized_long_min_value(-99, 2));
  EXPECT_LEQ(-99, quantized_long_min_value(-99, 2));
  EXPECT_LEQ(-90, quantized_long_min_value(-90, 2));
  EXPECT_LEQ(-90, quantized_long_max_value(-90, 2));
  EXPECT_LEQ(-90, quantized_long_min_value(-90, 5));
  EXPECT_LEQ(-90, quantized_long_max_value(-90, 5));
  EXPECT_LEQ(
      9000000000000000000,
      quantized_long_min_value(9000000000000000000, 1));
  EXPECT_LEQ(
      LONG_MAX,
      quantized_long_max_value(9000000000000000000, 1));
  EXPECT_LEQ(
      -LONG_MAX,
      quantized_long_min_value(-9000000000000000000, 1));
  EXPECT_LEQ(
      -9000000000000000000,
      quantized_long_max_value(-9000000000000000000, 1));
  EXPECT_LEQ(
      9220000000000000000,
      quantized_long_min_value(9220000000000000000, 3));
  EXPECT_LEQ(
      LONG_MAX,
      quantized_long_max_value(9220000000000000000, 3));
  EXPECT_LEQ(
      -LONG_MAX,
      quantized_long_min_value(-9220000000000000000, 3));
  EXPECT_LEQ(
      -9220000000000000000,
      quantized_long_max_value(-9220000000000000000, 3));
  EXPECT_LEQ(
      8000000000000000000,
      quantized_long_min_value(8000000000000000000, 1));
  EXPECT_LEQ(
      8999999999999999999,
      quantized_long_max_value(8000000000000000000, 1));
  EXPECT_LEQ(
      -8999999999999999999,
      quantized_long_min_value(-8000000000000000000, 1));
  EXPECT_LEQ(
      -8000000000000000000,
      quantized_long_max_value(-8000000000000000000, 1));
  EXPECT_LEQ(
      8000000000000000000,
      quantized_long_min_value(8000000000000000000, 5));
  EXPECT_LEQ(
      8000099999999999999,
      quantized_long_max_value(8000000000000000000, 5));
  EXPECT_LEQ(
      -8000099999999999999,
      quantized_long_min_value(-8000000000000000000, 5));
  EXPECT_LEQ(
      -8000000000000000000,
      quantized_long_max_value(-8000000000000000000, 5));
  EXPECT_LEQ(123000000, quantized_long_min_value(123000000, 3));
  EXPECT_LEQ(123999999, quantized_long_max_value(123000000, 3));
  EXPECT_LEQ(-123999999, quantized_long_min_value(-123000000, 3));
  EXPECT_LEQ(-123000000, quantized_long_max_value(-123000000, 3));
}

TEST(Quantize, Double) {
  double pi = 3.141592653589793238;
  double q;

  EXPECT_DEQ(0, quantize_double(0, 0, 1));
  EXPECT_DEQ(0, quantize_double(0, 0, 2));
  EXPECT_DEQ(0, quantize_double(0, 0, 5));
  EXPECT_DEQ(0, quantize_double(0, 0, 10));
  EXPECT_DEQ(0, quantize_double(0, 0, 100));
  EXPECT_DEQ(1, quantize_double(1, 0, 1));
  EXPECT_DEQ(1, quantize_double(1, 0, 2));
  EXPECT_DEQ(1, quantize_double(1, 0, 5));
  EXPECT_DEQ(1, quantize_double(1, 0, 10));
  EXPECT_DEQ(1, quantize_double(1, 0, 100));
  EXPECT_DEQ(-1, quantize_double(-1, 0, 1));
  EXPECT_DEQ(-1, quantize_double(-1, 0, 2));
  EXPECT_DEQ(-1, quantize_double(-1, 0, 5));
  EXPECT_DEQ(-1, quantize_double(-1, 0, 10));
  EXPECT_DEQ(-1, quantize_double(-1, 0, 100));
  EXPECT_DEQ(1, quantize_double(1, 0, 1));
  EXPECT_DEQ(3, quantize_double(pi, 0, 1));
  EXPECT_DEQ(3.1, quantize_double(pi, 0, 2));
  EXPECT_DEQ(3.1415, quantize_double(pi, 0, 5));
  EXPECT_DEQ(3.14159265358979, q = quantize_double(pi, 0, 15));
  EXPECT_DEQ(3.1415926535897, q = quantize_double(q, 15, 14));
  EXPECT_DEQ(3.141592653589, q = quantize_double(q, 14, 13));
  EXPECT_DEQ(3.14159265358, q = quantize_double(q, 14, 12));
  EXPECT_DEQ(3.1415926535, q = quantize_double(q, 14, 11));
  EXPECT_DEQ(3.141592653, q = quantize_double(q, 14, 10));
  EXPECT_DEQ(3.14159265, q = quantize_double(q, 14, 9));
  EXPECT_DEQ(3.1415926, q = quantize_double(q, 14, 8));
  EXPECT_DEQ(3.141592, q = quantize_double(q, 14, 7));
  EXPECT_DEQ(3.14159, q = quantize_double(q, 14, 6));
  EXPECT_DEQ(3.1415, q = quantize_double(q, 14, 5));
  EXPECT_DEQ(3.141, q = quantize_double(q, 14, 4));
  EXPECT_DEQ(3.14, q = quantize_double(q, 14, 3));
  EXPECT_DEQ(3.1, q = quantize_double(q, 14, 2));
  EXPECT_DEQ(3, q = quantize_double(q, 14, 1));
  EXPECT_DEQ(
      -0.00999999999999999,
      q = quantize_double(-0.00999999999999999, 0, 15));
  EXPECT_DEQ(-0.0099999999999999, q = quantize_double(q, 15, 14));
  EXPECT_DEQ(-0.009999999999999, q = quantize_double(q, 14, 13));
  EXPECT_DEQ(-0.00999999999999, q = quantize_double(q, 13, 12));
  EXPECT_DEQ(-0.0099999999999, q = quantize_double(q, 12, 11));
  EXPECT_DEQ(-0.009999999999, q = quantize_double(q, 11, 10));
  EXPECT_DEQ(-0.00999999999, q = quantize_double(q, 10, 9));
  EXPECT_DEQ(-0.0099999999, q = quantize_double(q, 9, 8));
  EXPECT_DEQ(-0.009999999, q = quantize_double(q, 8, 7));
  EXPECT_DEQ(-0.00999999, q = quantize_double(q, 7, 6));
  EXPECT_DEQ(-0.0099999, q = quantize_double(q, 6, 5));
  EXPECT_DEQ(-0.009999, q = quantize_double(q, 5, 4));
  EXPECT_DEQ(-0.00999, q = quantize_double(q, 4, 3));
  EXPECT_DEQ(-0.0099, q = quantize_double(q, 3, 2));
  EXPECT_DEQ(-0.009, q = quantize_double(q, 2, 1));
  EXPECT_DEQ(
      0.009999999999999,
      q = quantize_double(0.00999999999999999, 0, 13));
  EXPECT_DEQ(0.009, q = quantize_double(q, 13, 1));
  EXPECT_DEQ(
      1.2345678901234e+34,
      q = quantize_double(12345.6789012345678e30, 0, 14));
  EXPECT_DEQ(1.23456789012e+34, q = quantize_double(q, 14, 12));
  EXPECT_DEQ(1.23456789e+34, q = quantize_double(q, 12, 9));
  EXPECT_DEQ(1.2345e+34, q = quantize_double(q, 12, 5));
  EXPECT_DEQ(
      -1.09010900109991e-234,
      q = quantize_double(-1.090109001099909e-234, 0, 15));
  EXPECT_DEQ(-1.09010900109991e-234, q = quantize_double(q, 15, 15));
  EXPECT_DEQ(-1.0901090010999e-234, q = quantize_double(q, 15, 14));
  EXPECT_DEQ(-1.0901090010999e-234, q = quantize_double(q, 14, 14));
  EXPECT_DEQ(-1.090109001099e-234, q = quantize_double(q, 14, 13));
  EXPECT_DEQ(-1.0901090010e-234, q = quantize_double(q, 13, 11));
  EXPECT_DEQ(-1.0901090e-234, q = quantize_double(q, 11, 8));
  EXPECT_DEQ(-1.090e-234, q = quantize_double(q, 8, 4));
  EXPECT_DEQ(-1.0e-234, q = quantize_double(q, 4, 2));
  EXPECT_DEQ(-1e-234, q = quantize_double(q, 2, 1));
}

TEST(Quantize, DoubleMinMax) {
  EXPECT_DEQ(0, quantized_double_min_value(0, 1));
  EXPECT_DEQ(0, quantized_double_max_value(0, 1));
  EXPECT_DEQ(0, quantized_double_min_value(0, 2));
  EXPECT_DEQ(0, quantized_double_max_value(0, 2));
  EXPECT_DEQ(0, quantized_double_min_value(0, 10));
  EXPECT_DEQ(0, quantized_double_max_value(0, 10));
  EXPECT_NEAR(
      1.2,
      quantized_double_min_value(quantize_double(1.2468, 0, 2), 2), 0);
  EXPECT_NEAR(
      1.3,
      quantized_double_max_value(quantize_double(1.2468, 0, 2), 2), 0);
  EXPECT_NEAR(
      1.24e-6,
      quantized_double_min_value(quantize_double(1.2468e-6, 0, 3), 3), 0);
  EXPECT_NEAR(
      1.25e-6,
      quantized_double_max_value(quantize_double(1.2468e-6, 0, 3), 3), 1e-21);
  EXPECT_NEAR(
      1.246e3,
      quantized_double_min_value(quantize_double(1.2468e3, 0, 4), 4), 1e-21);
  EXPECT_NEAR(
      1.247e3,
      quantized_double_max_value(quantize_double(1.2468e3, 0, 4), 4), 1e-21);
  EXPECT_NEAR(
      -1.3,
      quantized_double_min_value(quantize_double(-1.2468, 0, 2), 2), 1e-21);
  EXPECT_NEAR(
      -1.2,
      quantized_double_max_value(quantize_double(-1.2468, 0, 2), 2), 1e-21);
  EXPECT_NEAR(
      -1.25e-6,
      quantized_double_min_value(quantize_double(-1.2468e-6, 0, 3), 3), 1e-21);
  EXPECT_NEAR(
      -1.24e-6,
      quantized_double_max_value(quantize_double(-1.2468e-6, 0, 3), 3), 1e-21);
  EXPECT_NEAR(
      -1.247e3,
      quantized_double_min_value(quantize_double(-1.2468e3, 0, 4), 4), 1e-21);
  EXPECT_NEAR(
      -1.246e3,
      quantized_double_max_value(quantize_double(-1.2468e3, 0, 4), 4), 1e-21);
}

FINISH
