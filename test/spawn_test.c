#include "spawn_test.h"

static char *check_(char *filename, char *s) {
  char *f = null;
  char *s2 = null;

  if (!ansifile_contents_equal_string_from_filename(filename, s)) {
    external_file_read_all_into_string(filename, &s2);
    return string_new_f(&f, "Expected file contents '%s' but got '%s'", s, s2);
  }
  return null;
}

START

TEST(Spawn, Simple) {
  char *command = null;
  char *echo;
  char *expected = null;
  char *out_filename = null;
  char *err_filename = null;
  char *completed_filename = null;

  /* Simple test.
   */
  echo = "Hi there.\nGood on you!";
  string_new_f(&command, "echo '%s'", echo);
  spawn(command, true, &out_filename, &err_filename, &completed_filename);
  string_delete(&command);
  string_new_concat(&expected, echo, "\n");
  EXPECT(check_(out_filename, expected));
  string_delete(&expected);
  EXPECT(check_(err_filename, ""));
  EXPECT_TRUE(external_file_exists(completed_filename));
  string_delete(&completed_filename);
  string_delete(&out_filename);
  string_delete(&err_filename);

  /* Erroneous command.
   */
  EXPECT_ULNE(
      0,
      try_spawn(
        "ls --bogus-option",
        true,
        &out_filename,
        &err_filename,
        &completed_filename));
  EXPECT(check_(out_filename, ""));
  EXPECT_FALSE(external_file_exists(completed_filename));
  string_delete(&completed_filename);
  string_delete(&out_filename);
  string_delete(&err_filename);
}

FINISH
