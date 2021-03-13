#include "numbers_test_death.h"

START_DEATH

TEST(Parse, Byte) {
  EXPECT_DEATH(parse_byte("-1"), "Couldn't parse '-1' as a byte");
  EXPECT_DEATH(parse_byte("+ 1"), "Couldn't parse '+ 1' as a byte");
  EXPECT_DEATH(parse_byte("1+"), "Couldn't parse '1+' as a byte");
  EXPECT_DEATH(parse_byte("256"), "Couldn't parse '256' as a byte");
  EXPECT_DEATH(
      parse_byte("12334875643875643876587346565536"),
      "Couldn't parse '12,334,875,643,875,643,876,587,346,565,536' as a byte");
  EXPECT_DEATH(parse_byte("12 34"), "Couldn't parse '12 34' as a byte");
  EXPECT_DEATH(parse_byte("12.7"), "Couldn't parse '12.7' as a byte");
  EXPECT_DEATH(parse_byte("4."), "Couldn't parse '4.' as a byte");
  EXPECT_DEATH(parse_byte("5e3"), "Couldn't parse '5e3' as a byte");
}

TEST(Parse, Ushort) {
  EXPECT_DEATH(parse_ushort("-1"), "Couldn't parse '-1' as a ushort");
  EXPECT_DEATH(parse_ushort("+ 1"), "Couldn't parse '+ 1' as a ushort");
  EXPECT_DEATH(parse_ushort("1+"), "Couldn't parse '1+' as a ushort");
  EXPECT_DEATH(parse_ushort("65536"), "Couldn't parse '65,536' as a ushort");
  EXPECT_DEATH(
      parse_ushort("12334875643875643876587346565536"),
      "Couldn't parse '12,334,875,643,875,643,876,587,346,565,536' "
      "as a ushort");
  EXPECT_DEATH(parse_ushort("12 34"), "Couldn't parse '12 34' as a ushort");
  EXPECT_DEATH(parse_ushort("12.7"), "Couldn't parse '12.7' as a ushort");
  EXPECT_DEATH(parse_ushort("4."), "Couldn't parse '4.' as a ushort");
  EXPECT_DEATH(parse_ushort("5e3"), "Couldn't parse '5e3' as a ushort");
}

TEST(Parse, Uint) {
  EXPECT_DEATH(parse_uint("-1"), "Couldn't parse '-1' as a uint");
  EXPECT_DEATH(parse_uint("+ 1"), "Couldn't parse '+ 1' as a uint");
  EXPECT_DEATH(parse_uint("1+"), "Couldn't parse '1+' as a uint");
  EXPECT_DEATH(
      parse_uint("4294967296"),
      "Couldn't parse '4,294,967,296' as a uint");
  EXPECT_DEATH(
      parse_uint("12334875643875643876587346565536"),
      "Couldn't parse '12,334,875,643,875,643,876,587,346,565,536' as a uint");
  EXPECT_DEATH(parse_uint("12 34"), "Couldn't parse '12 34' as a uint");
  EXPECT_DEATH(parse_uint("12.7"), "Couldn't parse '12.7' as a uint");
  EXPECT_DEATH(parse_uint("4."), "Couldn't parse '4.' as a uint");
  EXPECT_DEATH(parse_uint("5e3"), "Couldn't parse '5e3' as a uint");
}

TEST(Parse, Ulong) {
  EXPECT_DEATH(parse_ulong("-1"), "Couldn't parse '-1' as a ulong");
  EXPECT_DEATH(parse_ulong("+ 1"), "Couldn't parse '+ 1' as a ulong");
  EXPECT_DEATH(parse_ulong("1+"), "Couldn't parse '1+' as a ulong");
  EXPECT_DEATH(
      parse_ulong("18446744073709551616"),
      "Couldn't parse '18,446,744,073,709,551,616' as a ulong");
  EXPECT_DEATH(
      parse_ulong("12334875643875643876587346565536"),
      "Couldn't parse '12,334,875,643,875,643,876,587,346,565,536' as a ulong");
  EXPECT_DEATH(parse_ulong("12 34"), "Couldn't parse '12 34' as a ulong");
  EXPECT_DEATH(parse_ulong("12.7"), "Couldn't parse '12.7' as a ulong");
  EXPECT_DEATH(parse_ulong("4."), "Couldn't parse '4.' as a ulong");
  EXPECT_DEATH(parse_ulong("5e3"), "Couldn't parse '5e3' as a ulong");
}

TEST(Parse, Short) {
  EXPECT_DEATH(parse_short("-+1"), "Couldn't parse '-+1' as a short");
  EXPECT_DEATH(parse_short("- 1"), "Couldn't parse '- 1' as a short");
  EXPECT_DEATH(parse_short("1-"), "Couldn't parse '1-' as a short");
  EXPECT_DEATH(parse_short("32768"), "Couldn't parse '32,768' as a short");
  EXPECT_DEATH(
      parse_short("12334875643875643876587346565536"),
      "Couldn't parse '12,334,875,643,875,643,876,587,346,565,536' as a short");
  EXPECT_DEATH(parse_short("12 34"), "Couldn't parse '12 34' as a short");
  EXPECT_DEATH(parse_short("12.7"), "Couldn't parse '12.7' as a short");
  EXPECT_DEATH(parse_short("4."), "Couldn't parse '4.' as a short");
  EXPECT_DEATH(parse_short("5e3"), "Couldn't parse '5e3' as a short");
}

TEST(Parse, Int) {
  EXPECT_DEATH(parse_int("-+1"), "Couldn't parse '-+1' as an int");
  EXPECT_DEATH(parse_int("- 1"), "Couldn't parse '- 1' as an int");
  EXPECT_DEATH(parse_int("1-"), "Couldn't parse '1-' as an int");
  EXPECT_DEATH(
      parse_int("2147483648"),
      "Couldn't parse '2,147,483,648' as an int");
  EXPECT_DEATH(
      parse_int("12334875643875643876587346565536"),
      "Couldn't parse '12,334,875,643,875,643,876,587,346,565,536' as an int");
  EXPECT_DEATH(parse_int("12 34"), "Couldn't parse '12 34' as an int");
  EXPECT_DEATH(parse_int("12.7"), "Couldn't parse '12.7' as an int");
  EXPECT_DEATH(parse_int("4."), "Couldn't parse '4.' as an int");
  EXPECT_DEATH(parse_int("5e3"), "Couldn't parse '5e3' as an int");
}

TEST(Parse, Long) {
  EXPECT_DEATH(parse_long("-+1"), "Couldn't parse '-+1' as a long");
  EXPECT_DEATH(parse_long("- 1"), "Couldn't parse '- 1' as a long");
  EXPECT_DEATH(parse_long("1-"), "Couldn't parse '1-' as a long");
  EXPECT_DEATH(
      parse_long("9223372036854775808"),
      "Couldn't parse '9,223,372,036,854,775,808' as a long");
  EXPECT_DEATH(
      parse_long("12334875643875643876587346565536"),
      "Couldn't parse '12,334,875,643,875,643,876,587,346,565,536' as a long");
  EXPECT_DEATH(parse_long("12 34"), "Couldn't parse '12 34' as a long");
  EXPECT_DEATH(parse_long("12.7"), "Couldn't parse '12.7' as a long");
  EXPECT_DEATH(parse_long("4."), "Couldn't parse '4.' as a long");
  EXPECT_DEATH(parse_long("5e3"), "Couldn't parse '5e3' as a long");
}

TEST(Parse, Double) {
  EXPECT_DEATH(
      parse_double("1.797693134862317e+308"),
      "Couldn't parse '1.797693134862317e+308' as a double");
  EXPECT_DEATH(
      parse_double("-1.797693134862317e+308"),
      "Couldn't parse '-1.797693134862317e+308' as a double");
  EXPECT_DEATH(parse_double("-+1"), "Couldn't parse '-+1' as a double");
  EXPECT_DEATH(parse_double("+-1"), "Couldn't parse '+-1' as a double");
  EXPECT_DEATH(parse_double("1g3"), "Couldn't parse '1g3' as a double");
  EXPECT_DEATH(parse_double("1e-+2"), "Couldn't parse '1e-+2' as a double");
  EXPECT_DEATH(parse_double("1e+-2"), "Couldn't parse '1e+-2' as a double");
  EXPECT_DEATH(parse_double("1.34a"), "Couldn't parse '1.34a' as a double");
  EXPECT_DEATH(parse_double("1.23.4"), "Couldn't parse '1.23.4' as a double");
}

TEST(Quantize, LongMinMax) {
  EXPECT_DEATH(
      quantized_long_min_value(123, 0),
      "Number of significant figures cannot be zero");
  EXPECT_DEATH(
      quantized_long_min_value(123, 2),
      "123 was not quantized to 2 significant figures");
  EXPECT_DEATH(
      quantized_long_min_value(-123, 2),
      "-123 was not quantized to 2 significant figures");
  EXPECT_DEATH(
      quantized_long_min_value(123450000, 4),
      "123,450,000 was not quantized to 4 significant figures");
  EXPECT_DEATH(
      quantized_long_min_value(123450000, 3),
      "123,450,000 was not quantized to 3 significant figures");
  EXPECT_DEATH(
      quantized_long_min_value(123450000, 2),
      "123,450,000 was not quantized to 2 significant figures");
  EXPECT_DEATH(
      quantized_long_min_value(123450000, 1),
      "123,450,000 was not quantized to 1 significant figure");
  EXPECT_DEATH(
      quantized_long_min_value(-123450000, 4),
      "-123,450,000 was not quantized to 4 significant figures");
  EXPECT_DEATH(
      quantized_long_min_value(-123450000, 3),
      "-123,450,000 was not quantized to 3 significant figures");
  EXPECT_DEATH(
      quantized_long_min_value(-123450000, 2),
      "-123,450,000 was not quantized to 2 significant figures");
  EXPECT_DEATH(
      quantized_long_min_value(-123450000, 1),
      "-123,450,000 was not quantized to 1 significant figure");
}

TEST(Quantize, Double) {
  EXPECT_DEATH(
      quantize_double(1, 0, 0),
      "Number of significant figures cannot be zero");
  EXPECT_DEATH(
      quantize_double(1, 5, 6),
      "Number of significant figures cannot be increased from 5 to 6");
}

TEST(Quantize, DoubleMinMax) {
  EXPECT_DEATH(
      quantized_double_min_value(0, 0),
      "Number of significant figures cannot be zero");
  EXPECT_DEATH(
      quantized_double_max_value(0, 0),
      "Number of significant figures cannot be zero");
  EXPECT_DEATH(
      quantized_double_min_value(1, 0),
      "Number of significant figures cannot be zero");
  EXPECT_DEATH(
      quantized_double_max_value(1, 0),
      "Number of significant figures cannot be zero");
  EXPECT_DEATH(
      quantized_double_min_value(1.2345, 3),
      "1.23449999999999993e+00 is not quantized to 3 significant figures; "
      "should be 1.22999999999999998e+00");
  EXPECT_DEATH(
      quantized_double_max_value(1.2345, 3),
      "1.23449999999999993e+00 is not quantized to 3 significant figures; "
      "should be 1.22999999999999998e+00");
  EXPECT_DEATH(
      quantized_double_min_value(-1.2345, 3),
      "-1.23449999999999993e+00 is not quantized to 3 significant figures; "
      "should be -1.22999999999999998e+00");
  EXPECT_DEATH(
      quantized_double_max_value(-1.2345, 3),
      "-1.23449999999999993e+00 is not quantized to 3 significant figures; "
      "should be -1.22999999999999998e+00");
}

FINISH_DEATH
