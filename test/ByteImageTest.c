#include "ByteImageTest.h"

START

TEST(ByteImage, Constructor) {
  ByteImage *bi = null;

  byte_image_new(&bi, 3, 4);
  EXPECT_ULEQ(3, bi->width);
  EXPECT_ULEQ(4, bi->height);
  EXPECT_ULEQ(12, bi->numPixels);
  byte_image_delete(&bi);
}

TEST(ByteImage, FromToDouble) {
  ByteImagePixel pixel;
  DoubleImage *di = null;
  ByteImage *bi = null;

  double_image_new_rgb(&di, 2, 3);
  byte_image_new_from_double(&bi, di);
  double_image_delete(&di);
  EXPECT_ULEQ(2, bi->width);
  EXPECT_ULEQ(3, bi->height);
  EXPECT_ULEQ(6, bi->numPixels);
  EXPECT_ULEQ(0, byte_image_get_r(bi, 0, 2));
  EXPECT_ULEQ(0, byte_image_get_g(bi, 1, 2));
  EXPECT_ULEQ(0, byte_image_get_b(bi, 0, 1));
  EXPECT_ULEQ(255, byte_image_get_a(bi, 1, 1));
  double_image_new_from_byte(&di, bi);
  byte_image_delete(&bi);
  EXPECT_ULEQ(2, di->width);
  EXPECT_ULEQ(3, di->height);
  EXPECT_NULL(di->a);
  EXPECT_DEQ(0, di->r[0][2]);
  EXPECT_DEQ(0, di->g[1][2]);
  EXPECT_DEQ(0, di->b[0][1]);
  double_image_delete(&di);

  double_image_new_rgba(&di, 2, 3);
  byte_image_new_from_double(&bi, di);
  double_image_delete(&di);
  EXPECT_ULEQ(2, bi->width);
  EXPECT_ULEQ(3, bi->height);
  EXPECT_ULEQ(6, bi->numPixels);
  EXPECT_ULEQ(0, byte_image_get_r(bi, 0, 2));
  EXPECT_ULEQ(0, byte_image_get_g(bi, 1, 2));
  EXPECT_ULEQ(0, byte_image_get_b(bi, 0, 1));
  EXPECT_ULEQ(255, byte_image_get_a(bi, 1, 1));
  double_image_new_from_byte(&di, bi);
  byte_image_delete(&bi);
  EXPECT_ULEQ(2, di->width);
  EXPECT_ULEQ(3, di->height);
  EXPECT_NULL(di->a);
  EXPECT_DEQ(0, di->r[0][2]);
  EXPECT_DEQ(0, di->g[1][2]);
  EXPECT_DEQ(0, di->b[0][1]);
  double_image_delete(&di);

  /* Test premultiplied alpha.
   */
  double_image_new_rgba(&di, 1, 2);
  di->r[0][0] = 0.5;
  di->g[0][0] = 0.25;
  di->b[0][0] = 0.75;
  di->a[0][0] = 0.8;
  di->r[0][1] = 0.5;
  di->g[0][1] = 0.25;
  di->b[0][1] = 0.75;
  di->a[0][1] = 0;
  byte_image_new_from_double(&bi, di);
  double_image_delete(&di);
  pixel = byte_image_get(bi, 0, 0);
  EXPECT_ULEQ(159, pixel.r);
  EXPECT_ULEQ(80, pixel.g);
  EXPECT_ULEQ(239, pixel.b);
  EXPECT_ULEQ(204, pixel.a);
  pixel = byte_image_get(bi, 0, 1);
  EXPECT_ULEQ(0, pixel.r);
  EXPECT_ULEQ(0, pixel.g);
  EXPECT_ULEQ(0, pixel.b);
  EXPECT_ULEQ(0, pixel.a);
  double_image_new_from_byte(&di, bi);
  byte_image_delete(&bi);
  EXPECT_NEAR(159.0 * 204 / 65025, di->r[0][0], 1e-15);
  EXPECT_NEAR(80.0 * 204 / 65025, di->g[0][0], 1e-15);
  EXPECT_NEAR(239.0 * 204 / 65025, di->b[0][0], 1e-15);
  EXPECT_NEAR(204.0 / 255, di->a[0][0], 1e-15);
  EXPECT_DEQ(0, di->r[0][1]);
  EXPECT_DEQ(0, di->g[0][1]);
  EXPECT_DEQ(0, di->b[0][1]);
  EXPECT_DEQ(0, di->a[0][1]);
  double_image_delete(&di);
}

TEST(ByteImage, AccessorsAndDefaults) {
  ulong i;
  ulong j;
  ByteImagePixel pixel;
  ByteImage *bi = null;

  byte_image_new(&bi, 2, 3);
  byte_image_set_r(bi, 0, 0, 27);
  byte_image_set_g(bi, 1, 2, 98);
  byte_image_set_b(bi, 0, 1, 7);
  byte_image_set_a(bi, 0, 1, 253);

  for (i = 0; i < 2; i++) {
    for (j = 0; j < 3; j++) {
      EXPECT_ULEQ(i == 0 && j == 0 ? 27 : 0, byte_image_get_r(bi, i, j));
      EXPECT_ULEQ(i == 1 && j == 2 ? 98 : 0, byte_image_get_g(bi, i, j));
      EXPECT_ULEQ(i == 0 && j == 1 ? 7 : 0, byte_image_get_b(bi, i, j));
      EXPECT_ULEQ(i == 0 && j == 1 ? 253 : 255, byte_image_get_a(bi, i, j));
      pixel = byte_image_get(bi, i, j);
      EXPECT_ULEQ(i == 0 && j == 0 ? 27 : 0, pixel.r);
      EXPECT_ULEQ(i == 1 && j == 2 ? 98 : 0, pixel.g);
      EXPECT_ULEQ(i == 0 && j == 1 ? 7 : 0, pixel.b);
      EXPECT_ULEQ(i == 0 && j == 1 ? 253 : 255, pixel.a);
    }
  }

  byte_image_delete(&bi);

  byte_image_new(&bi, 2, 3);
  pixel.g = pixel.b = 0;
  pixel.a = 255;
  pixel.r = 27;
  byte_image_set(bi, 0, 0, pixel);
  pixel.r = 0;
  pixel.g = 98;
  byte_image_set(bi, 1, 2, pixel);
  pixel.g = 0;
  pixel.b = 7;
  pixel.a = 253;
  byte_image_set(bi, 0, 1, pixel);

  for (i = 0; i < 2; i++) {
    for (j = 0; j < 3; j++) {
      EXPECT_ULEQ(i == 0 && j == 0 ? 27 : 0, byte_image_get_r(bi, i, j));
      EXPECT_ULEQ(i == 1 && j == 2 ? 98 : 0, byte_image_get_g(bi, i, j));
      EXPECT_ULEQ(i == 0 && j == 1 ? 7 : 0, byte_image_get_b(bi, i, j));
      EXPECT_ULEQ(i == 0 && j == 1 ? 253 : 255, byte_image_get_a(bi, i, j));
      pixel = byte_image_get(bi, i, j);
      EXPECT_ULEQ(i == 0 && j == 0 ? 27 : 0, pixel.r);
      EXPECT_ULEQ(i == 1 && j == 2 ? 98 : 0, pixel.g);
      EXPECT_ULEQ(i == 0 && j == 1 ? 7 : 0, pixel.b);
      EXPECT_ULEQ(i == 0 && j == 1 ? 253 : 255, pixel.a);
    }
  }

  byte_image_delete(&bi);
}

TEST(ByteImage, MakeOpaque) {
  ulong i;
  ulong j;
  ByteImage *bi = null;

  byte_image_new(&bi, 2, 3);

  for (i = 0; i < 2; i++) {
    for (j = 0; j < 3; j++) {
      byte_image_set_a(bi, i, j, i + j);
    }
  }

  byte_image_make_opaque(bi);

  for (i = 0; i < 2; i++) {
    for (j = 0; j < 3; j++) {
      EXPECT_ULEQ(255, byte_image_get_a(bi, i, j));
    }
  }

  byte_image_delete(&bi);
}

TEST(ByteImage, Clear) {
  ulong i;
  ulong j;
  ByteImagePixel pixel;
  ByteImage *bi = null;

  byte_image_new(&bi, 2, 3);

  for (i = 0; i < 2; i++) {
    for (j = 0; j < 3; j++) {
      byte_image_set_r(bi, i, j, i + j);
      byte_image_set_g(bi, i, j, 128 + i - j);
      byte_image_set_b(bi, i, j, i * j);
      byte_image_set_a(bi, i, j, 128 - i - j);
    }
  }

  pixel.r = 12;
  pixel.g = 34;
  pixel.b = 56;
  pixel.a = 78;

  byte_image_clear(bi, pixel);

  for (i = 0; i < 2; i++) {
    for (j = 0; j < 3; j++) {
      EXPECT_ULEQ(12, byte_image_get_r(bi, i, j));
      EXPECT_ULEQ(34, byte_image_get_g(bi, i, j));
      EXPECT_ULEQ(56, byte_image_get_b(bi, i, j));
      EXPECT_ULEQ(78, byte_image_get_a(bi, i, j));
    }
  }

  byte_image_delete(&bi);
}

TEST(ByteImage, ContainsTransparency) {
  ByteImage *bi = null;

  byte_image_new(&bi, 2, 3);
  EXPECT_FALSE(byte_image_contains_transparency(bi));
  byte_image_set_a(bi, 1, 2, 254);
  EXPECT_TRUE(byte_image_contains_transparency(bi));
  byte_image_delete(&bi);
}

TEST(ByteImage, Pixels) {
  ByteImagePixel pixel;
  ByteImagePixel pixel2;

  pixel = byte_image_pixel_from_rgba_hex(0x12345678);
  EXPECT_ULEQ(0x12, pixel.r);
  EXPECT_ULEQ(0x34, pixel.g);
  EXPECT_ULEQ(0x56, pixel.b);
  EXPECT_ULEQ(0x78, pixel.a);

  pixel = byte_image_pixel_opaque_from_rgb_hex(0x123456);
  EXPECT_ULEQ(0x12, pixel.r);
  EXPECT_ULEQ(0x34, pixel.g);
  EXPECT_ULEQ(0x56, pixel.b);
  EXPECT_ULEQ(0xff, pixel.a);

  pixel = byte_image_pixel_opaque_black();
  EXPECT_ULEQ(0, pixel.r);
  EXPECT_ULEQ(0, pixel.g);
  EXPECT_ULEQ(0, pixel.b);
  EXPECT_ULEQ(255, pixel.a);

  pixel.r = 12;
  pixel.g = 34;
  pixel.b = 56;
  pixel.a = 78;
  pixel2 = pixel;

  EXPECT_TRUE(byte_image_pixel_equals(pixel2, pixel));

  pixel = byte_image_pixel_from_hsl(0.3, 0.4, 0.5);
  EXPECT_ULEQ(97, pixel.r);
  EXPECT_ULEQ(179, pixel.g);
  EXPECT_ULEQ(77, pixel.b);
  EXPECT_ULEQ(255, pixel.a);

  EXPECT_ULEQ(0, byte_image_bound(-1e30));
  EXPECT_ULEQ(0, byte_image_bound(-123));
  EXPECT_ULEQ(0, byte_image_bound(-1));
  EXPECT_ULEQ(0, byte_image_bound(-1e-6));
  EXPECT_ULEQ(0, byte_image_bound(0));
  EXPECT_ULEQ(0, byte_image_bound(0.5 - 1e-6));
  EXPECT_ULEQ(1, byte_image_bound(0.5 + 1e-6));
  EXPECT_ULEQ(254, byte_image_bound(254.5 - 1e-6));
  EXPECT_ULEQ(255, byte_image_bound(254.5 + 1e-6));
  EXPECT_ULEQ(255, byte_image_bound(255));
  EXPECT_ULEQ(255, byte_image_bound(256));
  EXPECT_ULEQ(255, byte_image_bound(260));
  EXPECT_ULEQ(255, byte_image_bound(1000));
  EXPECT_ULEQ(255, byte_image_bound(1e30));
}

TEST(ByteImage, YccaPixels) {
  ushort u;
  ByteImagePixel pixel;
  ByteImageYccaPixel ycca;

  /* Test all opaque grays.
   */
  pixel.a = 255;

  for (u = 0; u < 0x100; u++) {
    pixel.r = pixel.g = pixel.b = (byte)u;
    ycca = byte_image_ycca_pixel_from_rgba(pixel);
    EXPECT_ULEQ(u, ycca.y);
    EXPECT_ULEQ(128, ycca.cb);
    EXPECT_ULEQ(128, ycca.cr);
    EXPECT_ULEQ(255, ycca.a);
    pixel = byte_image_pixel_from_ycca(ycca);
    EXPECT_ULEQ(u, pixel.r);
    EXPECT_ULEQ(u, pixel.g);
    EXPECT_ULEQ(u, pixel.b);
    EXPECT_ULEQ(255, pixel.a);
  }

  /* Test all alpha values for mid-gray.
   */
  pixel.r = pixel.g = pixel.b = 128;

  for (u = 0; u < 0x100; u++) {
    pixel.a = (byte)u;
    ycca = byte_image_ycca_pixel_from_rgba(pixel);
    EXPECT_ULEQ(128, ycca.y);
    EXPECT_ULEQ(128, ycca.cb);
    EXPECT_ULEQ(128, ycca.cr);
    EXPECT_ULEQ(u, ycca.a);
    pixel = byte_image_pixel_from_ycca(ycca);
    EXPECT_ULEQ(128, pixel.r);
    EXPECT_ULEQ(128, pixel.g);
    EXPECT_ULEQ(128, pixel.b);
    EXPECT_ULEQ(u, pixel.a);
  }

  /* Test some particular values.
   */
  pixel.a = 255;
  pixel.r = 255;
  pixel.g = pixel.b = 32;
  ycca = byte_image_ycca_pixel_from_rgba(pixel);
  EXPECT_ULEQ(99, ycca.y);
  EXPECT_ULEQ(90, ycca.cb);
  EXPECT_ULEQ(240, ycca.cr);
  pixel = byte_image_pixel_from_ycca(ycca);
  EXPECT_ULEQ(255, pixel.r);
  EXPECT_ULEQ(32, pixel.g);
  EXPECT_ULEQ(32, pixel.b);

  pixel.g = 255;
  pixel.r = pixel.b = 0;
  ycca = byte_image_ycca_pixel_from_rgba(pixel);
  EXPECT_ULEQ(150, ycca.y);
  EXPECT_ULEQ(44, ycca.cb);
  EXPECT_ULEQ(21, ycca.cr);
  pixel = byte_image_pixel_from_ycca(ycca);
  EXPECT_ULEQ(0, pixel.r);
  EXPECT_ULEQ(255, pixel.g);
  EXPECT_ULEQ(1, pixel.b);

  pixel.b = 255;
  pixel.r = pixel.g = 16;
  ycca = byte_image_ycca_pixel_from_rgba(pixel);
  EXPECT_ULEQ(43, ycca.y);
  EXPECT_ULEQ(248, ycca.cb);
  EXPECT_ULEQ(109, ycca.cr);
  pixel = byte_image_pixel_from_ycca(ycca);
  EXPECT_ULEQ(16, pixel.r);
  EXPECT_ULEQ(15, pixel.g);
  EXPECT_ULEQ(255, pixel.b);
}

FINISH
