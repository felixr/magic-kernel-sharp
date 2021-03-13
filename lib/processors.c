#include "processors.h"
#include <math.h>
#include <string.h>

static ulong total_processors_ = 0;

ulong processors_get_total(void) {
  char *out = null;
  char *err = null;
  char *completed = null;
  ulong n;
  TextFile *tf = null;

  if (total_processors_ > 0) {
    return total_processors_;
  }

  /* Works on Linux.
   */
  spawn(
      "cat /proc/cpuinfo | grep processor | wc -l",
      true,
      &out,
      &err,
      &completed);

  if (external_file_is_empty(err)) {
    text_file_new(&tf, out, "r");
    n = text_file_read_single_ulong(tf);
    text_file_delete(&tf);

    external_file_try_remove(completed);
    external_file_try_remove(out);
    external_file_try_remove(err);

    string_delete(&completed);
    string_delete(&out);
    string_delete(&err);

    total_processors_ = n;

    return n;
  }

  /* Linux command failed. Clean up.
   */
  external_file_try_remove(completed);
  external_file_try_remove(out);
  external_file_try_remove(err);

  string_delete(&completed);
  string_delete(&out);
  string_delete(&err);

  /* This works on macOS.
   */
  spawn("sysctl -n hw.ncpu", true, &out, &err, &completed);

  if (external_file_is_empty(err)) {
    text_file_new(&tf, out, "r");
    n = text_file_read_single_ulong(tf);
    text_file_delete(&tf);

    external_file_try_remove(completed);
    external_file_try_remove(out);
    external_file_try_remove(err);

    string_delete(&completed);
    string_delete(&out);
    string_delete(&err);

    total_processors_ = n;

    return n;
  }

  throw("Error in catting /proc/cpuinfo: see '%s'", err);
  return 0;
}

ulong processors_get_free(void) {
  char *out = null;
  char *err = null;
  char *completed = null;
  char *field;
  char *e = null;
  ulong n;
  ulong index;
  ulong total;
  double idle;
  TextFile *tf = null;

  total = processors_get_total();

  if (try_spawn("mpstat 1 1", true, &out, &err, &completed) != 0) {
    external_file_try_remove(completed);
    external_file_try_remove(out);
    external_file_try_remove(err);

    string_delete(&completed);
    string_delete(&out);
    string_delete(&err);

    return processors_get_total();
  }

  if (!external_file_is_empty(err)) {
    throw("Error in mpstat: see '%s'", err);
  }

  text_file_new(&tf, out, "r");

  /* Check that we understand the output.
   */
  string_new_f(&e, "(%lu CPU)", total);
  field = text_file_read_single(tf);
  if (!string_ends_with(field, e)) {
    throw("First line '%s' of mpstat did not end with '%s'", field, e);
  }
  string_delete(&e);

  field = text_file_read_single(tf);
  if (!string_equals(field, "")) {
    throw("Second line '%s' of mpstat was not empty", field);
  }

  field = text_file_read_single(tf);
  if (!string_ends_with(field, "   %idle")) {
    throw("Third line '%s' of mpstat did not end with '  %%idle'", field);
  }

  field = text_file_read_single(tf);
  if (!string_contains_at(field, 13, "all")) {
    throw(
        "Fourth line '%s' of mpstat did not contain 'all' at position 13",
        field);
  }

  index = string_last_index_of(field, ' ');
  idle = parse_double(field + index + 1);

  n = (ulong)round(idle * 0.01 * total);

  text_file_delete(&tf);

  external_file_try_remove(completed);
  external_file_try_remove(out);
  external_file_try_remove(err);

  string_delete(&completed);
  string_delete(&out);
  string_delete(&err);

  return n;
}

ulong processors_get_memory_per(void) {
  char *line;
  char *completed = null;
  char *err = null;
  char *out = null;
  char *sub = null;
  long l;
  ulong total_memory;
  TextFile *tf = null;

  total_memory = 0;

  /* This works on Linux.
   */
  if (text_file_try_new(&tf, "/proc/meminfo", "r")) {
    line = text_file_read_single(tf);
    if (!string_ends_with(line, " kB")) {
      throw("Expected first line '%s' of /proc/meminfo to end in ' kB'", line);
    }
    l = string_nth_last_index_of(line, 1, ' ');
    if (l < 0) {
      throw("Couldn't find two spaces in '%s'", line);
    }
    string_new_substring(&sub, line, l + 1, strlen(line) - (l + 1) - 3);
    total_memory = parse_ulong(sub) * 1024;
    string_delete(&sub);
    text_file_delete(&tf);
  } else {
    /* This works on macOS.
    */
    spawn("sysctl -n hw.memsize", true, &out, &err, &completed);

    if (external_file_is_empty(err)) {
      text_file_new(&tf, out, "r");
      total_memory = text_file_read_single_ulong(tf);
      text_file_delete(&tf);

      external_file_try_remove(completed);
      external_file_try_remove(out);
      external_file_try_remove(err);

      string_delete(&completed);
      string_delete(&out);
      string_delete(&err);
    }
  }

  if (total_memory == 0) {
    throw("Could not determine total memory");
  }

  return total_memory / processors_get_total();
}
