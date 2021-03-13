#include "File.h"

byte file_set_retries(byte retries) {
  return external_file_set_retries(retries);
}

byte file_get_retries(void) {
  return external_file_get_retries();
}

File *file_new(File **pthis, char *filename, char *mode) {
  if (!file_try_new(pthis, filename, mode)) {
    throw("Could not open file '%s' in mode '%s'", filename, mode);
  }

  return *pthis;
}

bool file_try_new(
    File **pthis,
    char *filename,
    char *mode) {
  if (mode == null) {
    file_new_memory_file(pthis);
    return true;
  }
  return file_try_new_external_file(pthis, filename, mode);
}

File *file_new_f(File **pthis, char *mode, char *fmt, ...) {
  va_list ap1;
  va_list ap2;

  va_start(ap1, fmt);
  va_start(ap2, fmt);
  file_new_vf(pthis, mode, fmt, ap1, ap2);
  va_end(ap1);
  va_end(ap2);

  return *pthis;
}

File *file_new_vf(
    File **pthis,
    char *mode,
    char *fmt,
    va_list ap1,
    va_list ap2) {
  char *filename = null;

  string_new_vf(&filename, fmt, ap1, ap2);
  file_new(pthis, filename, mode);
  string_delete(&filename);

  return *pthis;
}

bool file_try_new_f(File **pthis, char *mode, char *fmt, ...) {
  bool success;
  va_list ap1;
  va_list ap2;

  va_start(ap1, fmt);
  va_start(ap2, fmt);
  success = file_try_new_vf(pthis, mode, fmt, ap1, ap2);
  va_end(ap1);
  va_end(ap2);

  return success;
}

bool file_try_new_vf(
    File **pthis,
    char *mode,
    char *fmt,
    va_list ap1,
    va_list ap2) {
  bool success;
  char *filename = null;

  string_new_vf(&filename, fmt, ap1, ap2);
  success = file_try_new(pthis, filename, mode);
  string_delete(&filename);
  return success;
}

File *file_new_memory_file(File **pthis) {
  File *this;

  this = newo(pthis);
  memory_file_new(&this->mf);
  return this;
}

File *file_new_external_file(
    File **pthis,
    char *filename,
    char *mode) {
  byte retries;
  char *s = null;

  if (!file_try_new_external_file(pthis, filename, mode)) {
    if ((retries = file_get_retries()) != 0) {
      string_new_f(&s, " even with %d retries", retries);
    } else {
      string_new_empty(&s);
    }
    throw(
        "Could not open external file '%s' with mode '%s'%s",
        filename,
        mode,
        s);
  }
  return *pthis;
}

bool file_try_new_external_file(
    File **pthis,
    char *filename,
    char *mode) {
  File *this;

  this = newo(pthis);

  if (!external_file_try_new(&this->ef, filename, mode)) {
    file_delete(pthis);
    return false;
  }

  return true;
}

File *file_new_from_memory_file(File **pthis, MemoryFile **pmf) {
  File *this;

  this = newo(pthis);
  donate(pmf, &this->mf);
  return this;
}

File *file_new_from_external_file(File **pthis, ExternalFile **pef) {
  File *this;

  this = newo(pthis);
  donate(pef, &this->ef);
  return this;
}

File *file_new_memory_reference(File **pthis, File *other) {
  File *this;

  if (!file_is_memory_file(other)) {
    throw("Can only create memory reference of a MemoryFile");
  }

  this = newo(pthis);
  memory_file_new_reference(&this->mf, other->mf);
  return this;
}

File *file_new_wrap_ansifile(File **pthis, FILE *ansifile, char *mode) {
  File *this;

  this = newo(pthis);

  external_file_new_wrap_ansifile(&this->ef, ansifile, mode);

  return this;
}

File *file_new_memory_copy(
    File **pthis,
    char *other_filename) {
  return file_new_copy(pthis, null, null, other_filename);
}

File *file_new_copy(
    File **pthis,
    char *filename,
    char *mode,
    char *other_filename) {
  File *this;
  File *other = null;

  file_new(&other, other_filename, "r");
  this = file_new_copy_other(
      pthis,
      filename,
      mode,
      other);
  file_delete(&other);

  return this;
}

File *file_new_copy_other(
    File **pthis,
    char *filename,
    char *mode,
    File *other) {
  File *this;

  this = file_new(pthis, filename, mode);

  file_copy_other(this, other);

  return this;
}

void file_delete(File **pthis) {
  File *this;

  if ((this = *pthis) == null) {
    return;
  }

  memory_file_delete(&this->mf);
  external_file_delete(&this->ef);

  deleteo(pthis);
}

void file_print(File *this, FILE *f, byte n, bool do_contents) {
  fprintfn(f, n++, "[File]");
  if (file_is_memory_file(this)) {
    memory_file_print(this->mf, f, n, do_contents);
  } else {
    external_file_print(this->ef, f, n, do_contents);
  }
}

bool file_is_memory_file(File *this) {
  return this->mf != null;
}

bool file_is_external_file(File *this) {
  return this->ef != null;
}

char *file_get_filename(File *this) {
  return file_is_external_file(this) ? this->ef->filename : null;
}

ulong file_get_size(File *this) {
  if (file_is_memory_file(this)) {
    return memory_file_get_size(this->mf);
  } else {
    return external_file_get_size(this->ef);
  }
}

ulong file_get_position(File *this) {
  if (file_is_memory_file(this)) {
    return memory_file_get_position(this->mf);
  } else {
    return external_file_get_position(this->ef);
  }
}

void file_set_position(File *this, ulong position) {
  if (file_is_memory_file(this)) {
    memory_file_set_position(this->mf, position);
  } else {
    external_file_set_position(this->ef, position);
  }
}

bool file_is_eof(File *this) {
  if (file_is_memory_file(this)) {
    return memory_file_is_eof(this->mf);
  } else {
    return external_file_is_eof(this->ef);
  }
}

void file_rewind(File *this) {
  if (file_is_memory_file(this)) {
    memory_file_rewind(this->mf);
  } else {
    external_file_rewind(this->ef);
  }
}

void file_fast_forward(File *this) {
  if (file_is_memory_file(this)) {
    memory_file_fast_forward(this->mf);
  } else {
    external_file_fast_forward(this->ef);
  }
}

void file_backup_one_byte(File *this) {
  ulong pos;

  if ((pos = file_get_position(this)) == 0) {
    throw("Cannot backup one byte when file is at position 0");
  }

  file_set_position(this, pos - 1);
}

void file_flush(File *this) {
  if (file_is_external_file(this)) {
    external_file_flush(this->ef);
  }
}

void file_write_byte(File *this, byte b) {
  if (file_is_memory_file(this)) {
    memory_file_write_byte(this->mf, b);
  } else {
    external_file_write_byte(this->ef, b);
  }
}

void file_write_bytes(File *this, byte *bs, ulong n) {
  byte *p;
  byte *e;

  for (p = bs, e = p + n; p < e; p++) {
    file_write_byte(this, *p);
  }
}

void file_write_char(File *this, char c) {
  file_write_byte(this, (byte)c);
}

void file_write_string(File *this, char *s) {
  char c;
  char *p;

  for (p = s; (c = *p) != '\0'; p++) {
    file_write_byte(this, (byte)c);
  }
}

bool file_try_read_byte(File *this, byte *b) {
  if (file_is_memory_file(this)) {
    return memory_file_try_read_byte(this->mf, b);
  } else {
    return external_file_try_read_byte(this->ef, b);
  }
}

byte file_read_byte(File *this) {
  if (file_is_memory_file(this)) {
    return memory_file_read_byte(this->mf);
  } else {
    return external_file_read_byte(this->ef);
  }
}

void file_read_bytes(File *this, byte *bs, ulong n) {
  byte *p;
  byte *e;

  for (p = bs, e = p + n; p < e; p++) {
    *p = file_read_byte(this);
  }
}

bool file_contents_equal_bytes(File *this, byte *bytes, ulong len) {
  if (file_is_memory_file(this)) {
    return memory_file_contents_equal_bytes(this->mf, bytes, len);
  } else {
    return external_file_contents_equal_bytes(this->ef, bytes, len);
  }
}

void file_assert_contents_equal_bytes(File *this, byte *bytes, ulong len) {
  if (file_is_memory_file(this)) {
    memory_file_assert_contents_equal_bytes(this->mf, bytes, len);
  } else {
    external_file_assert_contents_equal_bytes(this->ef, bytes, len);
  }
}

void file_copy_other(File *this, File *other) {
  byte b;

  while (file_try_read_byte(other, &b)) {
    file_write_byte(this, b);
  }
}
