#include "ExternalFileTest.h"

#define N_ (1024 * 1024 + 123)

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

START

TEST(ExternalFile, WriteRead) {
  byte *data = null;
  char *filename = null;
  ulong u;
  ExternalFile *ef = null;

  string_new_temp_filename(&filename);

  /* Make sure we're overwriting by creating an existing file.
   */
  external_file_new(&ef, filename, "w");
  external_file_write_byte(ef, 'j');
  external_file_write_byte(ef, 'p');
  external_file_write_byte(ef, 'c');
  external_file_delete(&ef);

  /* Now open the file write-only. This should overwrite.
   */
  external_file_new(&ef, filename, "w");
  EXPECT_ULEQ(0, external_file_get_size(ef));
  EXPECT_ULEQ(0, external_file_get_position(ef));
  new(&data, N_);
  for (u = 0; u < N_ - 10; u++) {
    EXPECT_ULEQ(u, external_file_get_position(ef));
    if (u % 2573 == 0) {
      /* Expensive for an external file.
      */
      EXPECT_ULEQ(u, external_file_get_size(ef));
    }
    external_file_write_byte(ef, data[u] = hash_(u));
  }
  EXPECT_TRUE(external_file_is_eof(ef));

  /* Reposition and write some different bytes.
   */
  u = 12345;
  external_file_set_position(ef, u);
  external_file_write_byte(ef, data[u++] = 'j');
  external_file_write_byte(ef, data[u++] = 'p');
  external_file_write_byte(ef, data[u++] = 'c');

  /* Now extend the file to the full N bytes. The intermediate bytes should be
   * zeroed. Note that a byte must be written for the file to be extended;
   * simply repositioning past the end of the file doesn't do it.
   */
  external_file_set_position(ef, N_ - 1);
  external_file_write_byte(ef, '!');
  for (u = N_ - 10; u < N_ - 1; u++) {
    data[u] = 0;
  }
  data[u] = '!';
  external_file_delete(&ef);

  /* Reopen file for reading only.
  */
  external_file_new(&ef, filename, "r");
  EXPECT_ULEQ(N_, external_file_get_size(ef));
  EXPECT_ULEQ(0, external_file_get_position(ef));
  EXPECT_FALSE(external_file_is_eof(ef));
  for (u = 0; u < N_; u++) {
    EXPECT_ULEQ(u, external_file_get_position(ef));
    if (u % 1237 == 0) {
      /* Expensive for an external file.
      */
      EXPECT_ULEQ(N_, external_file_get_size(ef));
    }
    EXPECT_ULEQ(data[u], external_file_read_byte(ef));
  }

  /* Move to arbitrary position and check all is well.
   */
  u = 12345;
  external_file_set_position(ef, u);
  EXPECT_ULEQ(u, external_file_get_position(ef));
  for (; u < 12349; u++) {
    EXPECT_ULEQ(u, external_file_get_position(ef));
    if (u % 1233 == 0) {
      /* Expensive for an external file.
       */
      EXPECT_ULEQ(N_, external_file_get_size(ef));
    }
    EXPECT_ULEQ(data[u], external_file_read_byte(ef));
  }

  deletev(&data);
  string_delete(&filename);
  external_file_delete(&ef);
}

TEST(ExternalFile, Append) {
  byte *data = null;
  char *filename = null;
  ulong first_length;
  ulong s;
  ulong u;
  ExternalFile *ef = null;

  string_new_temp_filename(&filename);
  new(&data, N_);

  /* Create the first part of the file in write-only mode.
   */
  first_length = N_ / 3;
  external_file_new(&ef, filename, "w");
  for (u = 0; u < first_length; u++) {
    external_file_write_byte(ef, data[u] = hash_(u));
  }
  external_file_delete(&ef);

  /* Now append to it.
   */
  external_file_new(&ef, filename, "a");
  for (; u < N_ - 3; u++) {
    external_file_write_byte(ef, data[u] = hash_(u));
  }

  /* Repositioning in append mode only affects the read position; writes are
   * always appends. And since we are not in update mode, we can't actually
   * read anything anyway.
   */
  s = u;
  u = 12345;
  external_file_set_position(ef, u);

  /* This is a read position.
   */
  EXPECT_ULEQ(u, external_file_get_position(ef));

  /* This is appended.
   */
  external_file_write_byte(ef, data[s++] = 'j');
  external_file_write_byte(ef, data[s++] = 'p');
  external_file_write_byte(ef, data[s++] = 'c');
  external_file_delete(&ef);

  /* Reopen file in read mode, and check the contents.
   */
  external_file_new(&ef, filename, "r");
  EXPECT_TRUE(external_file_contents_equal_bytes(ef, data, N_));

  deletev(&data);
  string_delete(&filename);
  external_file_delete(&ef);
}

TEST(ExternalFile, ReadUpdate) {
  byte *data = null;
  char *filename = null;
  ulong u;
  ExternalFile *ef = null;

  string_new_temp_filename(&filename);
  new(&data, N_);

  /* Start the process by creating an existing file.
   */
  external_file_new(&ef, filename, "w");
  u = 0;
  external_file_write_byte(ef, data[u++] = 'j');
  external_file_write_byte(ef, data[u++] = 'p');
  external_file_write_byte(ef, data[u++] = 'c');
  external_file_write_byte(ef, data[u++] = '!');
  external_file_write_byte(ef, data[u++] = '?');
  external_file_delete(&ef);

  /* Read and update within the file.
   */
  external_file_new(&ef, filename, "r+");

  /* Read the second byte.
   */
  external_file_set_position(ef, 1);
  EXPECT_ULEQ('p', external_file_read_byte(ef));

  /* Overwrite the fourth byte.
   */
  external_file_set_position(ef, 3);
  external_file_write_byte(ef, data[3] = 'G');

  /* Extend the file to 20 bytes. A write is needed.
   */
  external_file_set_position(ef, 19);
  external_file_write_byte(ef, data[19] = 'W');

  /* Reread the second and fourth bytes.
   */
  external_file_set_position(ef, 1);
  EXPECT_ULEQ('p', external_file_read_byte(ef));
  external_file_set_position(ef, 3);
  EXPECT_ULEQ('G', external_file_read_byte(ef));

  /* Write out the rest of the file.
   */
  external_file_set_position(ef, 20);
  for (u = 20; u < N_; u++) {
    external_file_write_byte(ef, data[u] = hash_(u));
  }

  /* Check the contents without needing to reopen.
   */
  EXPECT_TRUE(external_file_contents_equal_bytes(ef, data, N_));

  deletev(&data);
  string_delete(&filename);
  external_file_delete(&ef);
}

TEST(ExternalFile, WriteUpdate) {
  byte *data = null;
  char *filename = null;
  ulong s;
  ulong u;
  ExternalFile *ef = null;

  string_new_temp_filename(&filename);
  new(&data, N_);

  /* Make sure we're overwriting by creating an existing file.
   */
  external_file_new(&ef, filename, "w");
  external_file_write_byte(ef, 'j');
  external_file_write_byte(ef, 'p');
  external_file_write_byte(ef, 'c');
  external_file_delete(&ef);

  /* Write and update within the file.
   */
  external_file_new(&ef, filename, "w+");

  /* Write out some.
   */
  for (u = 0; u < N_ / 3; u++) {
    external_file_write_byte(ef, data[u] = hash_(u));
  }

  /* Go back and read some.
   */
  s = u;
  u = N_ / 6 + 123;
  external_file_set_position(ef, u);
  EXPECT_ULEQ(s, external_file_get_size(ef));
  EXPECT_ULEQ(data[u], external_file_read_byte(ef));
  u++;
  EXPECT_ULEQ(u, external_file_get_position(ef));
  EXPECT_ULEQ(s, external_file_get_size(ef));
  EXPECT_ULEQ(data[u], external_file_read_byte(ef));
  u++;
  EXPECT_ULEQ(u, external_file_get_position(ef));
  EXPECT_ULEQ(s, external_file_get_size(ef));

  /* Go back and change some.
   */
  u = N_ / 7;
  external_file_set_position(ef, u);
  external_file_write_byte(ef, data[u] = 'j');
  u++;
  external_file_write_byte(ef, data[u] = 'p');
  u++;
  external_file_write_byte(ef, data[u] = 'c');

  /* Write the rest of the data, except the last 10 bytes.
   */
  external_file_set_position(ef, N_ / 3);
  for (u = N_ / 3; u < N_ - 10; u++) {
    external_file_write_byte(ef, data[u] = hash_(u));
  }

  /* Extend the last 10 bytes.
   */
  external_file_set_position(ef, N_ - 1);
  external_file_write_byte(ef, '!');
  for (u = N_ - 10; u < N_ - 1; u++) {
    data[u] = 0;
  }
  data[u] = '!';

  /* Check the contents without needing to reopen.
   */
  EXPECT_TRUE(external_file_contents_equal_bytes(ef, data, N_));

  deletev(&data);
  string_delete(&filename);
  external_file_delete(&ef);
}

TEST(ExternalFile, AppendUpdate) {
  byte *data = null;
  char *filename = null;
  ulong first_length;
  ulong u;
  ExternalFile *ef = null;

  string_new_temp_filename(&filename);
  new(&data, N_);

  /* Create the first part of the file in write-only mode.
   */
  first_length = N_ / 3;
  external_file_new(&ef, filename, "w");
  for (u = 0; u < first_length; u++) {
    external_file_write_byte(ef, data[u] = hash_(u));
  }
  external_file_delete(&ef);

  /* Now append to it in update mode. This is a strange ANSI mode in which all
   * writes are appends at the end of the file, but reading can occur anywhere
   * within the file.
   */
  external_file_new(&ef, filename, "a+");
  for (; u < N_ - 3; u++) {
    external_file_write_byte(ef, data[u] = hash_(u));
  }

  /* Read some random bytes.
   */
  u = 12345;
  external_file_set_position(ef, u);
  EXPECT_ULEQ(data[u], external_file_read_byte(ef));
  u++;
  EXPECT_ULEQ(data[u], external_file_read_byte(ef));
  u++;
  EXPECT_ULEQ(data[u], external_file_read_byte(ef));
  u++;

  /* Now append the last three bytes.
   */
  u = external_file_get_size(ef);
  external_file_write_byte(ef, data[u] = 'j');
  u++;
  external_file_write_byte(ef, data[u] = 'p');
  u++;
  external_file_write_byte(ef, data[u] = 'c');
  u++;

  /* Check the contents.
   */
  EXPECT_TRUE(external_file_contents_equal_bytes(ef, data, N_));

  deletev(&data);
  string_delete(&filename);
  external_file_delete(&ef);
}

TEST(ExternalFile, Exists) {
  char *filename = null;
  FILE *file;

  EXPECT_FALSE(external_file_exists("/badname/of/file.JPC"));
  string_new_temp_filename(&filename);
  file = fopen(filename, "w");
  EXPECT_TRUE(external_file_exists(filename));
  fclose(file);
  EXPECT_TRUE(external_file_exists(filename));
  string_delete(&filename);
}

TEST(Ansifile, ContentsEqual) {
  char e[] = "This is a test\nYes it is.\f\v";
  FILE *file = tmpfile();

  fprintf(file, "%s", e);
  EXPECT_TRUE(ansifile_contents_equal_string(file, e));
  fclose(file);
}

TEST(Ansifile, ContentsEqualFromFilename) {
  char e[] = "This is a test\nYes it is.\f\v";
  char *filename = null;
  FILE *file;

  string_new_temp_filename(&filename);
  file = fopen(filename, "w");
  fprintf(file, "%s", e);
  fclose(file);
  EXPECT_TRUE(
      ansifile_contents_equal_string_from_filename(filename, e));
  string_delete(&filename);
}

FINISH
