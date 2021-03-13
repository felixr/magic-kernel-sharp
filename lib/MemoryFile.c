#include "MemoryFile.h"
#include <ctype.h>

/* Lengthen the file capacity. Returns whether a new chunk was added. (The
 * first chunk can be lengthened before a second chunk needs to be added.)
 */
static bool lengthen_(MemoryFile *this);

MemoryFile *memory_file_new(MemoryFile **pthis) {
  MemoryFile *this;

  this = newo(pthis);

  lengthen_(this);

  return this;
}

MemoryFile *memory_file_new_reference(MemoryFile **pthis, MemoryFile *other) {
  MemoryFile *this;

  this = newo(pthis);
  other->locked = true;
  *this = *other;
  this->reference = true;

  return this;
}

void memory_file_delete(MemoryFile **pthis) {
  MemoryFile *this;

  if ((this = *pthis) == null) {
    return;
  }

  if (!this->reference) {
    delete(&this->chunks, this->numChunks, value_array_delete);
  }

  deleteo(pthis);
}

void memory_file_print(MemoryFile *this, FILE *f, byte n, bool do_contents) {
  byte hexits;
  byte block;
  byte b;
  byte c;
  char *s = null;
  ulong size;
  ulong position;
  ulong u;

  fprintfn(f, n++, "[MemoryFile]");
  size = memory_file_get_size(this);
  position = memory_file_get_position(this);
  fprintfn(f, n, "Size: %lu", size);
  fprintfn(f, n, "Current position: %lu", position);
  fprintfn(f, n, "Number of chunks: %lu", this->numChunks);
  fprintfn(f, n, "Chunk index: %lu", this->chunkIndex);
  fprintfn(f, n, "Chunk offset: %u", this->chunkOffset);
  fprintfn(f, n, "Last chunk used: %lu", this->lastChunkUsed);
  fprintfn(f, n, "First chunk size: %lu", this->firstChunkSize);
  if (do_contents) {
    fprintfn(f, n++, "Contents:");
    memory_file_rewind(this);
    hexits = ulong_hexit_width(size - 1);

    for (u = 0; u < size;) {
      string_new_f(&s, "%0*lx: ", (int)hexits, u);
      for (block = 0; block < 8; block++) {
        for (b = 0; b < 2; b++, u++) {
          if (u >= size) {
            string_renew_concat(&s, "  ");
          } else {
            string_renew_concat_f(
                &s,
                "%02x",
                memory_file_read_byte(this));
          }
        }
        string_renew_concat(&s, " ");
      }
      string_renew_concat(&s, " ");
      u -= 16;
      memory_file_set_position(this, u);
      for (b = 0; b < 16; b++, u++) {
        if (u >= size) {
          c = ' ';
        } else {
          c = (char)memory_file_read_byte(this);
          if (!isprint(c)) {
            c = '.';
          }
        }
        string_renew_concat_char(&s, c);
      }

      fprintfn(f, n, "%s", s);
      string_delete(&s);
    }
    memory_file_set_position(this, position);
  }
}

ulong memory_file_get_size(MemoryFile *this) {
  return ((this->numChunks - 1) << 16) + this->lastChunkUsed;
}

ulong memory_file_get_position(MemoryFile *this) {
  return (this->chunkIndex << 16) + this->chunkOffset;
}

void memory_file_set_position(MemoryFile *this, ulong position) {
  ushort chunk_offset;
  ulong chunk_index;

  chunk_index = this->chunkIndex = position >> 16;
  chunk_offset = this->chunkOffset = (ushort)position;

  if (chunk_index > 0) {
    while (this->numChunks <= chunk_index) {
      lengthen_(this);
    }
  } else {
    while (
        this->numChunks < 2 &&
        this->firstChunkSize > 0 &&
        this->firstChunkSize <= chunk_offset) {
      lengthen_(this);
    }
  }

  if (chunk_index + 1 == this->numChunks &&
      chunk_offset > this->lastChunkUsed) {
    this->lastChunkUsed = chunk_offset;
  }
}

bool memory_file_is_eof(MemoryFile *this) {
  return memory_file_get_position(this) == memory_file_get_size(this);
}

void memory_file_rewind(MemoryFile *this) {
  memory_file_set_position(this, 0);
}

void memory_file_fast_forward(MemoryFile *this) {
  memory_file_set_position(this, memory_file_get_size(this));
}

void memory_file_write_byte(MemoryFile *this, byte b) {
  bool in_last_chunk;
  ushort chunk_offset;
  ulong num_chunks;
  ulong chunk_index;

  if (this->locked) {
    throw("Cannot write to locked MemoryFile: one of more references exist");
  }

  num_chunks = this->numChunks;
  chunk_offset = this->chunkOffset;
  chunk_index = this->chunkIndex;

  in_last_chunk = (chunk_index + 1 == num_chunks);

  this->chunks[chunk_index][chunk_offset] = b;

  chunk_offset++;

  if (chunk_offset == 0) {
    if (in_last_chunk) {
      lengthen_(this);
    }
    this->chunkIndex++;
  } else if (num_chunks == 1 && chunk_offset == this->firstChunkSize) {
    if (lengthen_(this)) {
      this->chunkIndex++;
    }
  }

  this->chunkOffset = chunk_offset;

  if (in_last_chunk) {
    if (chunk_offset > this->lastChunkUsed) {
      this->lastChunkUsed = chunk_offset;
    }
  }
}

bool memory_file_try_read_byte(MemoryFile *this, byte *b) {
  ushort chunk_offset;
  ulong num_chunks;
  ulong chunk_index;

  num_chunks = this->numChunks;
  chunk_offset = this->chunkOffset;
  chunk_index = this->chunkIndex;

  if (chunk_index + 1 == num_chunks && chunk_offset == this->lastChunkUsed) {
    return false;
  }

  *b = this->chunks[chunk_index][chunk_offset];

  chunk_offset++;

  if (chunk_offset == 0) {
    this->chunkIndex++;
  }
  this->chunkOffset = chunk_offset;

  return true;
}

byte memory_file_read_byte(MemoryFile *this) {
  byte result = 0;

  if (!memory_file_try_read_byte(this, &result)) {
    throw("Cannot read byte: end of file");
  }

  return result;
}

bool memory_file_contents_equal_bytes(
    MemoryFile *this,
    byte *bytes,
    ulong len) {
  byte read;
  byte *p;
  byte *e;

  memory_file_rewind(this);

  for (p = bytes, e = p + len; p != e; p++) {
    if (!memory_file_try_read_byte(this, &read)) {
      return false;
    }
    if (read != *p) {
      return false;
    }
  }

  if (!memory_file_is_eof(this)) {
    return false;
  }

  return true;
}

void memory_file_assert_contents_equal_bytes(
    MemoryFile *this,
    byte *bytes,
    ulong len) {
  byte read = 0;
  byte *p;
  byte *e;

  memory_file_rewind(this);

  for (p = bytes, e = p + len; p != e; p++) {
    if (!memory_file_try_read_byte(this, &read)) {
      throw(
          "Memory file shorter than expected: "
          "actual %lu, expected %lu",
          p - bytes,
          len);
    }
    if (read != *p) {
      throw(
          "Disagreement in memory file at offset %lu: "
          "actual 0x%02x, expected 0x%02x",
          p - bytes,
          read,
          *p);
    }
  }

  if (!memory_file_is_eof(this)) {
      throw(
          "Memory file longer than expected: "
          "actual %lu, expected %lu",
          memory_file_get_size(this),
          len);
  }
}

static bool lengthen_(MemoryFile *this) {
  ushort old_chunk_size;
  ulong new_chunk_size;
  ulong num_chunks;

  num_chunks = this->numChunks;

  if (num_chunks > 1) {
    renewup(&this->chunks, num_chunks, num_chunks + 1);
    new(&this->chunks[num_chunks], 0x10000);
    this->numChunks = num_chunks + 1;
    this->lastChunkUsed = 0;
    return true;
  }

  if (num_chunks == 0) {
    /* Only called in the constructor.
     */
    new(&this->chunks, 1);
    new(&this->chunks[0], this->firstChunkSize = 0x10);
    this->numChunks = 1;
    return true;
  }

  /* num_chunks == 1.
   */
  if ((old_chunk_size = this->firstChunkSize) == 0) {
    renewup(&this->chunks, 1, 2);
    new(&this->chunks[1], 0x10000);
    this->numChunks = 2;
    this->lastChunkUsed = 0;
    return true;
  }
  new_chunk_size = (ulong)old_chunk_size << 1;
  this->firstChunkSize = (ushort)new_chunk_size;

  renewup(&this->chunks[0], old_chunk_size, new_chunk_size);
  return false;
}
