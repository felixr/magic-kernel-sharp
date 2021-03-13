#include "TextFileTestDeath.h"

START_DEATH

TEST(TextFile, Death) {
  char *e = null;
  char *filename = null;
  char **fields;
  TextFile *tf = null;
  TextFile *tf2 = null;

  string_new_temp_filename(&filename);
  string_new_f(
      &e,
      "Could not open file '%s' in mode 'r'",
      filename);
  EXPECT_DEATH(text_file_new(&tf, filename, "r"), e);
  string_delete(&e);

  text_file_new(&tf, filename, "w");
  EXPECT_DEATH(
      text_file_new_memory_reference(&tf2, tf),
      "Can only create memory reference of a MemoryFile");
  EXPECT_DEATH(
      text_file_set_type(tf, "dog"),
      "TextFile filetype 'dog' not recognized");
  text_file_delete(&tf2);
  text_file_delete(&tf);
  text_file_new(&tf, filename, "r");
  EXPECT_DEATH(
      text_file_read(tf, &fields),
      "Couldn't read record");
  EXPECT_DEATH(
      text_file_read_single(tf),
      "Could not read from file");
  text_file_delete(&tf);
  external_file_remove(filename);

  text_file_new(&tf, filename, "w");
  text_file_write(tf, "dog");
  text_file_write(tf, "cat");
  text_file_end_record(tf);
  text_file_write(tf, "dog");
  text_file_end_record(tf);
  text_file_delete(&tf);
  text_file_new(&tf, filename, "r");
  EXPECT_DEATH(
      text_file_read_empty_record(tf),
      "Record was not empty: it had 2 fields");
  text_file_rewind(tf);
  EXPECT_DEATH(
      text_file_read_single(tf),
      "Expected only a single field but read 2 fields");
  EXPECT_DEATH(
      text_file_read_label(tf, "cat"),
      "Expected to read label 'cat', but read label 'dog'");
  text_file_rewind(tf);
  EXPECT_DEATH(
      text_file_read_labeled(tf, "cat"),
      "Expected to read label 'cat', but read label 'dog'");
  text_file_rewind(tf);
  text_file_read(tf, &fields);
  EXPECT_DEATH(
      text_file_merge_fields(tf, 0, 2, ','),
      "Last field index 2 invalid: must be less than 2");
  EXPECT_DEATH(
      text_file_merge_fields(tf, 1, 0, ','),
      "First field index 1 not less than last field index 0");
  EXPECT_DEATH(
      text_file_merge_fields(tf, 0, 0, ','),
      "First field index 0 not less than last field index 0");
  text_file_delete(&tf);
  external_file_remove(filename);
  string_delete(&filename);
}

FINISH_DEATH
