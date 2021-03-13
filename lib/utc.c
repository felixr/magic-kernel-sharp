#include "utc.h"
#include <time.h>

long utc_microseconds(void) {
  struct timeval tv;

  gettimeofday(&tv, 0);
  return (long)tv.tv_sec * 1000000 + tv.tv_usec;
}

double utc_double(void) {
  return utc_microseconds() * 0.000001;
}

char *string_new_utc(
    char **pthis,
    char *date_sep,
    char *t_sep,
    char *time_sep,
    char *decimal_sep,
    bool do_date,
    bool do_time,
    bool do_minutes,
    bool do_seconds,
    byte decimal_places) {
  struct timeval tv;

  gettimeofday(&tv, 0);
  return string_new_utc_from_timeval(
      pthis,
      &tv,
      date_sep,
      t_sep,
      time_sep,
      decimal_sep,
      do_date,
      do_time,
      do_minutes,
      do_seconds,
      decimal_places);
}

char *string_new_utc_from_timeval(
    char **pthis,
    struct timeval *tv,
    char *date_sep,
    char *t_sep,
    char *time_sep,
    char *decimal_sep,
    bool do_date,
    bool do_time,
    bool do_minutes,
    bool do_seconds,
    byte decimal_places) {
  static bool init__ = false;
  static Lock lock__;

  byte d;
  uint f;
  struct tm *t;

  if (decimal_places > 6) {
    throw("Decimal places (%u) cannot be more than 6", decimal_places);
  }

  if (!init__) {
    lock_init(&lock__);
    init__ = true;
  }

  lock(&lock__);
  t = gmtime(&tv->tv_sec);

  string_new_empty(pthis);

  if (do_date) {
    if (date_sep == null) {
      throw("Cannot have null date_sep when date specified");
    }
    string_renew_concat_f(
        pthis,
        "%04d%s%02d%s%02d",
        1900 + t->tm_year,
        date_sep,
        1 + t->tm_mon,
        date_sep,
        t->tm_mday);

    if (do_time) {
      string_renew_concat_f(pthis, "%s", t_sep);
    }
  }

  if (do_time) {
    string_renew_concat_f(pthis, "%02d", t->tm_hour);

    if (do_minutes) {
      if (time_sep == null) {
        throw("Cannot have null time_sep when minutes specified");
      }
      string_renew_concat_f(pthis, "%s%02d", time_sep, t->tm_min);

      if (do_seconds) {
        string_renew_concat_f(pthis, "%s%02d", time_sep, t->tm_sec);

        if (decimal_places != 0) {
          if (decimal_sep == null) {
            throw("Cannot have null decimal_sep with decimal places");
          }
          for (d = 0, f = 1; d < 6 - decimal_places; d++, f *= 10) {
          }
          string_renew_concat_f(
              pthis,
              "%s%0*d",
              decimal_sep,
              decimal_places,
              ((int)tv->tv_usec + f / 2) / f);
        }
      } else if (decimal_places != 0) {
        unlock(&lock__);
        throw("Can't specify decimal places without seconds");
      }
    } else if (do_seconds || decimal_places != 0) {
      unlock(&lock__);
      throw("Can't specify seconds or decimal places without minutes");
    }
  } else if (do_minutes || do_seconds || decimal_places != 0) {
    unlock(&lock__);
    throw("Can't specify minutes, seconds or decimal places without time");
  }

  unlock(&lock__);

  return *pthis;
}
