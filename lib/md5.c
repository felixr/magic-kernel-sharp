#include "md5.h"
#include <string.h>

#define LEFTROTATE_(x, c) (((x) << (c)) | ((x) >> (32 - (c))))

typedef union {
  byte b[8];
  ulong u;
} Converter_;

static const uint k_[] = {
  0xd76aa478, 0xe8c7b756, 0x242070db, 0xc1bdceee,
  0xf57c0faf, 0x4787c62a, 0xa8304613, 0xfd469501,
  0x698098d8, 0x8b44f7af, 0xffff5bb1, 0x895cd7be,
  0x6b901122, 0xfd987193, 0xa679438e, 0x49b40821,
  0xf61e2562, 0xc040b340, 0x265e5a51, 0xe9b6c7aa,
  0xd62f105d, 0x02441453, 0xd8a1e681, 0xe7d3fbc8,
  0x21e1cde6, 0xc33707d6, 0xf4d50d87, 0x455a14ed,
  0xa9e3e905, 0xfcefa3f8, 0x676f02d9, 0x8d2a4c8a,
  0xfffa3942, 0x8771f681, 0x6d9d6122, 0xfde5380c,
  0xa4beea44, 0x4bdecfa9, 0xf6bb4b60, 0xbebfbc70,
  0x289b7ec6, 0xeaa127fa, 0xd4ef3085, 0x04881d05,
  0xd9d4d039, 0xe6db99e5, 0x1fa27cf8, 0xc4ac5665,
  0xf4292244, 0x432aff97, 0xab9423a7, 0xfc93a039,
  0x655b59c3, 0x8f0ccc92, 0xffeff47d, 0x85845dd1,
  0x6fa87e4f, 0xfe2ce6e0, 0xa3014314, 0x4e0811a1,
  0xf7537e82, 0xbd3af235, 0x2ad7d2bb, 0xeb86d391
};

static const uint r_[] = {
  7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22,
  5,  9, 14, 20, 5,  9, 14, 20, 5,  9, 14, 20, 5,  9, 14, 20,
  4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23,
  6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21
};

static void to_bytes_(uint val, byte *bytes);
static uint to_uint_(byte *bytes);

char *string_new_md5(char **pthis, ulong len, byte *blob) {
  byte md5[16];

  md5_bytes(len, blob, md5);
  return string_new_md5_from_bytes(pthis, md5);
}

void md5_ulongs(ulong len, byte *blob, ulong *hi, ulong *lo) {
  byte md5[16];

  md5_bytes(len, blob, md5);
  md5_bytes_to_ulongs(md5, hi, lo);
}

void md5_bytes(ulong len, byte *blob, byte *md5) {
  byte *msg = null;
  uint h0;
  uint h1;
  uint h2;
  uint h3;
  uint a;
  uint b;
  uint c;
  uint d;
  uint i;
  uint f;
  uint g;
  uint temp;
  uint w[16];
  ulong new_len;
  ulong offset;

  h0 = 0x67452301;
  h1 = 0xefcdab89;
  h2 = 0x98badcfe;
  h3 = 0x10325476;

  for (new_len = len + 1; new_len % (512 / 8) != 448 / 8; new_len++) {
  }

  new(&msg, new_len + 8);
  memcpy(msg, blob, len);
  msg[len] = 0x80;
  for (offset = len + 1; offset < new_len; offset++) {
    msg[offset] = 0;
  }

  to_bytes_(len * 8, msg + new_len);
  to_bytes_(len >> 29, msg + new_len + 4);

  for (offset = 0; offset < new_len; offset += 512 / 8) {
    for (i = 0; i < 16; i++) {
      w[i] = to_uint_(msg + offset + i * 4);
    }

    a = h0;
    b = h1;
    c = h2;
    d = h3;

    for (i = 0; i < 64; i++) {
      if (i < 16) {
        f = (b & c) | ((~b) & d);
        g = i;
      } else if (i < 32) {
        f = (d & b) | ((~d) & c);
        g = (5 * i + 1) % 16;
      } else if (i < 48) {
        f = b ^ c ^ d;
        g = (3 * i + 5) % 16;
      } else {
        f = c ^ (b | (~d));
        g = (7 * i) % 16;
      }

      temp = d;
      d = c;
      c = b;
      b = b + LEFTROTATE_((a + f + k_[i] + w[g]), r_[i]);
      a = temp;
    }

    h0 += a;
    h1 += b;
    h2 += c;
    h3 += d;
  }

  deletev(&msg);

  to_bytes_(h0, md5);
  to_bytes_(h1, md5 + 4);
  to_bytes_(h2, md5 + 8);
  to_bytes_(h3, md5 + 12);
}

void md5_bytes_to_ulongs(byte *md5, ulong *hi, ulong *lo) {
  byte i;
  Converter_ c;

  for (i = 0; i < 8; i++) {
    c.b[7 - i] = md5[i];
  }
  *hi = c.u;
  for (i = 0; i < 8; i++) {
    c.b[7 - i] = md5[i + 8];
  }
  *lo = c.u;
}

char *string_new_md5_from_ulongs(char **pthis, ulong hi, ulong lo) {
  return string_new_f(pthis, "%016lx%016lx", hi, lo);
}

char *string_new_md5_from_bytes(char **pthis, byte *md5) {
  char *this;
  byte i;
  char *p;

  this = new(pthis, 33);
  for (i = 0, p = this; i < 16; i++, p+= 2) {
    sprintf(p, "%02x", md5[i]);
  }
  return this;
}

static void to_bytes_(uint val, byte *bytes) {
  bytes[0] = (byte)val;
  bytes[1] = (byte)(val >> 8);
  bytes[2] = (byte)(val >> 16);
  bytes[3] = (byte)(val >> 24);
}

static uint to_uint_(byte *bytes) {
  return
    ((uint)bytes[0]) |
    ((uint)bytes[1] << 8) |
    ((uint)bytes[2] << 16) |
    ((uint)bytes[3] << 24);
}
