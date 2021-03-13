/* Simple logging. Prepends with UTC timestamp. Mutexed to be thread-safe.
 */
#ifndef LOG_H
#define LOG_H

#ifdef __cplusplus
extern "C" {
#endif

#include "Lock.h"
#include "utc.h"

/* Log to an ANSI FILE.
 */
void flog(FILE *f, char *fmt, ...);

/* Log to stdout.
 */
void olog(char *fmt, ...);

/* Log to stderr.
 */
void elog(char *fmt, ...);

/* Log for debugging.
 */
#define dlog \
  g_LOG_file_ = __FILE__, \
  g_LOG_line_ = __LINE__, \
  dlogI
void dlogI(char *fmt, ...);
#define DLOG_ON g_LOG_on_ = true;
#define DLOG_OFF g_LOG_on_ = false;
#define DLOG_FILE_LINE_ON g_LOG_fl_on_ = true;
#define DLOG_FILE_LINE_OFF g_LOG_fl_on_ = false;

/* Whether to include timestamps. Defaults to true. Returns the previous value.
 */
bool log_timestamps(bool b);

/* Lock and unlock for logging. Be cautious if you use these yourself.
 */
void lock_log(void);
void unlock_log(void);

#ifndef LOG_C
extern bool g_LOG_on_;
extern bool g_LOG_fl_on_;
extern char *g_LOG_file_;
extern int g_LOG_line_;
#endif /* iLOG_C */

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* _H */
