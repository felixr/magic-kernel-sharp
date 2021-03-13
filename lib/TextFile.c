#include "TextFile.h"
#include <stdarg.h>

static void error_(char *filename, char *mode);
static TextFile *construct_(TextFile **pthis);

byte text_file_set_retries(byte retries) {
  return external_file_set_retries(retries);
}

byte text_file_get_retries(void) {
  return external_file_get_retries();
}

TextFile *text_file_new(
    TextFile **pthis,
    char *filename,
    char *mode) {
  if (!text_file_try_new(pthis, filename, mode)) {
    error_(filename, mode);
  }
  return *pthis;
}

TextFile *text_file_new_csv(
    TextFile **pthis,
    char *filename,
    char *mode) {
  if (!text_file_try_new_csv(pthis, filename, mode)) {
    error_(filename, mode);
  }
  return *pthis;
}

TextFile *text_file_new_tsv(
    TextFile **pthis,
    char *filename,
    char *mode) {
  if (!text_file_try_new_tsv(pthis, filename, mode)) {
    error_(filename, mode);
  }
  return *pthis;
}

TextFile *text_file_new_hsv(
    TextFile **pthis,
    char *filename,
    char *mode) {
  if (!text_file_try_new_hsv(pthis, filename, mode)) {
    error_(filename, mode);
  }
  return *pthis;
}

TextFile *text_file_new_nsv(
    TextFile **pthis,
    char *filename,
    char *mode) {
  return text_file_new(pthis, filename, mode);
}

TextFile *text_file_new_type(
    TextFile **pthis,
    char *filename,
    char *mode,
    char *filetype) {
  TextFile *this;

  this = text_file_new(pthis, filename, mode);
  text_file_set_type(this, filetype);

  return this;
}

TextFile *text_file_new_stdin(TextFile **pthis) {
  return text_file_new(pthis, null, "stdin");
}

TextFile *text_file_new_stdin_csv(TextFile **pthis) {
  return text_file_new_csv(pthis, null, "stdin");
}

TextFile *text_file_new_stdin_tsv(TextFile **pthis) {
  return text_file_new_tsv(pthis, null, "stdin");
}

TextFile *text_file_new_stdin_hsv(TextFile **pthis) {
  return text_file_new_hsv(pthis, null, "stdin");
}

TextFile *text_file_new_stdout(TextFile **pthis) {
  return text_file_new(pthis, null, "stdout");
}

TextFile *text_file_new_stdout_csv(TextFile **pthis) {
  return text_file_new_csv(pthis, null, "stdout");
}

TextFile *text_file_new_stdout_tsv(TextFile **pthis) {
  return text_file_new_tsv(pthis, null, "stdout");
}

TextFile *text_file_new_stdout_hsv(TextFile **pthis) {
  return text_file_new_hsv(pthis, null, "stdout");
}

TextFile *text_file_new_stderr(TextFile **pthis) {
  return text_file_new(pthis, null, "stderr");
}

TextFile *text_file_new_stderr_csv(TextFile **pthis) {
  return text_file_new_csv(pthis, null, "stderr");
}

TextFile *text_file_new_stderr_tsv(TextFile **pthis) {
  return text_file_new_tsv(pthis, null, "stderr");
}

TextFile *text_file_new_stderr_hsv(TextFile **pthis) {
  return text_file_new_hsv(pthis, null, "stderr");
}

TextFile *text_file_new_memory(TextFile **pthis) {
  return text_file_new(pthis, null, null);
}

TextFile *text_file_new_memory_csv(TextFile **pthis) {
  return text_file_new_csv(pthis, null, null);
}

TextFile *text_file_new_memory_tsv(TextFile **pthis) {
  return text_file_new_tsv(pthis, null, null);
}

TextFile *text_file_new_memory_hsv(TextFile **pthis) {
  return text_file_new_hsv(pthis, null, null);
}

TextFile *text_file_new_memory_reference(TextFile **pthis, TextFile *other) {
  TextFile *this;

  if (!file_is_memory_file(other->file)) {
    throw("Can only create memory reference of a MemoryFile");
  }

  this = construct_(pthis);
  file_new_memory_reference(&this->file, other->file);
  text_file_set_type(this, text_file_get_type(other));
  return this;
}

TextFile *text_file_new_memory_copy(
    TextFile **pthis,
    char *other_filename,
    char *filetype) {
  TextFile *this;

  this = construct_(pthis);

  file_new_memory_copy(&this->file, other_filename);
  text_file_set_type(this, filetype);
  return this;
}

TextFile *text_file_new_csv_memory_copy(
    TextFile **pthis,
    char *other_filename) {
  TextFile *this;

  this = construct_(pthis);

  file_new_memory_copy(&this->file, other_filename);
  text_file_set_csv(this);
  return this;
}

TextFile *text_file_new_tsv_memory_copy(
    TextFile **pthis,
    char *other_filename) {
  TextFile *this;

  this = construct_(pthis);

  file_new_memory_copy(&this->file, other_filename);
  text_file_set_tsv(this);
  return this;
}

TextFile *text_file_new_hsv_memory_copy(
    TextFile **pthis,
    char *other_filename) {
  TextFile *this;

  this = construct_(pthis);

  file_new_memory_copy(&this->file, other_filename);
  text_file_set_hsv(this);
  return this;
}

bool text_file_try_new(
    TextFile **pthis,
    char *filename,
    char *mode) {
  TextFile *this;

  this = construct_(pthis);

  if (!file_try_new(&this->file, filename, mode)) {
    text_file_delete(pthis);
    return false;
  }

  return true;
}

bool text_file_try_new_csv(
    TextFile **pthis,
    char *filename,
    char *mode) {
  TextFile *this;

  if (!text_file_try_new(pthis, filename, mode)) {
    return false;
  }
  this = *pthis;
  text_file_set_csv(this);
  return true;
}

bool text_file_try_new_tsv(
    TextFile **pthis,
    char *filename,
    char *mode) {
  TextFile *this;

  if (!text_file_try_new(pthis, filename, mode)) {
    return false;
  }
  this = *pthis;
  text_file_set_tsv(this);
  return true;
}

bool text_file_try_new_hsv(
    TextFile **pthis,
    char *filename,
    char *mode) {
  TextFile *this;

  if (!text_file_try_new(pthis, filename, mode)) {
    return false;
  }
  this = *pthis;
  text_file_set_hsv(this);
  return true;
}

void text_file_delete(TextFile **pthis) {
  TextFile *this;

  if ((this = *pthis) == null) {
    return;
  }

  string_delete(&this->record);
  deletev(&this->fields);

  file_delete(&this->file);

  deleteo(pthis);
}

char *text_file_get_type(TextFile *this) {
  switch (this->separator) {
    case ',':
      return "csv";
    case '\t':
      return "tsv";
    case '\x01':
      return "hsv";
    default:
      return "unknown";
  }
}

void text_file_set_csv(TextFile *this) {
  text_file_set_separator(this, ',');
}

void text_file_set_tsv(TextFile *this) {
  text_file_set_separator(this, '\t');
}

void text_file_set_hsv(TextFile *this) {
  text_file_set_separator(this, 0x01);
}

void text_file_set_type(TextFile *this, char *filetype) {
  char separator = '\0';

  if (filetype == null ||
      string_equals(filetype, "txt") ||
      string_equals(filetype, "nsv")) {
    separator = '\0';
  } else if (string_equals(filetype, "csv")) {
    separator = ',';
  } else if (string_equals(filetype, "tsv")) {
    separator = '\t';
  } else if (string_equals(filetype, "hsv")) {
    separator = 0x01;
  } else {
    throw("TextFile filetype '%s' not recognized", filetype);
  }
  text_file_set_separator(this, separator);
}

void text_file_set_separator(TextFile *this, char separator) {
  this->separator = separator;
}

void text_file_print(TextFile *this, FILE *f, byte n, bool do_contents) {
  char **fields;
  ulong num_fields;
  ulong u;
  ulong r;

  fprintfn(f, n++, "[TextFile]");
  fprintfn(f, n, "Separator: 0x%02x", this->separator);
  if (do_contents) {
    fprintfn(f, n++, "Records:");
    text_file_rewind(this);
    r = 0;
    while (text_file_try_read(this, &num_fields, &fields)) {
      fprintfn(f, n++, "Record %lu:", r);
      fprintfn(f, n, "Number of fields: %lu", num_fields);
      for (u = 0; u < num_fields; u++) {
        fprintfn(f, n, "Field %lu: '%s'", u, fields[u]);
      }
      n--;
      r++;
    }
    n--;
  }
  file_print(this->file, f, n, do_contents);
}

void text_file_write(TextFile *this, char *s) {
  char c;
  char *p;

  if (this->currentFieldIndex > 0) {
    file_write_byte(this->file, (byte)this->separator);
  }
  for (p = s; (c = *p) != '\0'; p++) {
    file_write_byte(this->file, (byte)c);
  }
  this->currentFieldIndex++;
}

void text_file_write_f(TextFile *this, char *fmt, ...) {
  char *field = null;
  ulong len;
  va_list ap;

  va_start(ap, fmt);
  len = string_vf_get_max_len(fmt, ap);
  va_end(ap);

  new(&field, len + 1);

  va_start(ap, fmt);
  string_vf_print_into(field, fmt, ap, len);
  va_end(ap);

  text_file_write(this, field);

  string_delete(&field);
}

void text_file_write_bool(TextFile *this, bool b) {
  text_file_write(this, bool_string(b));
}

void text_file_write_byte(TextFile *this, byte b) {
  text_file_write_ulong(this, b);
}

void text_file_write_ulong(TextFile *this, ulong u) {
  text_file_write_f(this, "%lu", u);
}

void text_file_write_long(TextFile *this, long l) {
  text_file_write_f(this, "%ld", l);
}

void text_file_write_double(TextFile *this, double d) {
  char *s = null;

  string_new_double(&s, d);
  text_file_write(this, s);
  string_delete(&s);
}

void text_file_write_empty_field(TextFile *this) {
  text_file_write_f(this, "");
}

void text_file_end_record(TextFile *this) {
  file_write_byte(this->file, (byte)'\n');
  this->currentFieldIndex = 0;
}

void text_file_write_empty_record(TextFile *this) {
  text_file_end_record(this);
}

void text_file_write_single(TextFile *this, char *s) {
  text_file_write(this, s);
  text_file_end_record(this);
}

void text_file_write_single_ulong(TextFile *this, ulong u) {
  text_file_write_ulong(this, u);
  text_file_end_record(this);
}

void text_file_write_single_long(TextFile *this, long l) {
  text_file_write_long(this, l);
  text_file_end_record(this);
}

void text_file_write_single_double(TextFile *this, double d) {
  text_file_write_double(this, d);
  text_file_end_record(this);
}

void text_file_write_label(TextFile *this, char *label) {
  text_file_write_single(this, label);
}

void text_file_write_labeled(TextFile *this, char *label, char *s) {
  text_file_write(this, label);
  text_file_write(this, s);
  text_file_end_record(this);
}

void text_file_write_labeled_bool(TextFile *this, char *label, bool b) {
  text_file_write(this, label);
  text_file_write_bool(this, b);
  text_file_end_record(this);
}

void text_file_write_labeled_byte(TextFile *this, char *label, byte b) {
  text_file_write_labeled_ulong(this, label, b);
}

void text_file_write_labeled_ulong(TextFile *this, char *label, ulong u) {
  text_file_write(this, label);
  text_file_write_ulong(this, u);
  text_file_end_record(this);
}

void text_file_write_sole(char *filename, char *s) {
  TextFile *tf = null;

  text_file_new(&tf, filename, "w");
  text_file_write_single(tf, s);
  text_file_delete(&tf);
}

void text_file_write_sole_ulong(char *filename, ulong u) {
  TextFile *tf = null;

  text_file_new(&tf, filename, "w");
  text_file_write_single_ulong(tf, u);
  text_file_delete(&tf);
}

ulong text_file_read(TextFile *this, char ***fields) {
  ulong num_fields;

  if (!text_file_try_read(this, &num_fields, fields)) {
    throw( "Couldn't read record");
  }

  return num_fields;
}

ulong text_file_read_copy(TextFile *this, char ***pfields) {
  char **fields;
  char **ref;
  ulong f;
  ulong num_fields;

  num_fields = text_file_read(this, &ref);
  fields = new(pfields, num_fields);

  for (f = 0; f < num_fields; f++) {
    string_new_copy(&fields[f], ref[f]);
  }

  return num_fields;
}

bool text_file_try_read(TextFile *this, ulong *num_fields, char ***fields) {
  bool eof;
  byte read;
  char c;
  char sep;
  char *p;
  char *pe;
  char **f;
  char **fe;
  ulong max;
  ulong new_max;
  File *file;

  sep = this->separator;
  file = this->file;

  max = this->maxNumRecordChars;

  p = this->record;
  pe = p + max;

  eof = false;

  for (;;) {
    if (!file_try_read_byte(file, &read)) {
      *p = '\0';
      eof = true;
      break;
    }
    c = (char)read;
    if (c == '\n') {
      if (this->justReadCR) {
        this->justReadCR = false;
        continue;
      }
      this->justReadCR = false;
      *p = '\0';
      break;
    }
    if (c == '\r') {
      this->justReadCR = true;
      *p = '\0';
      break;
    } else {
      this->justReadCR = false;
    }
    *p = c;
    if (++p == pe) {
      new_max = max << 1;
      renewup(&this->record, max, new_max);
      this->maxNumRecordChars = new_max;
      p = this->record + max;
      pe = this->record + new_max;
      max = new_max;
    }
  }

  pe = p;
  p = this->record;

  this->numRecordChars = pe - p + 1;

  max = this->maxNumFields;

  f = this->fields;
  fe = f + max;

  while (p < pe) {
    *f = p;
    if (++f == fe) {
      new_max = max << 1;
      renewup(&this->fields, max, new_max);
      this->maxNumFields = new_max;
      f = this->fields + max;
      fe = this->fields + new_max;
      max = new_max;
    }

    for (; p < pe; p++) {
      if (*p == sep) {
        *p++ = '\0';
        if (p == pe) {
          *f++ = p;
        }
        break;
      }
    }
  }

  this->numFields = *num_fields = f - this->fields;
  *fields = this->fields;

  return !eof || *num_fields > 0;
}

ulong text_file_get_num_record_chars(TextFile *this) {
  return this->numRecordChars;
}

void text_file_read_discard(TextFile *this) {
  char **fields;

  text_file_read(this, &fields);
}

void text_file_read_empty_record(TextFile *this) {
  char **fields;
  ulong num_fields;

  num_fields = text_file_read(this, &fields);
  if (num_fields > 0) {
    throw(
        "Record was not empty: it had %lu field%s",
        num_fields,
        plural_s(num_fields));
  }
}

bool text_file_try_copy(TextFile *src, TextFile *dst) {
  char **fields;
  ulong num_fields;
  ulong f;

  if (!text_file_try_read(src, &num_fields, &fields)) {
    return false;
  }

  for (f = 0; f < num_fields; f++) {
    text_file_write(dst, fields[f]);
  }
  text_file_end_record(dst);

  return true;
}

char *text_file_read_single(TextFile *this) {
  char *s;

  if (!text_file_try_read_single(this, &s)) {
    throw("Could not read from file");
  }
  return s;
}

bool text_file_try_read_single(TextFile *this, char **s) {
  char **fields;
  ulong num_fields;

  if (!text_file_try_read(this, &num_fields, &fields)) {
    *s = null;
    return false;
  }
  if (num_fields == 0) {
    *s = "";
    return true;
  }
  if (num_fields == 1) {
    *s = fields[0];
    return true;
  }
  throw("Expected only a single field but read %lu fields", num_fields);
  return false;
}

ulong text_file_read_single_ulong(TextFile *this) {
  char *field;

  field = text_file_read_single(this);
  return parse_ulong(field);
}

bool text_file_try_read_single_ulong(TextFile *this, ulong *u) {
  char *field;

  if (!text_file_try_read_single(this, &field)) {
    return false;
  }
  *u = parse_ulong(field);
  return true;
}

void text_file_read_label(TextFile *this, char *label) {
  char *read;

  read = text_file_read_single(this);
  if (!string_equals(read, label)) {
    throw("Expected to read label '%s', but read label '%s'", label, read);
  }
}

char *text_file_read_labeled(TextFile *this, char *label) {
  char *read;
  char **fields;
  ulong num_fields;

  num_fields = text_file_read(this, &fields);
  read = num_fields == 0 ? "" : fields[0];
  if (!string_equals(read, label)) {
    throw("Expected to read label '%s', but read label '%s'", label, read);
  }
  return num_fields < 2 ? "" : fields[1];
}

bool text_file_read_labeled_bool(TextFile *this, char *label) {
  return parse_bool(text_file_read_labeled(this, label));
}

byte text_file_read_labeled_byte(TextFile *this, char *label) {
  return parse_byte(text_file_read_labeled(this, label));
}

ulong text_file_read_labeled_ulong(TextFile *this, char *label) {
  return parse_ulong(text_file_read_labeled(this, label));
}

char *text_file_read_sole(char *filename, char **s) {
  TextFile *tf = null;

  text_file_new(&tf, filename, "r");
  string_new_copy(s, text_file_read_single(tf));
  text_file_delete(&tf);

  return *s;
}

ulong text_file_read_sole_ulong(char *filename) {
  char *s = null;
  ulong u;

  text_file_read_sole(filename, &s);
  u = parse_ulong(s);
  string_delete(&s);

  return u;
}

void text_file_merge_fields(
    TextFile *this,
    ulong first_field_index,
    ulong last_field_index,
    char merged_field_separator) {
  char **fields;
  ulong field_index;

  if (last_field_index >= this->numFields) {
    throw(
        "Last field index %lu invalid: must be less than %lu",
        last_field_index,
        this->numFields);
  }

  if (first_field_index >= last_field_index) {
    throw(
        "First field index %lu not less than last field index %lu",
        first_field_index,
        last_field_index);
  }

  fields = this->fields;

  for (
      field_index = first_field_index + 1;
      field_index <= last_field_index;
      field_index++) {
    *(fields[field_index] - 1) = merged_field_separator;
  }
}

void text_file_rewind(TextFile *this) {
  file_rewind(this->file);
}

static TextFile *construct_(TextFile **pthis) {
  TextFile *this;

  this = newo(pthis);

  new(&this->record, this->maxNumRecordChars = 16);
  new(&this->fields, this->maxNumFields = 2);

  return this;
}

ulong text_file_num_records(TextFile *this) {
  char **fields;
  ulong n;
  ulong num_fields;

  text_file_rewind(this);
  for (n = 0; text_file_try_read(this, &num_fields, &fields); n++) {
  }

  text_file_rewind(this);

  return n;
}

void text_file_copy_remaining(TextFile *src, TextFile *dst) {
  char **fields;
  ulong num_fields;
  ulong u;

  while(text_file_try_read(src, &num_fields, &fields)) {
    for (u = 0; u < num_fields; u++) {
      text_file_write(dst, fields[u]);
    }
    text_file_end_record(dst);
  }
}

bool text_file_contents_equal(TextFile *this, TextFile *other) {
  char **f_this;
  char **f_other;
  ulong n_this;
  ulong n_other;
  ulong u;
  text_file_rewind(this);
  text_file_rewind(other);

  while (text_file_try_read(this, &n_this, &f_this)) {
    if (!text_file_try_read(other, &n_other, &f_other)) {
      return false;
    }
    if (n_other != n_this) {
      return false;
    }
    for (u = 0; u < n_this; u++) {
      if (!string_equals(f_this[u], f_other[u])) {
        return false;
      }
    }
  }
  if (text_file_try_read(other, &n_other, &f_other)) {
    return false;
  }
  return true;
}

char *text_file_get_filename(TextFile *this) {
  return file_get_filename(this->file);
}

static void error_(char *filename, char *mode) {
  byte retries;
  char *s = null;

  if ((retries = text_file_get_retries()) > 1) {
    string_new_f(&s, " even with %d retries", retries);
  } else {
    string_new_empty(&s);
  }
  throw("Could not open file '%s' in mode '%s'%s", filename, mode, s);
}
