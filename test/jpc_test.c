#include "jpc_test.h"

#define N_ 100000

typedef struct {
  bool b: 1;
} Boolbit_;

START

TEST(Type, Size) {
  EXPECT_ULEQ(1, sizeof(bool));
  EXPECT_ULEQ(1, sizeof(sbyte));
  EXPECT_ULEQ(1, sizeof(byte));
  EXPECT_ULEQ(2, sizeof(short));
  EXPECT_ULEQ(2, sizeof(ushort));
  EXPECT_ULEQ(4, sizeof(int));
  EXPECT_ULEQ(4, sizeof(uint));
  EXPECT_ULEQ(8, sizeof(long));
  EXPECT_ULEQ(8, sizeof(ulong));
  EXPECT_ULEQ(4, sizeof(float));
  EXPECT_ULEQ(8, sizeof(double));
  EXPECT_ULEQ(8, sizeof(size_t));
}

TEST(Boolean, Values) {
  EXPECT_ULEQ(0, false);
  EXPECT_ULEQ(!0, true);
}

TEST(Boolbit, Basic) {
  bool d[32];
  byte a;
  byte v;
  ulong u;
  struct {
    boolbit a: 1;
    boolbit b: 1;
    boolbit c: 1;
    boolbit d: 1;
    boolbit e: 1;
    boolbit f: 1;
    boolbit g: 1;
    boolbit h: 1;
    boolbit i: 1;
    boolbit j: 1;
    boolbit k: 1;
    boolbit l: 1;
    boolbit m: 1;
    boolbit n: 1;
    boolbit o: 1;
    boolbit p: 1;
    boolbit q: 1;
    boolbit r: 1;
    boolbit s: 1;
    boolbit t: 1;
    boolbit u: 1;
    boolbit v: 1;
    boolbit w: 1;
    boolbit x: 1;
    boolbit y: 1;
    boolbit z: 1;
    boolbit A: 1;
    boolbit B: 1;
    boolbit C: 1;
    boolbit D: 1;
    boolbit E: 1;
    boolbit F: 1;
  } s;
  Random *random = null;

  EXPECT_ULEQ(4, sizeof(s));
  random_new(&random);
  for (u = 0; u < N_; u++) {
    for (v = 0; v < 32; v++) {
      d[v] = random_bool(random);
    }
    a = 0;
    s.a = d[a++];
    s.b = d[a++];
    s.c = d[a++];
    s.d = d[a++];
    s.e = d[a++];
    s.f = d[a++];
    s.g = d[a++];
    s.h = d[a++];
    s.i = d[a++];
    s.j = d[a++];
    s.k = d[a++];
    s.l = d[a++];
    s.m = d[a++];
    s.n = d[a++];
    s.o = d[a++];
    s.p = d[a++];
    s.q = d[a++];
    s.r = d[a++];
    s.s = d[a++];
    s.t = d[a++];
    s.u = d[a++];
    s.v = d[a++];
    s.w = d[a++];
    s.x = d[a++];
    s.y = d[a++];
    s.z = d[a++];
    s.A = d[a++];
    s.B = d[a++];
    s.C = d[a++];
    s.D = d[a++];
    s.E = d[a++];
    s.F = d[a++];
    a = 0;
    EXPECT_ULEQ(d[a], s.a);
    a++;
    EXPECT_ULEQ(d[a], s.b);
    a++;
    EXPECT_ULEQ(d[a], s.c);
    a++;
    EXPECT_ULEQ(d[a], s.d);
    a++;
    EXPECT_ULEQ(d[a], s.e);
    a++;
    EXPECT_ULEQ(d[a], s.f);
    a++;
    EXPECT_ULEQ(d[a], s.g);
    a++;
    EXPECT_ULEQ(d[a], s.h);
    a++;
    EXPECT_ULEQ(d[a], s.i);
    a++;
    EXPECT_ULEQ(d[a], s.j);
    a++;
    EXPECT_ULEQ(d[a], s.k);
    a++;
    EXPECT_ULEQ(d[a], s.l);
    a++;
    EXPECT_ULEQ(d[a], s.m);
    a++;
    EXPECT_ULEQ(d[a], s.n);
    a++;
    EXPECT_ULEQ(d[a], s.o);
    a++;
    EXPECT_ULEQ(d[a], s.p);
    a++;
    EXPECT_ULEQ(d[a], s.q);
    a++;
    EXPECT_ULEQ(d[a], s.r);
    a++;
    EXPECT_ULEQ(d[a], s.s);
    a++;
    EXPECT_ULEQ(d[a], s.t);
    a++;
    EXPECT_ULEQ(d[a], s.u);
    a++;
    EXPECT_ULEQ(d[a], s.v);
    a++;
    EXPECT_ULEQ(d[a], s.w);
    a++;
    EXPECT_ULEQ(d[a], s.x);
    a++;
    EXPECT_ULEQ(d[a], s.y);
    a++;
    EXPECT_ULEQ(d[a], s.z);
    a++;
    EXPECT_ULEQ(d[a], s.A);
    a++;
    EXPECT_ULEQ(d[a], s.B);
    a++;
    EXPECT_ULEQ(d[a], s.C);
    a++;
    EXPECT_ULEQ(d[a], s.D);
    a++;
    EXPECT_ULEQ(d[a], s.E);
    a++;
    EXPECT_ULEQ(d[a], s.F);
  }
  random_delete(&random);
}

TEST(Null, Value) {
  EXPECT_PEQ((char *)0, (char *)null);
}

TEST(Bool, Identity) {
  bool b;
  Boolbit_ bb;

  b = true;
  bb.b = true;
  EXPECT_TRUE(b);
  EXPECT_TRUE((1 == 1) == b);
  EXPECT_TRUE(bb.b);
  EXPECT_TRUE((1 == 1) == bb.b);
  EXPECT_TRUE(bb.b == b);
  EXPECT_FALSE(bb.b != b);

  b = true;
  bb.b = false;
  EXPECT_TRUE(b);
  EXPECT_FALSE(bb.b);
  EXPECT_FALSE(bb.b == b);
  EXPECT_TRUE(bb.b != b);

  b = false;
  bb.b = true;
  EXPECT_FALSE(b);
  EXPECT_TRUE(bb.b);
  EXPECT_FALSE(bb.b == b);
  EXPECT_TRUE(bb.b != b);

  b = false;
  bb.b = false;
  EXPECT_FALSE(b);
  EXPECT_TRUE((1 == 0) == b);
  EXPECT_FALSE(bb.b);
  EXPECT_TRUE((1 == 0) == bb.b);
  EXPECT_TRUE(bb.b == b);
  EXPECT_FALSE(bb.b != b);
}

TEST(Bool, String) {
  EXPECT_STREQ("true", bool_string(true));
  EXPECT_STREQ("false", bool_string(false));
}

TEST(String, PluralS) {
  EXPECT_STREQ("s", plural_s(-12345));
  EXPECT_STREQ("s", plural_s(-1));
  EXPECT_STREQ("s", plural_s(0));
  EXPECT_STREQ("", plural_s(1));
  EXPECT_STREQ("s", plural_s(2));
  EXPECT_STREQ("s", plural_s(12345));
}

TEST(Compare, MaxMin) {
  EXPECT_LEQ(6, maximum(-1, 6));
  EXPECT_LEQ(6, maximum(6, -1));
  EXPECT_LEQ(1, maximum(1, -6));
  EXPECT_LEQ(1, maximum(-6, 1));
  EXPECT_LEQ(6, maximum(6, 6));
  EXPECT_LEQ(-1, maximum(-1, -1));
  EXPECT_LEQ(0, maximum(0, 0));
  EXPECT_LEQ(0, maximum(0, -4));
  EXPECT_LEQ(0, maximum(-4, 0));
  EXPECT_LEQ(4, maximum(0, 4));
  EXPECT_LEQ(4, maximum(4, 0));

  EXPECT_LEQ(-1, minimum(-1, 6));
  EXPECT_LEQ(-1, minimum(6, -1));
  EXPECT_LEQ(-6, minimum(1, -6));
  EXPECT_LEQ(-6, minimum(-6, 1));
  EXPECT_LEQ(6, minimum(6, 6));
  EXPECT_LEQ(-1, minimum(-1, -1));
  EXPECT_LEQ(0, minimum(0, 0));
  EXPECT_LEQ(-4, minimum(0, -4));
  EXPECT_LEQ(-4, minimum(-4, 0));
  EXPECT_LEQ(0, minimum(0, 4));
  EXPECT_LEQ(0, minimum(4, 0));
}

TEST(Integer, Modulo) {
  EXPECT_LEQ(0, modulo(-3, 1));
  EXPECT_LEQ(0, modulo(-2, 1));
  EXPECT_LEQ(0, modulo(-1, 1));
  EXPECT_LEQ(0, modulo(0, 1));
  EXPECT_LEQ(0, modulo(1, 1));
  EXPECT_LEQ(0, modulo(2, 1));
  EXPECT_LEQ(0, modulo(3, 1));

  EXPECT_LEQ(0, modulo(-6, 2));
  EXPECT_LEQ(1, modulo(-5, 2));
  EXPECT_LEQ(0, modulo(-4, 2));
  EXPECT_LEQ(1, modulo(-3, 2));
  EXPECT_LEQ(0, modulo(-2, 2));
  EXPECT_LEQ(1, modulo(-1, 2));
  EXPECT_LEQ(0, modulo(0, 2));
  EXPECT_LEQ(1, modulo(1, 2));
  EXPECT_LEQ(0, modulo(2, 2));
  EXPECT_LEQ(1, modulo(3, 2));
  EXPECT_LEQ(0, modulo(4, 2));
  EXPECT_LEQ(1, modulo(5, 2));

  EXPECT_LEQ(0, modulo(-9, 3));
  EXPECT_LEQ(1, modulo(-8, 3));
  EXPECT_LEQ(2, modulo(-7, 3));
  EXPECT_LEQ(0, modulo(-6, 3));
  EXPECT_LEQ(1, modulo(-5, 3));
  EXPECT_LEQ(2, modulo(-4, 3));
  EXPECT_LEQ(0, modulo(-3, 3));
  EXPECT_LEQ(1, modulo(-2, 3));
  EXPECT_LEQ(2, modulo(-1, 3));
  EXPECT_LEQ(0, modulo(0, 3));
  EXPECT_LEQ(1, modulo(1, 3));
  EXPECT_LEQ(2, modulo(2, 3));
  EXPECT_LEQ(0, modulo(3, 3));
  EXPECT_LEQ(1, modulo(4, 3));
  EXPECT_LEQ(2, modulo(5, 3));
  EXPECT_LEQ(0, modulo(6, 3));
  EXPECT_LEQ(1, modulo(7, 3));
  EXPECT_LEQ(2, modulo(8, 3));

  /* Test different types. Type coercion can yield wrong results if the macro
   * is not set up correctly.
   */
  EXPECT_LEQ(9, modulo(-1, 10));
  EXPECT_LEQ(9, modulo((long)-1, 10));
  EXPECT_LEQ(9, modulo(-1, (ulong)10));
  EXPECT_LEQ(9, modulo((long)-1, (ulong)10));
}

TEST(Memory, Instance) {
  double *pd1 = null;
  double *pd2;

  pd2 = newo(&pd1);

  /* Should be allocated, and set in both pointers.
   */
  EXPECT_NONNULL(pd1);
  EXPECT_PEQ(pd2, pd1);

  /* Delete should set the pointer to be empty.
   */
  deleteo(&pd1);
  EXPECT_NULL(pd1);

  /* Second deleteo should not die.
   */
  deleteo(&pd1);
}

TEST(Memory, Array) {
  int i;
  int *a = null;

  new(&a, 3);

  /* Should be zeroed on allocation.
   */
  for (i = 0; i < 3; i++) {
    EXPECT_ULEQ(0, a[i]);
  }
  a[0] = 24;
  a[1] = 9;
  a[2] = 1966;
  renew(&a, 3, 7, null);

  /* Should not affect existing entries.
   */
  EXPECT_ULEQ(24, a[0]);
  EXPECT_ULEQ(9, a[1]);
  EXPECT_ULEQ(1966, a[2]);

  /* Should be zeroed by renew.
   */
  for (i = 3; i < 7; i++) {
    EXPECT_ULEQ(0, a[i]);
  }
  renew(&a, 7, 2, null);

  /* Should not affect surviving entries.
   */
  EXPECT_ULEQ(24, a[0]);
  EXPECT_ULEQ(9, a[1]);
  renew(&a, 2, 3, null);

  /* Should be zeroed by renew.
   */
  EXPECT_ULEQ(0, a[2]);
  deletev(&a);

  /* Second deletev() should be no-op.
   */
  deletev(&a);

  /* n = 0 is ok.
   */
  new(&a, 0);
  renew(&a, 0, 5, null);
  renew(&a, 5, 0, null);
  deletev(&a);
}

TEST(Memory, Donate) {
  double *src = null;
  double *dst = null;
  double *local;

  newo(&src);
  local = src;
  donate(&src, &dst);
  EXPECT_NULL(src);
  EXPECT_PEQ(dst, local);
  deleteo(&dst);
}

TEST(Memory, Matrix) {
  int i;
  int j;
  int **a = null;
  double **b = null;

  newm(&a, 2, 3);

  /* Should be zeroed on allocation.
   */
  for (i = 0; i < 2; i++) {
    for (j = 0; j < 3; j++) {
      EXPECT_ULEQ(0, a[i][j]);
    }
  }
  a[0][0] = 24;
  a[0][1] = 9;
  a[0][2] = 1966;
  a[1][0] = 7;
  a[1][1] = 7;
  a[1][2] = 1968;
  EXPECT_ULEQ(24, a[0][0]);
  EXPECT_ULEQ(9, a[0][1]);
  EXPECT_ULEQ(1966, a[0][2]);
  EXPECT_ULEQ(7, a[1][0]);
  EXPECT_ULEQ(7, a[1][1]);
  EXPECT_ULEQ(1968, a[1][2]);
  deletemv(&a, 2);

  newm(&b, 2, 3);

  /* Should be zeroed on allocation.
   */
  for (i = 0; i < 2; i++) {
    for (j = 0; j < 3; j++) {
      EXPECT_DEQ(0, b[i][j]);
    }
  }
  b[0][0] = 0.24;
  b[0][1] = -0.9;
  b[0][2] = 19.66;
  b[1][0] = 0.07;
  b[1][1] = -0.07;
  b[1][2] = 1.968;
  EXPECT_DEQ(0.24, b[0][0]);
  EXPECT_DEQ(-0.9, b[0][1]);
  EXPECT_DEQ(19.66, b[0][2]);
  EXPECT_DEQ(0.07, b[1][0]);
  EXPECT_DEQ(-0.07, b[1][1]);
  EXPECT_DEQ(1.968, b[1][2]);
  deletemv(&b, 2);
}

TEST(Memory, Tensor3) {
  int i;
  int j;
  int k;
  int ***a = null;
  newt3(&a, 2, 3, 4);

  /* Should be zeroed on allocation.
   */
  for (i = 0; i < 2; i++) {
    for (j = 0; j < 3; j++) {
      for (k = 0; k < 4; k++) {
        EXPECT_ULEQ(0, a[i][j][k]);
      }
    }
  }
  for (i = 0; i < 2; i++) {
    for (j = 0; j < 3; j++) {
      for (k = 0; k < 4; k++) {
        a[i][j][k] = -2 * i + 3 + j + 7 * k;
      }
    }
  }
  for (i = 0; i < 2; i++) {
    for (j = 0; j < 3; j++) {
      for (k = 0; k < 4; k++) {
        EXPECT_ULEQ(a[i][j][k], -2 * i + 3 + j + 7 * k);
      }
    }
  }
  deletet3v(&a, 2, 3);
}

TEST(Memory, Tensor4) {
  int i;
  int j;
  int k;
  int l;
  int ****a = null;

  newt4(&a, 2, 3, 4, 7);

  /* Should be zeroed on allocation.
   */
  for (i = 0; i < 2; i++) {
    for (j = 0; j < 3; j++) {
      for (k = 0; k < 4; k++) {
        for (l = 0; l < 7; l++) {
          EXPECT_ULEQ(0, a[i][j][k][l]);
        }
      }
    }
  }
  for (i = 0; i < 2; i++) {
    for (j = 0; j < 3; j++) {
      for (k = 0; k < 4; k++) {
        for (l = 0; l < 7; l++) {
          a[i][j][k][l] = -2 * i + 3 + j + 7 * k - 3 * l * i;
        }
      }
    }
  }
  for (i = 0; i < 2; i++) {
    for (j = 0; j < 3; j++) {
      for (k = 0; k < 4; k++) {
        for (l = 0; l < 7; l++) {
          EXPECT_ULEQ(a[i][j][k][l], -2 * i + 3 + j + 7 * k - 3 * l * i);
        }
      }
    }
  }
  deletet4v(&a, 2, 3, 4);
}

TEST(Print, Indented) {
  char expected[] = "        yeah 42\n";
  FILE *file;

  file = tmpfile();
  fprintfn(file, 4, "%s %d", "yeah", 42);
  EXPECT_TRUE(ansifile_contents_equal_string(file, expected));
  fclose(file);
}

TEST(Math, Round) {
  EXPECT_DEQ(0, round(0));
  EXPECT_DEQ(0, round(0.49999999));
  EXPECT_DEQ(1, round(0.5));
  EXPECT_DEQ(2, round(1.5));
  EXPECT_DEQ(100, round(99.5));
  EXPECT_DEQ(100, round(100.4999));
  EXPECT_DEQ(0, round(-0.49999999));
  EXPECT_DEQ(-1, round(-0.5));
  EXPECT_DEQ(-1, round(-0.9));
  EXPECT_DEQ(-1, round(-1.49999999));
  EXPECT_DEQ(-2, round(-1.5));
}

TEST(PointerArray, Defrag) {
  ushort **a = null;
  ushort u;
  ushort u2;
  ulong n;

  /* All null.
   */
  n = 10;
  new(&a, n);
  defrag_pointer_array(&a, &n);
  EXPECT_ULEQ(0, n);
  deletev(&a);

  /* One filled.
   */
  u = 24;
  n = 7;
  new(&a, n);
  a[0] = &u;
  defrag_pointer_array(&a, &n);
  EXPECT_ULEQ(1, n);
  EXPECT_PEQ(&u, a[0]);
  deletev(&a);

  n = 7;
  new(&a, n);
  a[1] = &u;
  defrag_pointer_array(&a, &n);
  EXPECT_ULEQ(1, n);
  EXPECT_PEQ(&u, a[0]);
  deletev(&a);

  n = 7;
  new(&a, n);
  a[6] = &u;
  defrag_pointer_array(&a, &n);
  EXPECT_ULEQ(1, n);
  EXPECT_PEQ(&u, a[0]);
  deletev(&a);

  /* Two filled.
   */
  u2 = 42;
  n = 7;
  new(&a, n);
  a[0] = &u;
  a[1] = &u2;
  defrag_pointer_array(&a, &n);
  EXPECT_ULEQ(2, n);
  EXPECT_PEQ(&u, a[0]);
  EXPECT_PEQ(&u2, a[1]);
  deletev(&a);

  n = 7;
  new(&a, n);
  a[0] = &u;
  a[6] = &u2;
  defrag_pointer_array(&a, &n);
  EXPECT_ULEQ(2, n);
  EXPECT_PEQ(&u, a[0]);
  EXPECT_PEQ(&u2, a[1]);
  deletev(&a);

  n = 7;
  new(&a, n);
  a[5] = &u;
  a[6] = &u2;
  defrag_pointer_array(&a, &n);
  EXPECT_ULEQ(2, n);
  EXPECT_PEQ(&u, a[0]);
  EXPECT_PEQ(&u2, a[1]);
  deletev(&a);

  /* All filled.
   */
  n = 5;
  new(&a, n);
  a[0] = &u;
  a[1] = &u2;
  a[2] = &u2;
  a[3] = &u;
  a[4] = &u2;
  defrag_pointer_array(&a, &n);
  EXPECT_ULEQ(5, n);
  EXPECT_PEQ(&u, a[0]);
  EXPECT_PEQ(&u2, a[1]);
  EXPECT_PEQ(&u2, a[2]);
  EXPECT_PEQ(&u, a[3]);
  EXPECT_PEQ(&u2, a[4]);
  deletev(&a);
}

FINISH
