#define LOG_C
#include "log.h"
#include <stdarg.h>

bool g_LOG_timestamps_ = true;
bool g_LOG_on_ = true;
bool g_LOG_fl_on_ = true;
char *g_LOG_file_ = null;
int g_LOG_line_ = -1;

static bool d_;
static Lock lock_ = LOCK_INITIALIZER;

static void v_(FILE *f, char *fmt, va_list ap1, va_list ap2);

void flog(FILE *f, char *fmt, ...) {
  va_list ap1;
  va_list ap2;

  d_ = false;
  va_start(ap1, fmt);
  va_start(ap2, fmt);
  v_(f, fmt, ap1, ap2);
  va_end(ap1);
  va_end(ap2);
}

void olog(char *fmt, ...) {
  va_list ap1;
  va_list ap2;

  d_ = false;
  va_start(ap1, fmt);
  va_start(ap2, fmt);
  v_(stdout, fmt, ap1, ap2);
  va_end(ap1);
  va_end(ap2);
}

void elog(char *fmt, ...) {
  va_list ap1;
  va_list ap2;

  d_ = false;
  va_start(ap1, fmt);
  va_start(ap2, fmt);
  v_(stderr, fmt, ap1, ap2);
  va_end(ap1);
  va_end(ap2);
}

void dlogI(char *fmt, ...) {
  va_list ap1;
  va_list ap2;

  if (!g_LOG_on_) {
    return;
  }

  d_ = true;
  va_start(ap1, fmt);
  va_start(ap2, fmt);
  v_(stderr, fmt, ap1, ap2);
  va_end(ap1);
  va_end(ap2);
}

bool log_timestamps(bool b){
  bool current;

  current = g_LOG_timestamps_;
  g_LOG_timestamps_ = b;
  return current;
}

static void v_(FILE *f, char *fmt, va_list ap1, va_list ap2) {
  bool space = false;
  char *s = null;
  char *utc = null;

  if (f == null) {
    return;
  }

  setbuf(f, null);

  if (g_LOG_timestamps_) {
    string_new_utc(&utc, null, null, ":", ".", false, true, true, true, 3);
    string_new_f(&s, "[%sZ]", utc);
    string_delete(&utc);
    space = true;
  } else {
    string_new_empty(&s);
  }

  if (d_ && g_LOG_fl_on_ && !string_equals(g_LOG_file_, "lib/jpc.c")) {
    string_renew_concat_f(&s, "[%s:%d]", g_LOG_file_, g_LOG_line_);
    space = true;
  }

  if (space) {
    string_renew_concat(&s, " ");
  }

  lock_log();
  fprintf(f, "%s", s);
  string_delete(&s);
  string_new_vf(&s, fmt, ap1, ap2);
  string_renew_commaize(&s);
  fprintf(f, "%s", s);
  string_delete(&s);
  fprintf(f, "\n");
  unlock_log();
}

void lock_log(void) {
  lock(&lock_);
}

void unlock_log(void) {
  unlock(&lock_);
}
