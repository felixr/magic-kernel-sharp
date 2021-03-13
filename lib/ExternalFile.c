#include "ExternalFile.h"
#include <ctype.h>
#include <limits.h>
#include <string.h>

typedef struct {
  FILE **pf;
  char *filename;
  char *new_filename;
  char *mode;
} Info_;

static bool retries_ = 0;

static bool retry_(bool (*f)(void *x), void *x);
static bool open_(void *x);
static bool remove_(void *x);
static bool rename_(void *x);
static bool try_open_(FILE **pf, char *filename, char *mode);
static void parse_mode_(ExternalFile *this, char *mode);
static char *make_partial_filename_(char **ppartial_filename, char *filename);

byte external_file_set_retries(byte retries) {
  byte current;

  current = retries_;
  retries_ = retries;
  return current;
}

byte external_file_get_retries(void) {
  return retries_;
}

ExternalFile *external_file_new(
    ExternalFile **pthis,
    char *filename,
    char *mode) {
  char *s = null;

  if (!external_file_try_new(pthis, filename, mode)) {
    if (retries_ != 0) {
      string_new_f(&s, " even after %d retries", retries_);
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

bool external_file_try_new(
    ExternalFile **pthis,
    char *filename,
    char *mode) {
  bool read;
  char *use_filename;
  ExternalFile *this = null;

  if (mode == null) {
    throw("Mode cannot be null");
  }

  if (filename == null) {
    if (string_equals(mode, "stdin")) {
      this = external_file_new_wrap_ansifile(pthis, stdin, "rt");
    } else if (string_equals(mode, "stdout")) {
      this = external_file_new_wrap_ansifile(pthis, stdout, "wt");
    } else if (string_equals(mode, "stderr")) {
      this = external_file_new_wrap_ansifile(pthis, stderr, "wt");
    } else {
      throw( "Mode for null filename must be 'stdin', 'stdout' or 'stderr'");
    }
    string_delete(&this->filename);
    string_new_concat_until_null(&this->filename, "(", mode, ")", null);
    return true;
  } else if (string_equals(filename, "-")) {
    this = external_file_new_wrap_ansifile(pthis, stdout, mode);
    read = this->read;
    string_delete(&this->mode);
    if (this->read) {
      this->ansifile = stdin;
      string_renew_copy(&this->filename, "(stdin)");
      parse_mode_(this, "rt");
    } else {
      string_renew_copy(&this->filename, "(stdin)");
      parse_mode_(this, "wt");
    }
    return true;
  }

  this = newo(pthis);

  string_new_copy(&this->filename, filename);

  parse_mode_(this, mode);

  if (this->write) {
    if (external_file_exists(filename)) {
      if (!external_file_is_writeable(filename)) {
        return false;
      }
      external_file_remove(filename);
    }
    use_filename = make_partial_filename_(&this->partialFilename, filename);
  } else {
    use_filename = filename;
  }

  if (string_ends_with(filename, ".gz")) {
    if ((this->gzfile = gzopen(use_filename, this->mode)) == null) {
      external_file_delete(pthis);
      return false;
    }
  } else {
    if (!try_open_(&this->ansifile, use_filename, this->mode)) {
      external_file_delete(pthis);
      return false;
    }
  }

  return true;
}

ExternalFile *external_file_new_wrap_ansifile(
    ExternalFile **pthis,
    FILE *ansifile,
    char *mode) {
  ExternalFile *this;

  this = newo(pthis);

  this->wrap = true;
  parse_mode_(this, mode);
  this->ansifile = ansifile;
  string_new_copy(&this->filename, "(wrapped ANSI FILE)");

  return this;
}

void external_file_delete(ExternalFile **pthis) {
  bool success;
  ExternalFile *this;

  if ((this = *pthis) == null) {
    return;
  }

  success = (this->ansifile != null || this->gzfile != null);
  if (!this->wrap && this->ansifile != null) {
    fclose(this->ansifile);
  }
  if (this->gzfile != null) {
    gzclose(this->gzfile);
  }
  if (success && this->partialFilename != null) {
    external_file_rename(this->partialFilename, this->filename);
  }
  string_delete(&this->mode);
  string_delete(&this->partialFilename);
  string_delete(&this->filename);

  deleteo(pthis);
}

void external_file_print(
    ExternalFile *this,
    FILE *f,
    byte n,
    bool do_contents) {
  byte hexits;
  byte block;
  byte b;
  byte c;
  char *s = null;
  ulong size;
  ulong position;
  ulong u;

  fprintfn(f, n++, "[ExternalFile]");
  fprintfn(f, n, "Filename: %lu", this->filename);
  if (this->partialFilename != null) {
    fprintfn(f, n, "Partial filename: %lu", this->partialFilename);
  }
  fprintfn(f, n, "Gzipped: %s", external_file_is_gzipped(this));
  fprintfn(f, n, "Mode: %lu", this->mode);
  size = external_file_get_size(this);
  position = external_file_get_position(this);
  fprintfn(f, n, "Size: %lu", size);
  fprintfn(f, n, "Current position: %lu", position);
  fprintfn(f, n, "Read: %s", bool_string(this->read));
  fprintfn(f, n, "Write: %s", bool_string(this->write));
  fprintfn(f, n, "Append: %s", bool_string(this->append));
  fprintfn(f, n, "Update: %s", bool_string(this->update));
  if (do_contents) {
    if (!this->read && !this->update) {
      throw("Cannot print contents of file in mode '%s'", this->mode);
    }
    fprintfn(f, n++, "Contents:");
    external_file_rewind(this);
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
                external_file_read_byte(this));
          }
        }
        string_renew_concat(&s, " ");
      }
      string_renew_concat(&s, " ");
      u -= 16;
      external_file_set_position(this, u);
      for (b = 0; b < 16; b++, u++) {
        if (u >= size) {
          c = ' ';
        } else {
          c = (char)external_file_read_byte(this);
          if (!isprint(c)) {
            c = '.';
          }
        }
        string_renew_concat_char(&s, c);
      }

      fprintfn(f, n, "%s", s);
      string_delete(&s);
    }
    external_file_set_position(this, position);
  }
}

bool external_file_is_gzipped(ExternalFile *this) {
  return this->gzfile != null;
}

ulong external_file_get_size(ExternalFile *this) {
  ulong size;
  fpos_t pos;

  if (external_file_is_gzipped(this)) {
    throw("Do not try to get the size of gzipped file '%s'", this->filename);
  }
  if (fgetpos(this->ansifile, &pos) != 0) {
    throw("Could not save file position for '%s'", this->filename);
  }
  if (fseek(this->ansifile, 0, SEEK_END) != 0) {
    throw("Could not set file '%s' position to end of file", this->filename);
  }
  size = external_file_get_position(this);
  if (fsetpos(this->ansifile, &pos) != 0) {
    throw("Could not restore file position for '%s'", this->filename);
  }

  return size;
}

ulong external_file_get_position(ExternalFile *this) {
  long position;

  if (external_file_is_gzipped(this)) {
    throw(
        "Do not try to get the position of gzipped file '%s'",
        this->filename);
  }
  if (this->text) {
    throw(
        "Do not try to get position for %s in text mode '%s'",
        this->filename,
        this->mode);
  }

  if ((position = ftell(this->ansifile)) < 0) {
    throw("Could not get position of file '%s'", this->filename);
  }
  return (ulong)position;
}

void external_file_set_position(ExternalFile *this, ulong position) {
  if (external_file_is_gzipped(this)) {
    throw(
        "Do not try to set the position of gzipped file '%s'",
        this->filename);
  }
  if (this->text) {
    throw(
        "Do not try to set position for %s in text mode '%s'",
        this->filename,
        this->mode);
  }

  if (fseek(this->ansifile, position, SEEK_SET) != 0) {
    throw(
        "Could not set file '%s' position to %lu in mode '%s'",
        this->filename,
        position,
        this->mode);
  }
}

bool external_file_is_eof(ExternalFile *this) {
  if (external_file_is_gzipped(this)) {
    return gzeof(this->gzfile);
  }

  return external_file_get_position(this) == external_file_get_size(this);
}

void external_file_rewind(ExternalFile *this) {
  if (external_file_is_gzipped(this)) {
    throw(
        "Do not try to rewind the gzipped file '%s'",
        this->filename);
  }
  external_file_set_position(this, 0);
}

void external_file_fast_forward(ExternalFile *this) {
  if (external_file_is_gzipped(this)) {
    throw(
        "Do not try to fast_forward the gzipped file '%s'",
        this->filename);
  }
  if (fseek(this->ansifile, 0, SEEK_END) != 0) {
    throw(
        "Could not fast-forward file '%s' in mode '%s'",
        this->filename,
        this->mode);
  }
}

void external_file_flush(ExternalFile *this) {
  if (external_file_is_gzipped(this)) {
    gzflush(this->gzfile, Z_FINISH);
  } else {
    fflush(this->ansifile);
  }
}

void external_file_write_byte(ExternalFile *this, byte b) {
  if (external_file_is_gzipped(this)) {
    if (gzputc(this->gzfile, b) < 0) {
      throw("Could not write byte to '%s'", this->filename);
    }
  } else {
    if (fputc(b, this->ansifile) == EOF) {
      throw("Could not write byte to '%s'", this->filename);
    }
  }
}

bool external_file_try_read_byte(ExternalFile *this, byte *b) {
  int c;

  if (external_file_is_gzipped(this)) {
    if ((c = gzgetc(this->gzfile)) < 0) {
      return false;
    }
  } else {
    if ((c = fgetc(this->ansifile)) == EOF) {
      return false;
    }
  }

  *b = (byte)c;
  return true;
}

byte external_file_read_byte(ExternalFile *this) {
  byte result = 0;

  if (!external_file_try_read_byte(this, &result)) {
    throw("Could not read byte from '%s'", this->filename);
  }

  return result;
}

bool external_file_contents_equal_bytes(
    ExternalFile *this,
    byte *bytes,
    ulong len) {
  byte read;
  byte *p;
  byte *e;

  external_file_rewind(this);

  for (p = bytes, e = p + len; p != e; p++) {
    if (!external_file_try_read_byte(this, &read)) {
      return false;
    }
    if (read != *p) {
      return false;
    }
  }

  if (!external_file_is_eof(this)) {
    return false;
  }

  return true;
}

void external_file_assert_contents_equal_bytes(
    ExternalFile *this,
    byte *bytes,
    ulong len) {
  byte read = 0;
  byte *p;
  byte *e;

  external_file_rewind(this);

  for (p = bytes, e = p + len; p != e; p++) {
    if (!external_file_try_read_byte(this, &read)) {
      throw("File '%s' had length %lu; expected %lu", p - bytes, len);
    }
    if (read != *p) {
      throw(
          "File '%s' disagreement at offset %lu: "
          "expected 0x%02x, actual 0x%02x",
          this->filename,
          p - bytes,
          *p,
          read);
    }
  }

  if (!external_file_is_eof(this)) {
    throw(
        "File '%s' is longer than expected: actual %lu, expected %lu",
        external_file_get_size(this),
        len);
  }
}

bool external_file_exists(char *filename) {
  FILE *f;

  if (try_open_(&f, filename, "r")) {
    fclose(f);
    return true;
  }
  return false;
}

bool external_file_exists_wait(char *filename, byte max_seconds) {
  uint seconds;

  for (seconds = 0; seconds < max_seconds; seconds++) {
    if (external_file_exists(filename)) {
      return true;
    }
    sleep(1);
  }
  return false;
}

bool external_file_partial_exists(char *filename) {
  bool result;
  char *partial_filename = null;

  make_partial_filename_(&partial_filename, filename);
  result = external_file_exists(partial_filename);
  string_delete(&partial_filename);

  return result;
}

bool external_file_either_exists(char *filename) {
  return
    external_file_exists(filename) ||
    external_file_partial_exists(filename);
}

bool external_file_is_empty(char *filename) {
  bool empty;
  byte b;
  FILE *f = null;

  if (!try_open_(&f, filename, "r")) {
    throw("Could not open '%s' for reading to check if it is empty", filename);
  }
  empty = fread(&b, 1, 1, f) != 1;
  fclose(f);
  return empty;
}

bool external_file_is_writeable(char *filename) {
  FILE *f = null;

  if (!try_open_(&f, filename, "r+")) {
    return false;
  }
  fclose(f);
  return true;
}

void external_file_create(char *filename) {
  FILE *f = null;

  if (external_file_exists(filename)) {
    throw("File '%s' already exists", filename);
  }
  if (!try_open_(&f, filename, "w")) {
    throw("Could not create file '%s'", filename);
  }
  fclose(f);
}

void external_file_remove(char *filename) {
  if (!external_file_try_remove(filename)) {
    throw("Could not remove file '%s'", filename);
  }
}

bool external_file_try_remove(char *filename) {
  Info_ info;

  info.filename = filename;

  return retry_(remove_, &info);
}

void external_file_remove_partial(char *filename) {
  char *partial_filename = null;

  make_partial_filename_(&partial_filename, filename);
  external_file_remove(partial_filename);
  string_delete(&partial_filename);
}

void external_file_remove_both(char *filename) {
  char *partial_filename = null;

  if (external_file_exists(filename)) {
    external_file_remove(filename);
  }

  make_partial_filename_(&partial_filename, filename);
  if (external_file_exists(partial_filename)) {
    external_file_remove(partial_filename);
  }
  string_delete(&partial_filename);
}

void external_file_rename(
    char *orig_filename,
    char *new_filename) {
  Info_ info;

  info.filename = orig_filename;
  info.new_filename = new_filename;

  if (!retry_(rename_, &info)) {
    throw("Could not rename file '%s' to '%s'", orig_filename, new_filename);
  }
}

ulong external_file_read_all(
    char *filename,
    byte **pblob) {
  byte retries;
  byte *blob;
  byte *e;
  byte *p;
  char *s = null;
  ulong len;
  long size;
  FILE *file = null;
  File *f = null;

  if (string_equals(filename, "-")) {
    file_new_memory_copy(&f, filename);
    len = file_get_size(f);
    blob = new(pblob, len);
    file_rewind(f);
    for (p = blob, e = p + len; p != e; p++) {
      *p = file_read_byte(f);
    }
    file_delete(&f);
    return len;
  }

  if (!try_open_(&file, filename, "r")) {
    if ((retries = file_get_retries()) != 0) {
      string_new_f(&s, " even with %d retries", retries);
    } else {
      string_new_empty(&s);
    }
    throw("Could not open file '%s' in mode 'r'%s", filename, s);
  }

  fseek(file, 0, SEEK_END);
  if ((size = ftell(file)) < 0 || size == LONG_MAX) {
    throw("Could not read from file '%s'", filename);
  }

  len = (ulong)size;
  blob = new(pblob, len);
  fseek(file, 0, SEEK_SET);
  fread(blob, 1, len, file);
  fclose(file);
  return len;
}

char *external_file_read_all_into_string(
    char *filename,
    char **ps) {
  char *s;
  char *p;
  char *e;
  ulong len;
  ExternalFile *this = null;

  external_file_new(&this, filename, "r");
  len = external_file_get_size(this);
  s = new(ps, len + 1);
  for (p = s, e = s + len; p < e; p++) {
    *p = (char)external_file_read_byte(this);
  }
  *p = '\0';
  external_file_delete(&this);
  return s;
}

int external_file_status(char *filename) {
  bool exists;
  bool partial_exists;

  partial_exists = external_file_partial_exists(filename);
  exists = external_file_exists(filename);

  if (!partial_exists && !exists) {
    return -1;
  }
  if (partial_exists) {
    return 0;
  }
  return +1;
}

int external_files_status(ulong num_filenames, char **filenames) {
  bool all_not_started;
  bool all_finished;
  int status;
  ulong u;

  all_not_started = true;
  all_finished = true;

  for (u = 0; u < num_filenames; u++) {
    status = external_file_status(filenames[u]);
    if (status <= 0) {
      all_finished = false;
    }
    if (status >= 0) {
      all_not_started = false;
    }
  }
  return all_finished ? +1 : all_not_started ? -1 : 0;
}

bool ansifile_contents_equal_bytes(
    FILE *file,
    byte *expected_contents,
    ulong len) {
  byte *p;
  byte *e;

  rewind(file);

  for (p = expected_contents, e = p + len; p < e; p++) {
    if (fgetc(file) != *p) {
      return false;
    }
  }

  if (fgetc(file) != EOF) {
    return false;
  }

  return true;
}

bool ansifile_contents_equal_bytes_from_filename(
    char *filename,
    byte *expected_contents,
    ulong len) {
  bool result;
  FILE *file;

  if ((file = fopen(filename, "r")) == null) {
    throw("Could not open file '%s'", filename);
  }
  result = ansifile_contents_equal_bytes(file, expected_contents, len);

  fclose(file);

  return result;
}

bool ansifile_contents_equal_string(
    FILE *file,
    char *expected_contents) {
  return ansifile_contents_equal_bytes(
      file,
      (byte *)expected_contents,
      strlen(expected_contents));
}

bool ansifile_contents_equal_string_from_filename(
    char *filename,
    char *expected_contents) {
  return ansifile_contents_equal_bytes_from_filename(
      filename,
      (byte *)expected_contents,
      strlen(expected_contents));
}

bool retry_(bool (*f)(void *x), void *x) {
  byte t;
  int time;

  for (t = 0, time = 0; t < 1 + retries_; t++, time = time * 2 + 1) {
    if (t > 0) {
      sleep(time);
    }
    if (f(x)) {
      return true;
    }
  }
  return false;
}

static void parse_mode_(ExternalFile *this, char *mode) {
  bool includes_b;
  char c;
  char *p;

  includes_b = false;

  for (p = mode; (c = *p) != '\0'; p++) {
    switch (c) {
      case 'r':
        this->read = true;
        break;
      case 'w':
        this->write = true;
        break;
      case 'a':
        this->append = true;
        break;
      case '+':
        this->update = true;
        break;
      case 'b':
        includes_b = true;
        break;
      case 't':
        if (!this->wrap) {
          throw(
              "Text mode not supported for non-wrapped ExternalFile "
              "(mode '%s' file '%s')",
              mode,
              this->filename);
        }
        this->text = true;
        break;
      default:
        throw(
            "Unknown mode character '%c' in mode string '%s' opening '%s'",
            c,
            mode,
            this->filename);
        break;
    }
  }

  if (this->append) {
    if (this->read) {
      throw(
          "Cannot include both 'a' and 'r' in mode (mode '%s' file '%s')",
          mode,
          this->filename);
    }
    if (this->write) {
      throw(
          "Cannot include both 'a' and 'w' in mode (mode '%s' file '%s')",
          mode,
          this->wrap ? "wrapped ANSI FILE" : this->filename);
    }
  }

  /* Even though there is no text translation mode needed in Unix and Linux,
   * opening in text mode prevents seeking (because on other architectures
   * there is no way of predicting binary offset from text offset). So always
   * open files in ANSI binary mode.
   */
  if (this->wrap || includes_b) {
    string_new_copy(&this->mode, mode);
  } else {
    string_new_concat_until_null(&this->mode, mode, "b", null);
  }
}

static char *make_partial_filename_(char **ppartial_filename, char *filename) {
  string_new_concat_until_null(ppartial_filename, filename, ".partial", null);
  return *ppartial_filename;
}

static bool open_(void *x) {
  FILE *f;
  Info_ *info;

  info = x;

  if ((f = fopen(info->filename, info->mode)) != null) {
    *info->pf = f;
    return true;
  }
  return false;
}

static bool remove_(void *x) {
  Info_ *info;

  info = x;

  return remove(info->filename) == 0;
}

static bool rename_(void *x) {
  Info_ *info;

  info = x;

  return rename(info->filename, info->new_filename) == 0;
}

static bool try_open_(FILE **pf, char *filename, char *mode) {
  Info_ info;

  info.pf = pf;
  info.filename = filename;
  info.mode = mode;

  return retry_(open_, &info);
}
