#include "bits.h"

byte ulong_compute_bit_width(ulong u) {

  byte width;

  for (width = 0; u != 0; u >>= 1, width++) {
  }

  return width;
}

byte ulong_bit_width(ulong u) {
  if ((u & 0xffffffff00000000) != 0) {
    return ulong_bit_width(u >> 32) + 32;
  }
  return uint_bit_width((uint)u);
}

byte uint_bit_width(uint u) {
  if ((u & 0xffff0000) != 0) {
    return ushort_bit_width(u >> 16) + 16;
  }
  return ushort_bit_width((ushort)u);
}

byte ushort_bit_width(ushort u) {
  static Lock lock_ = LOCK_INITIALIZER;
  static bool initialized_ = false;
  static byte widths_[0x10000];
  byte width;
  uint i;
  uint j;

  if (!initialized_) {
    lock(&lock_);
    if (!initialized_) {
      widths_[0] = width = 0;
      for (j = 0, i = 1; i < 0x10000; j = i, i++) {
        if ((i & j) == 0) {
          width++;
        }
        widths_[i] = width;
      }
      initialized_ = true;
    }
    unlock(&lock_);
  }

  return widths_[u];
}

byte byte_bit_width(byte b) {
  return ushort_bit_width(b);
}

byte ulong_hexit_width(ulong u) {
  return (ulong_bit_width(u) + 3) >> 2;
}
