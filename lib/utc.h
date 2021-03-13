/* Some convenient functions for UTC datetimes.
 */

#ifndef UTC_H
#define UTC_H

#ifdef __cplusplus
extern "C" {
#endif

#include "Lock.h"
#include <sys/time.h>

/* The current UTC unixtime, in microseconds.
 */
long utc_microseconds(void);

/* The current UTC unixtime, in seconds, correct to the microsecond.
 */
double utc_double(void);

/* Create a string representation of the current UTC time.
 *
 *   date_sep: separator between date parts (typical: "-" or "")
 *   t_sep: separator between the date and time (typical: " " or "T")
 *   time_sep: separator between time parts (typical: "-" or "")
 *   decimal_sep: separator between time and microseconds (typical: "." or "")
 *   do_date: include the date
 *   do_time: include the time
 *   do_minutes: include minutes in the time
 *   do_seconds: include seconds in the time
 *   decimal_places: number of decimal places of seconds (max 6)
 */
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
    byte decimal_places);

/* As above, but supplying the timeval rather than taking the current time.
 * Useful for unit tests.
 */
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
    byte decimal_places);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* _H */
