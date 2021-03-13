#include "MemoryFileTest.h"

static byte hash_(ulong u) {
  byte b[16];
  union {
    ulong u;
    byte b[8];
  } converter;

  converter.u = u;
  md5_bytes(8, converter.b, b);
  return b[0];
}

static void check_(MemoryFile *mf) {
  ulong size;
  ulong u;

  size = memory_file_get_size(mf);
  memory_file_rewind(mf);
  for (u = 0; u < size; u++) {
    EXPECT_ULEQ(u, memory_file_get_position(mf));
    EXPECT_ULEQ(size, memory_file_get_size(mf));
    EXPECT_ULEQ(hash_(u), memory_file_read_byte(mf));
  }
}

static void test_transition_(MemoryFile *mf, ulong start) {
  ulong u;

  while ((u = memory_file_get_position(mf)) < 15) {
    memory_file_write_byte(mf, hash_(u));
  }
  EXPECT_TRUE(memory_file_is_eof(mf));
  start = memory_file_get_position(mf);
  check_(mf);
  memory_file_write_byte(mf, 'j');
  EXPECT_ULEQ(start + 1, memory_file_get_size(mf));
  EXPECT_ULEQ(start + 1, memory_file_get_position(mf));
  EXPECT_TRUE(memory_file_is_eof(mf));
  memory_file_set_position(mf, start);
  EXPECT_FALSE(memory_file_is_eof(mf));
  EXPECT_ULEQ(start + 1, memory_file_get_size(mf));
  EXPECT_ULEQ(start, memory_file_get_position(mf));
  EXPECT_ULEQ('j', memory_file_read_byte(mf));
  EXPECT_ULEQ(start + 1, memory_file_get_size(mf));
  EXPECT_ULEQ(start + 1, memory_file_get_position(mf));
  EXPECT_TRUE(memory_file_is_eof(mf));
  memory_file_write_byte(mf, 'p');
  EXPECT_ULEQ(start + 2, memory_file_get_size(mf));
  EXPECT_ULEQ(start + 2, memory_file_get_position(mf));
  memory_file_set_position(mf, start);
  EXPECT_ULEQ(start + 2, memory_file_get_size(mf));
  EXPECT_ULEQ(start, memory_file_get_position(mf));
  EXPECT_ULEQ('j', memory_file_read_byte(mf));
  EXPECT_ULEQ(start + 2, memory_file_get_size(mf));
  EXPECT_ULEQ(start + 1, memory_file_get_position(mf));
  EXPECT_ULEQ('p', memory_file_read_byte(mf));
  EXPECT_ULEQ(start + 2, memory_file_get_size(mf));
  EXPECT_ULEQ(start + 2, memory_file_get_position(mf));
  memory_file_write_byte(mf, 'c');
  EXPECT_ULEQ(start + 3, memory_file_get_size(mf));
  EXPECT_ULEQ(start + 3, memory_file_get_position(mf));
  memory_file_set_position(mf, start);
  EXPECT_ULEQ(start + 3, memory_file_get_size(mf));
  EXPECT_ULEQ(start, memory_file_get_position(mf));
  EXPECT_ULEQ('j', memory_file_read_byte(mf));
  EXPECT_ULEQ(start + 3, memory_file_get_size(mf));
  EXPECT_ULEQ(start + 1, memory_file_get_position(mf));
  EXPECT_ULEQ('p', memory_file_read_byte(mf));
  EXPECT_ULEQ(start + 3, memory_file_get_size(mf));
  EXPECT_ULEQ(start + 2, memory_file_get_position(mf));
  EXPECT_ULEQ('c', memory_file_read_byte(mf));
  EXPECT_ULEQ(start + 3, memory_file_get_size(mf));
  EXPECT_ULEQ(start + 3, memory_file_get_position(mf));
  memory_file_set_position(mf, start + 1);
  EXPECT_ULEQ('p', memory_file_read_byte(mf));
  EXPECT_ULEQ('c', memory_file_read_byte(mf));
  memory_file_set_position(mf, start);
  memory_file_write_byte(mf, hash_(start));
  memory_file_write_byte(mf, hash_(start + 1));
  memory_file_write_byte(mf, hash_(start + 2));
  check_(mf);
}

START

TEST(MemoryFile, Basics) {
  byte b;
  MemoryFile *mf = null;
  MemoryFile *ref = null;

  memory_file_new(&mf);

  /* Empty file.
   */
  EXPECT_ULEQ(0, memory_file_get_size(mf));
  EXPECT_ULEQ(0, memory_file_get_position(mf));

  /* Write first byte.
   */
  memory_file_write_byte(mf, 'j');
  EXPECT_ULEQ(1, memory_file_get_size(mf));
  EXPECT_ULEQ(1, memory_file_get_position(mf));
  memory_file_set_position(mf, 0);
  EXPECT_ULEQ(0, memory_file_get_position(mf));
  EXPECT_ULEQ('j', memory_file_read_byte(mf));
  EXPECT_FALSE(memory_file_try_read_byte(mf, &b));
  memory_file_set_position(mf, 1);

  /* Write second byte.
   */
  memory_file_write_byte(mf, 'p');
  EXPECT_ULEQ(2, memory_file_get_size(mf));
  EXPECT_ULEQ(2, memory_file_get_position(mf));
  memory_file_set_position(mf, 0);
  EXPECT_ULEQ(0, memory_file_get_position(mf));
  EXPECT_ULEQ('j', memory_file_read_byte(mf));
  EXPECT_ULEQ(1, memory_file_get_position(mf));
  EXPECT_ULEQ('p', memory_file_read_byte(mf));
  memory_file_set_position(mf, 2);
  EXPECT_FALSE(memory_file_try_read_byte(mf, &b));
  memory_file_set_position(mf, 1);
  EXPECT_ULEQ('p', memory_file_read_byte(mf));
  memory_file_set_position(mf, 0);
  EXPECT_ULEQ('j', memory_file_read_byte(mf));

  /* Write third byte.
   */
  memory_file_fast_forward(mf);
  memory_file_write_byte(mf, 'c');
  EXPECT_ULEQ(3, memory_file_get_size(mf));
  EXPECT_ULEQ(3, memory_file_get_position(mf));
  memory_file_set_position(mf, 0);
  EXPECT_ULEQ(0, memory_file_get_position(mf));
  EXPECT_ULEQ('j', memory_file_read_byte(mf));
  EXPECT_ULEQ(1, memory_file_get_position(mf));
  EXPECT_ULEQ('p', memory_file_read_byte(mf));
  EXPECT_ULEQ(2, memory_file_get_position(mf));
  EXPECT_ULEQ('c', memory_file_read_byte(mf));
  memory_file_set_position(mf, 3);
  EXPECT_FALSE(memory_file_try_read_byte(mf, &b));
  memory_file_set_position(mf, 2);
  EXPECT_ULEQ('c', memory_file_read_byte(mf));
  memory_file_set_position(mf, 1);
  EXPECT_ULEQ('p', memory_file_read_byte(mf));
  memory_file_set_position(mf, 0);
  EXPECT_ULEQ('j', memory_file_read_byte(mf));

  /* Overwrite second byte.
   */
  memory_file_set_position(mf, 1);
  memory_file_write_byte(mf, 'h');
  memory_file_rewind(mf);
  EXPECT_ULEQ('j', memory_file_read_byte(mf));
  EXPECT_ULEQ('h', memory_file_read_byte(mf));
  EXPECT_ULEQ('c', memory_file_read_byte(mf));

  /* Overwrite first byte.
   */
  memory_file_rewind(mf);
  memory_file_write_byte(mf, 'm');
  memory_file_rewind(mf);
  EXPECT_ULEQ('m', memory_file_read_byte(mf));
  EXPECT_ULEQ('h', memory_file_read_byte(mf));
  EXPECT_ULEQ('c', memory_file_read_byte(mf));

  /* Create a reference.
   */
  memory_file_new_reference(&ref, mf);

  /* Check its contents, and independently that of the original file.
   */
  memory_file_rewind(ref);
  memory_file_rewind(mf);
  EXPECT_ULEQ('m', memory_file_read_byte(ref));
  EXPECT_ULEQ('m', memory_file_read_byte(mf));
  EXPECT_ULEQ('h', memory_file_read_byte(ref));
  EXPECT_ULEQ('c', memory_file_read_byte(ref));
  EXPECT_ULEQ('h', memory_file_read_byte(mf));
  EXPECT_ULEQ('c', memory_file_read_byte(mf));

  /* Delete the reference.
   */
  memory_file_delete(&ref);

  /* Check the original.
   */
  memory_file_rewind(mf);
  EXPECT_ULEQ('m', memory_file_read_byte(mf));
  EXPECT_ULEQ('h', memory_file_read_byte(mf));
  EXPECT_ULEQ('c', memory_file_read_byte(mf));

  memory_file_delete(&mf);
}

TEST(MemoryFile, Transitions) {
  MemoryFile *mf = null;

  memory_file_new(&mf);

  /* It starts off with a 16-byte chunk.
   */
  test_transition_(mf, 0xf);

  /* Then doubles up to 64k.
   */
  test_transition_(mf, 0x1f);
  test_transition_(mf, 0x3f);
  test_transition_(mf, 0x7f);
  test_transition_(mf, 0xff);
  test_transition_(mf, 0x1ff);
  test_transition_(mf, 0x3ff);
  test_transition_(mf, 0x7ff);
  test_transition_(mf, 0xfff);
  test_transition_(mf, 0x1fff);
  test_transition_(mf, 0x3fff);
  test_transition_(mf, 0x7fff);
  test_transition_(mf, 0xffff);

  /* Then chunks of 64k.
   */
  test_transition_(mf, 0x1ffff);
  test_transition_(mf, 0x2ffff);
  test_transition_(mf, 0x3ffff);
  test_transition_(mf, 0x4ffff);
  memory_file_delete(&mf);
}

TEST(MemoryFile, ContentsEqual) {
  byte b;
  byte *bytes = null;
  ulong n;
  ulong u;
  MemoryFile *mf = null;

  n = 1024 * 1024;

  new(&bytes, n);
  memory_file_new(&mf);
  for (u = 0; u < n; u++) {
    b = hash_(u);
    bytes[u] = hash_(u);
    memory_file_write_byte(mf, b);
  }
  EXPECT_TRUE(memory_file_contents_equal_bytes(mf, bytes, n));
  memory_file_delete(&mf);
  deletev(&bytes);
}

TEST(MemoryFile, Extend) {
  ulong p;
  ulong s;
  MemoryFile *mf = null;

  memory_file_new(&mf);
  EXPECT_ULEQ(0, memory_file_get_size(mf));
  EXPECT_ULEQ(0, memory_file_get_position(mf));
  p = 1;
  memory_file_set_position(mf, p);
  EXPECT_ULEQ(p, memory_file_get_size(mf));
  EXPECT_ULEQ(p, memory_file_get_position(mf));
  p = 7;
  memory_file_set_position(mf, p);
  EXPECT_ULEQ(p, memory_file_get_size(mf));
  EXPECT_ULEQ(p, memory_file_get_position(mf));
  s = p;
  p = 1;
  memory_file_set_position(mf, p);
  EXPECT_ULEQ(s, memory_file_get_size(mf));
  EXPECT_ULEQ(p, memory_file_get_position(mf));
  p = 6;
  memory_file_set_position(mf, p);
  EXPECT_ULEQ(s, memory_file_get_size(mf));
  EXPECT_ULEQ(p, memory_file_get_position(mf));

  /* Should extend first chunk.
   */
  p = 0x10;
  memory_file_set_position(mf, p);
  EXPECT_ULEQ(p, memory_file_get_size(mf));
  EXPECT_ULEQ(p, memory_file_get_position(mf));
  s = p;
  p = 0xf;
  memory_file_set_position(mf, p);
  EXPECT_ULEQ(s, memory_file_get_size(mf));
  EXPECT_ULEQ(p, memory_file_get_position(mf));
  p = 0x1f;
  memory_file_set_position(mf, p);
  EXPECT_ULEQ(p, memory_file_get_size(mf));
  EXPECT_ULEQ(p, memory_file_get_position(mf));
  p = 0x25;
  memory_file_set_position(mf, p);
  EXPECT_ULEQ(p, memory_file_get_size(mf));
  EXPECT_ULEQ(p, memory_file_get_position(mf));
  p = 0x3e;
  memory_file_set_position(mf, p);
  EXPECT_ULEQ(p, memory_file_get_size(mf));
  EXPECT_ULEQ(p, memory_file_get_position(mf));
  p = 0x41;
  memory_file_set_position(mf, p);
  EXPECT_ULEQ(p, memory_file_get_size(mf));
  EXPECT_ULEQ(p, memory_file_get_position(mf));
  p = 0x123;
  memory_file_set_position(mf, p);
  EXPECT_ULEQ(p, memory_file_get_size(mf));
  EXPECT_ULEQ(p, memory_file_get_position(mf));
  p = 0x247;
  memory_file_set_position(mf, p);
  EXPECT_ULEQ(p, memory_file_get_size(mf));
  EXPECT_ULEQ(p, memory_file_get_position(mf));
  p = 0x603;
  memory_file_set_position(mf, p);
  EXPECT_ULEQ(p, memory_file_get_size(mf));
  EXPECT_ULEQ(p, memory_file_get_position(mf));
  p = 0xfff;
  memory_file_set_position(mf, p);
  EXPECT_ULEQ(p, memory_file_get_size(mf));
  EXPECT_ULEQ(p, memory_file_get_position(mf));
  p = 0x1234;
  memory_file_set_position(mf, p);
  EXPECT_ULEQ(p, memory_file_get_size(mf));
  EXPECT_ULEQ(p, memory_file_get_position(mf));
  p = 0x37ae;
  memory_file_set_position(mf, p);
  EXPECT_ULEQ(p, memory_file_get_size(mf));
  EXPECT_ULEQ(p, memory_file_get_position(mf));
  p = 0x7fa0;
  memory_file_set_position(mf, p);
  EXPECT_ULEQ(p, memory_file_get_size(mf));
  EXPECT_ULEQ(p, memory_file_get_position(mf));
  p = 0xffff;
  memory_file_set_position(mf, p);
  EXPECT_ULEQ(p, memory_file_get_size(mf));
  EXPECT_ULEQ(p, memory_file_get_position(mf));

  /* Should extend to second chunk.
   */
  p = 0x10000;
  memory_file_set_position(mf, p);
  EXPECT_ULEQ(p, memory_file_get_size(mf));
  EXPECT_ULEQ(p, memory_file_get_position(mf));
  s = p;
  p = 0xf;
  memory_file_set_position(mf, p);
  EXPECT_ULEQ(s, memory_file_get_size(mf));
  EXPECT_ULEQ(p, memory_file_get_position(mf));
  p = 0xffff;
  memory_file_set_position(mf, p);
  EXPECT_ULEQ(s, memory_file_get_size(mf));
  EXPECT_ULEQ(p, memory_file_get_position(mf));

  /* Should extend to four chunks.
   */
  p = 0x30104;
  memory_file_set_position(mf, p);
  EXPECT_ULEQ(p, memory_file_get_size(mf));
  EXPECT_ULEQ(p, memory_file_get_position(mf));
  p = 0x90210;
  memory_file_set_position(mf, p);
  EXPECT_ULEQ(p, memory_file_get_size(mf));
  EXPECT_ULEQ(p, memory_file_get_position(mf));
  memory_file_delete(&mf);

  /* Start again to test direct jumps.
   */
  p = 0x10;
  memory_file_new(&mf);
  memory_file_set_position(mf, p);
  EXPECT_ULEQ(p, memory_file_get_size(mf));
  EXPECT_ULEQ(p, memory_file_get_position(mf));
  memory_file_delete(&mf);
  p = 0x11;
  memory_file_new(&mf);
  memory_file_set_position(mf, p);
  EXPECT_ULEQ(p, memory_file_get_size(mf));
  EXPECT_ULEQ(p, memory_file_get_position(mf));
  memory_file_delete(&mf);
  p = 0x1f;
  memory_file_new(&mf);
  memory_file_set_position(mf, p);
  EXPECT_ULEQ(p, memory_file_get_size(mf));
  EXPECT_ULEQ(p, memory_file_get_position(mf));
  memory_file_delete(&mf);
  p = 0x20;
  memory_file_new(&mf);
  memory_file_set_position(mf, p);
  EXPECT_ULEQ(p, memory_file_get_size(mf));
  EXPECT_ULEQ(p, memory_file_get_position(mf));
  memory_file_delete(&mf);
  p = 0x21;
  memory_file_new(&mf);
  memory_file_set_position(mf, p);
  EXPECT_ULEQ(p, memory_file_get_size(mf));
  EXPECT_ULEQ(p, memory_file_get_position(mf));
  memory_file_delete(&mf);
  p = 0x3f;
  memory_file_new(&mf);
  memory_file_set_position(mf, p);
  EXPECT_ULEQ(p, memory_file_get_size(mf));
  EXPECT_ULEQ(p, memory_file_get_position(mf));
  memory_file_delete(&mf);
  p = 0x40;
  memory_file_new(&mf);
  memory_file_set_position(mf, p);
  EXPECT_ULEQ(p, memory_file_get_size(mf));
  EXPECT_ULEQ(p, memory_file_get_position(mf));
  memory_file_delete(&mf);
  p = 0x41;
  memory_file_new(&mf);
  memory_file_set_position(mf, p);
  EXPECT_ULEQ(p, memory_file_get_size(mf));
  EXPECT_ULEQ(p, memory_file_get_position(mf));
  memory_file_delete(&mf);
  p = 0x7f;
  memory_file_new(&mf);
  memory_file_set_position(mf, p);
  EXPECT_ULEQ(p, memory_file_get_size(mf));
  EXPECT_ULEQ(p, memory_file_get_position(mf));
  memory_file_delete(&mf);
  p = 0x7fff;
  memory_file_new(&mf);
  memory_file_set_position(mf, p);
  EXPECT_ULEQ(p, memory_file_get_size(mf));
  EXPECT_ULEQ(p, memory_file_get_position(mf));
  memory_file_delete(&mf);
  p = 0x8000;
  memory_file_new(&mf);
  memory_file_set_position(mf, p);
  EXPECT_ULEQ(p, memory_file_get_size(mf));
  EXPECT_ULEQ(p, memory_file_get_position(mf));
  memory_file_delete(&mf);
  p = 0x8001;
  memory_file_new(&mf);
  memory_file_set_position(mf, p);
  EXPECT_ULEQ(p, memory_file_get_size(mf));
  EXPECT_ULEQ(p, memory_file_get_position(mf));
  memory_file_delete(&mf);
  p = 0xffff;
  memory_file_new(&mf);
  memory_file_set_position(mf, p);
  EXPECT_ULEQ(p, memory_file_get_size(mf));
  EXPECT_ULEQ(p, memory_file_get_position(mf));
  memory_file_delete(&mf);
  p = 0x10000;
  memory_file_new(&mf);
  memory_file_set_position(mf, p);
  EXPECT_ULEQ(p, memory_file_get_size(mf));
  EXPECT_ULEQ(p, memory_file_get_position(mf));
  memory_file_delete(&mf);
  p = 0x10001;
  memory_file_new(&mf);
  memory_file_set_position(mf, p);
  EXPECT_ULEQ(p, memory_file_get_size(mf));
  EXPECT_ULEQ(p, memory_file_get_position(mf));
  memory_file_delete(&mf);
  p = 0x1ffff;
  memory_file_new(&mf);
  memory_file_set_position(mf, p);
  EXPECT_ULEQ(p, memory_file_get_size(mf));
  EXPECT_ULEQ(p, memory_file_get_position(mf));
  memory_file_delete(&mf);
  p = 0x20000;
  memory_file_new(&mf);
  memory_file_set_position(mf, p);
  EXPECT_ULEQ(p, memory_file_get_size(mf));
  EXPECT_ULEQ(p, memory_file_get_position(mf));
  memory_file_delete(&mf);
  p = 0x20001;
  memory_file_new(&mf);
  memory_file_set_position(mf, p);
  EXPECT_ULEQ(p, memory_file_get_size(mf));
  EXPECT_ULEQ(p, memory_file_get_position(mf));
  memory_file_delete(&mf);
  p = 0x2ffff;
  memory_file_new(&mf);
  memory_file_set_position(mf, p);
  EXPECT_ULEQ(p, memory_file_get_size(mf));
  EXPECT_ULEQ(p, memory_file_get_position(mf));
  memory_file_delete(&mf);
  p = 0x30000;
  memory_file_new(&mf);
  memory_file_set_position(mf, p);
  EXPECT_ULEQ(p, memory_file_get_size(mf));
  EXPECT_ULEQ(p, memory_file_get_position(mf));
  memory_file_delete(&mf);
  p = 0x30001;
  memory_file_new(&mf);
  memory_file_set_position(mf, p);
  EXPECT_ULEQ(p, memory_file_get_size(mf));
  EXPECT_ULEQ(p, memory_file_get_position(mf));
  memory_file_delete(&mf);
}

TEST(MemoryFile, Comprehensive) {
  byte max_n_width;
  byte min_n_width;
  byte nwidth;
  byte *data = null;
  int i;
  ulong n;
  ulong u;
  MemoryFile *mf = null;
  Random *random = null;

  random_new(&random);
  for (i = 0; i < 100; i++) {
    memory_file_new(&mf);
    max_n_width = 19;
    min_n_width = 2;
    nwidth = random_modulo(random, max_n_width - min_n_width + 1)
      + min_n_width;
    n = random_modulo(random, 1UL << nwidth);
    new(&data, n);

    /* Write out a file full of data.
    */
    for (u = 0; u < n; u++) {
      memory_file_write_byte(mf, data[u] = random_modulo(random, 256));
    }

    /* Read it all and check it.
    */
    memory_file_rewind(mf);
    for (u = 0; u < n; u++) {
      EXPECT_ULEQ(data[u], memory_file_read_byte(mf));
    }

    /* Only do the following if there are entries.
    */
    if (n > 0) {
      /* Randomly rewrite entries, and check those either side where they
       * exist.
      */
      int j;
      for (j = 0; j < 1000; j++) {
        ulong target = random_modulo(random, n);
        data[target] = random_modulo(random, 256);
        memory_file_set_position(mf, target);
        memory_file_write_byte(mf, data[target]);
        if (target > 0) {
          memory_file_set_position(mf, target - 1);
          EXPECT_ULEQ(data[target - 1], memory_file_read_byte(mf));
        }
        memory_file_set_position(mf, target);
        EXPECT_ULEQ(data[target], memory_file_read_byte(mf));
        if (target < n - 1) {
          memory_file_set_position(mf, target + 1);
          EXPECT_ULEQ(data[target + 1], memory_file_read_byte(mf));
        }
      }
    }
    deletev(&data);
    memory_file_delete(&mf);
  }
  random_delete(&random);
}

FINISH
