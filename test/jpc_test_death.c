#include "jpc_test_death.h"

START_DEATH

TEST(Memory, Instance) {
  double *pd1 = null;
  double *pd2;

  pd2 = newo(&pd1);
  deleteo(&pd1);

  /* pd2 will still hold old address, so is not empty.
   */
  EXPECT_DEATH(newo(&pd2), "Pointer not empty in new() or newo()");
}

TEST(Memory, Array) {
  int *a = null;

  new(&a, 10);
  EXPECT_DEATH(new(&a, 3), "Pointer not empty in new() or newo()");
  deletev(&a);
}

TEST(Memory, Donate) {
  double *src = null;
  double *dst = null;
  double *local;

  newo(&src);
  local = src;
  donate(&src, &dst);
  EXPECT_DEATH(
      donate(&dst, &local),
      "Destination pointer not empty in donate()");
  deleteo(&dst);
}

TEST(Print, Indented) {
  FILE *file;

  file = tmpfile();
  EXPECT_DEATH(
      fprintfn(file, 255, "out of control"),
      "Aborting print: indent level got to 255, which is crazy, and probably "
      "broken");
  fclose(file);
}

FINISH_DEATH
