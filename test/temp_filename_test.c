#include "temp_filename_test.h"

#define N_ 1000

START

TEST(TempFilename, Rapid) {
  char **filenames = null;
  ulong u;

  new(&filenames, N_);

  /* Confirm that rapid calls do not create any duplicates.
   */
  for (u = 0; u < N_; u++) {
    string_new_temp_filename(&filenames[u]);
  }

  qsortx(filenames, N_, string_compare, null);

  for (u = 0; u < N_ - 1; u++) {
    EXPECT_STRNEQ(filenames[u], filenames[u + 1]);
  }

  delete(&filenames, N_, string_delete);
}

FINISH
