#include "bits_test.h"

START

TEST(Ulong, BitWidth) {
  EXPECT_ULEQ(0, ulong_bit_width(0));
  EXPECT_ULEQ(1, ulong_bit_width(1));
  EXPECT_ULEQ(2, ulong_bit_width(2));
  EXPECT_ULEQ(2, ulong_bit_width(3));
  EXPECT_ULEQ(3, ulong_bit_width(4));
  EXPECT_ULEQ(3, ulong_bit_width(7));
  EXPECT_ULEQ(4, ulong_bit_width(8));
  EXPECT_ULEQ(4, ulong_bit_width(0xf));
  EXPECT_ULEQ(5, ulong_bit_width(0x10));
  EXPECT_ULEQ(5, ulong_bit_width(0x1f));
  EXPECT_ULEQ(8, ulong_bit_width(0xff));
  EXPECT_ULEQ(9, ulong_bit_width(0x100));
  EXPECT_ULEQ(16, ulong_bit_width(0xffff));
  EXPECT_ULEQ(17, ulong_bit_width(0x10000));
  EXPECT_ULEQ(32, ulong_bit_width(0xffffffff));
  EXPECT_ULEQ(33, ulong_bit_width(0x100000000));
  EXPECT_ULEQ(64, ulong_bit_width(0x8000000000000000));
  EXPECT_ULEQ(64, ulong_bit_width(0xffffffffffffffff));
}

TEST(Uint, BitWidth) {
  EXPECT_ULEQ(0, uint_bit_width(0));
  EXPECT_ULEQ(1, uint_bit_width(1));
  EXPECT_ULEQ(2, uint_bit_width(2));
  EXPECT_ULEQ(2, uint_bit_width(3));
  EXPECT_ULEQ(3, uint_bit_width(4));
  EXPECT_ULEQ(3, uint_bit_width(7));
  EXPECT_ULEQ(4, uint_bit_width(8));
  EXPECT_ULEQ(4, uint_bit_width(0xf));
  EXPECT_ULEQ(5, uint_bit_width(0x10));
  EXPECT_ULEQ(5, uint_bit_width(0x1f));
  EXPECT_ULEQ(8, uint_bit_width(0xff));
  EXPECT_ULEQ(9, uint_bit_width(0x100));
  EXPECT_ULEQ(16, uint_bit_width(0xffff));
  EXPECT_ULEQ(17, uint_bit_width(0x10000));
  EXPECT_ULEQ(32, uint_bit_width(0x80000000));
  EXPECT_ULEQ(32, uint_bit_width(0xffffffff));
}

TEST(Ushort, BitWidth) {
  EXPECT_ULEQ(0, ushort_bit_width(0));
  EXPECT_ULEQ(1, ushort_bit_width(1));
  EXPECT_ULEQ(2, ushort_bit_width(2));
  EXPECT_ULEQ(2, ushort_bit_width(3));
  EXPECT_ULEQ(3, ushort_bit_width(4));
  EXPECT_ULEQ(3, ushort_bit_width(7));
  EXPECT_ULEQ(4, ushort_bit_width(8));
  EXPECT_ULEQ(4, ushort_bit_width(0xf));
  EXPECT_ULEQ(5, ushort_bit_width(0x10));
  EXPECT_ULEQ(5, ushort_bit_width(0x1f));
  EXPECT_ULEQ(8, ushort_bit_width(0xff));
  EXPECT_ULEQ(9, ushort_bit_width(0x100));
  EXPECT_ULEQ(16, ushort_bit_width(0x8000));
  EXPECT_ULEQ(16, ushort_bit_width(0xffff));
}

TEST(Byte, BitWidth) {
  EXPECT_ULEQ(0, byte_bit_width(0));
  EXPECT_ULEQ(1, byte_bit_width(1));
  EXPECT_ULEQ(2, byte_bit_width(2));
  EXPECT_ULEQ(2, byte_bit_width(3));
  EXPECT_ULEQ(3, byte_bit_width(4));
  EXPECT_ULEQ(3, byte_bit_width(7));
  EXPECT_ULEQ(4, byte_bit_width(8));
  EXPECT_ULEQ(4, byte_bit_width(0xf));
  EXPECT_ULEQ(5, byte_bit_width(0x10));
  EXPECT_ULEQ(5, byte_bit_width(0x1f));
  EXPECT_ULEQ(6, byte_bit_width(0x20));
  EXPECT_ULEQ(6, byte_bit_width(0x3f));
  EXPECT_ULEQ(7, byte_bit_width(0x40));
  EXPECT_ULEQ(7, byte_bit_width(0x7f));
  EXPECT_ULEQ(8, byte_bit_width(0x80));
  EXPECT_ULEQ(8, byte_bit_width(0xff));
}

TEST(Ulong, HexitWidth) {
  EXPECT_ULEQ(0, ulong_hexit_width(0));
  EXPECT_ULEQ(1, ulong_hexit_width(1));
  EXPECT_ULEQ(1, ulong_hexit_width(0xf));
  EXPECT_ULEQ(2, ulong_hexit_width(0x10));
  EXPECT_ULEQ(2, ulong_hexit_width(0xff));
  EXPECT_ULEQ(3, ulong_hexit_width(0x100));
  EXPECT_ULEQ(3, ulong_hexit_width(0xfff));
  EXPECT_ULEQ(4, ulong_hexit_width(0x1000));
  EXPECT_ULEQ(4, ulong_hexit_width(0xffff));
  EXPECT_ULEQ(5, ulong_hexit_width(0x10000));
  EXPECT_ULEQ(8, ulong_hexit_width(0xffffffff));
  EXPECT_ULEQ(9, ulong_hexit_width(0x100000000));
  EXPECT_ULEQ(16, ulong_hexit_width(0xffffffffffffffff));
}

FINISH
