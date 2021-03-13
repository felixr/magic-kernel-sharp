#ifndef MEMORY_FILE_H
#define MEMORY_FILE_H

#ifdef __cplusplus
extern "C" {
#endif

#include "bits.h"

typedef struct {
  boolbit reference: 1;
  boolbit locked: 1;
  ulong numChunks;
  byte **chunks;
  ulong chunkIndex;
  ushort chunkOffset;
  ushort lastChunkUsed;
  ushort firstChunkSize;
} MemoryFile;

MemoryFile *memory_file_new(MemoryFile **pthis);
MemoryFile *memory_file_new_reference(MemoryFile **pthis, MemoryFile *other);
void memory_file_delete(MemoryFile **pthis);

void memory_file_print(MemoryFile *this, FILE *s, byte n, bool do_contents);

void memory_file_write_byte(MemoryFile *this, byte b);
bool memory_file_try_read_byte(MemoryFile *this, byte *b);
byte memory_file_read_byte(MemoryFile *this);

ulong memory_file_get_size(MemoryFile *this);

ulong memory_file_get_position(MemoryFile *this);
bool memory_file_is_eof(MemoryFile *this);

void memory_file_set_position(MemoryFile *this, ulong position);
void memory_file_rewind(MemoryFile *this);
void memory_file_fast_forward(MemoryFile *this);

bool memory_file_contents_equal_bytes(
    MemoryFile *this,
    byte *bytes,
    ulong len);
void memory_file_assert_contents_equal_bytes(
    MemoryFile *this,
    byte *bytes,
    ulong len);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* _H */
