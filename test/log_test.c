#include "log_test.h"

START

TEST(Log, Ansifile) {
  char *filename = null;
  char *s = null;
  ExternalFile *ef = null;

  string_new_temp_filename(&filename);
  external_file_new(&ef, filename, "w");
  log_timestamps(false);
  flog(ef->ansifile, "Testing %d %d %d...", 1, 2, 3);
  external_file_delete(&ef);
  external_file_read_all_into_string(filename, &s);
  EXPECT_STREQ("Testing 1 2 3...\n", s);
  string_delete(&s);
  external_file_delete(&ef);
  external_file_remove(filename);
  string_delete(&filename);
}

FINISH
