#include "MemoryFileTestDeath.h"

START_DEATH

TEST(MemoryFile, Basics) {
  MemoryFile *mf = null;
  MemoryFile *ref = null;

  memory_file_new(&mf);

  /* Empty file.
   */
  EXPECT_DEATH(memory_file_read_byte(mf), "Cannot read byte: end of file");

  /* Write first byte.
   */
  memory_file_write_byte(mf, 'j');
  EXPECT_DEATH(memory_file_read_byte(mf), "Cannot read byte: end of file");
  EXPECT_DEATH(memory_file_read_byte(mf), "Cannot read byte: end of file");
  memory_file_set_position(mf, 1);

  /* Write second byte.
   */
  EXPECT_DEATH(memory_file_read_byte(mf), "Cannot read byte: end of file");
  memory_file_set_position(mf, 0);
  memory_file_set_position(mf, 2);
  memory_file_set_position(mf, 1);
  memory_file_set_position(mf, 0);

  /* Write third byte.
   */
  memory_file_fast_forward(mf);
  memory_file_write_byte(mf, 'c');
  EXPECT_DEATH(memory_file_read_byte(mf), "Cannot read byte: end of file");
  memory_file_set_position(mf, 0);
  memory_file_set_position(mf, 3);
  memory_file_set_position(mf, 2);
  memory_file_set_position(mf, 1);
  memory_file_set_position(mf, 0);

  /* Overwrite second byte.
   */
  memory_file_set_position(mf, 1);
  memory_file_write_byte(mf, 'h');
  memory_file_rewind(mf);

  /* Overwrite first byte.
   */
  memory_file_rewind(mf);
  memory_file_write_byte(mf, 'm');
  memory_file_rewind(mf);

  /* Create a reference.
   */
  memory_file_new_reference(&ref, mf);

  /* Check its contents, and independently that of the original file.
   */
  memory_file_rewind(ref);
  memory_file_rewind(mf);

  /* Delete the reference.
   */
  memory_file_delete(&ref);

  /* Check the original.
   */
  memory_file_rewind(mf);

  /* Original should still be locked.
   */
  EXPECT_DEATH(
      memory_file_write_byte(mf, 'a'),
      "Cannot write to locked MemoryFile: one of more references exist");
  memory_file_delete(&mf);
}

FINISH_DEATH
