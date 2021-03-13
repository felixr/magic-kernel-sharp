#include "TextFileTest.h"
#include <ctype.h>

typedef struct {
  ulong num_fields;
  char **fields;
} Record_;

static void record_delete_(Record_ **pthis);

START

TEST(TextFile, Comprehensive) {
  byte field_length_width;
  byte num_fields_width;
  byte num_records_width;
  char c;
  char *p;
  char *pe;
  char **fields;
  int i;
  int text_file_mode;
  ulong f;
  ulong field_length;
  ulong num_fields;
  ulong num_records;
  ulong u;
  TextFile *tf = null;
  Random *random = null;
  Record_ *record;
  Record_ **records = null;

  random_new(&random);

  for (i = 0; i < 20; i++) {
    text_file_mode = random_modulo(random, 4);
    text_file_new_csv(&tf, null, null);
    num_records_width = random_modulo(random, 18);
    num_records = random_modulo(random, 1 << num_records_width);
    new(&records, num_records);

    for (u = 0; u < num_records; u++) {
      record = (Record_ *)newo(&records[u]);
      num_fields_width = random_modulo(random, 9);
      record->num_fields = random_modulo(random, 1 << num_fields_width);
      new(&record->fields, record->num_fields);

      for (f = 0; f < record->num_fields; f++) {
        field_length_width = random_modulo(random, 5);
        field_length = random_modulo(random, 1 << field_length_width);
        new(&record->fields[f], field_length + 1);

        for (p = record->fields[f], pe = p + field_length; p < pe; p++) {
          do {
            c = random_modulo(random, 127);
          } while (c == ',' || !isprint(c));
          *p = c;
        }
        *p = '\0';
        text_file_write(tf, record->fields[f]);
      }
      switch (text_file_mode) {
        case 0:
          text_file_end_record(tf);
          break;
        case 1:
          file_write_byte(tf->file, '\n');
          tf->currentFieldIndex = 0;
          break;
        case 2:
          file_write_byte(tf->file, '\r');
          tf->currentFieldIndex = 0;
          break;
        default:
          file_write_byte(tf->file, '\r');
          file_write_byte(tf->file, '\n');
          tf->currentFieldIndex = 0;
          break;
      }
      if (record->num_fields == 1 && record->fields[0][0] == '\0') {
        record->num_fields = 0;
        deletev(&record->fields[0]);
      }
    }
    text_file_rewind(tf);

    for (u = 0; u < num_records; u++) {
      record = records[u];
      num_fields = text_file_read(tf, &fields);
      EXPECT_ULEQ(record->num_fields, num_fields);
      for (f = 0; f < num_fields; f++) {
        EXPECT_STREQ(record->fields[f], fields[f]);
      }
    }
    delete(&records, num_records, record_delete_);
    text_file_delete(&tf);
  }
  random_delete(&random);
}

FINISH

static void record_delete_(Record_ **pthis) {
  Record_ *this;

  this = *pthis;
  delete(&this->fields, this->num_fields, value_array_delete);
  deleteo(pthis);
}
