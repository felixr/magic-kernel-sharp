/* Handle command-line arguments in the standard fashion.
 */
#ifndef ARGS_H
#define ARGS_H

#ifdef __cplusplus
extern "C" {
#endif

#include "jpc.h"
#include "numbers.h"
#include <limits.h>
#include <float.h>

/* Specify the description for the program. This must be called first.
 */
void args_description(char *description);

/* Specify a positional argument. 'name' must start with alpha, and then can
 * only contain alphanumeric or '-'. The parameter name in the help screen is
 * 'name' converted to uppercase and '-' replaced by '_'. Any '$' in
 * 'description' is replaced by the parameter name. Any form-feed character
 * ('\f') is interpreted as an unbreakable space. Newlines ('\n') are allowed,
 * and are handled appropriately. One or more tabs ('\t') may appear after any
 * newline character, and indent the given line by the given number of tab
 * stops.
 */
void args_string(char *name, char *fmt, ...);
void args_integer(char *name, char *fmt, ...);
void args_real(char *name, char *fmt, ...);

/* Specify that after all of the positional arguments specified by the
 * functions above (if any), there can be zero to any number of string
 * positional arguments. No regular positional argument can be specified after
 * this is called.
 */
void args_strings(char *name, char *fmt, ...);

/* Use these to specify that an optional argument does not have a default.
 */
#define ARGS_STRING_NO_DEFAULT (null)
#define ARGS_INTEGER_NO_DEFAULT (LONG_MAX)
#define ARGS_REAL_NO_DEFAULT (DBL_MAX)

/* Specify an optional argument. 'shrt' is the short (single-character) name
 * for the option, and must be alphanumeric, or ' ' if you don't want a short
 * name. Except for flags, 'dflt' specifies the default value of the option.
 */
void args_flag(char *name, char shrt, char *fmt, ...);
void args_optional_string(char *name, char shrt, char *dflt, char *fmt, ...);
void args_optional_integer(char *name, char shrt, long dflt, char *fmt, ...);
void args_optional_real( char *name, char shrt, double dflt, char *fmt, ...);

/* Needed if args_help() is to be called before args_parse().
 */
void args_program(char *program);

/* Write the args help screen into a string. Can only be done after either
 * args_parse() or args_program() has been called.
 */
char *args_help(char **pthis);

/* Parse the command line arguments. None of the above functions can be called
 * after this has been called.
 */
void args_parse(int argc, char *argv[]);

/* Whether a given argument was set. Only makes sense for optional arguments.
 */
bool args_set(char *name);

/* Whether a given argument was set, specifying that, if so, the given other
 * argument cannot also be set.
 */
bool args_set_veto(char *name, char *vetoed);

/* Get the value of the given argument. Can only be called after the command
 * line arguments have been parsed. Note that string pointers are references
 * only: no new string (or array of strings) is allocated.
 */
char *args_get_string(char *name);
long args_get_integer(char *name);
double args_get_real(char *name);
bool args_get_flag(char *name);
ulong args_get_strings(char ***pstrings);

/* Get the value of a flag, specifying that its being set vetoes the given
 * other optional argument from being set.
 */
bool args_get_flag_veto(char *name, char *vetoed);

/* Use these to specify that a range is open at one end.
 */
#define ARGS_INTEGER_NO_MIN (LONG_MIN)
#define ARGS_INTEGER_NO_MAX (LONG_MAX)
#define ARGS_REAL_NO_MIN (-DBL_MAX)
#define ARGS_REAL_NO_MAX (DBL_MAX)

/* Get the value with additional constraints. Effectively looks as if the
 * constraints were checked when the arguments were parsed.
 */
ulong args_get_positive_integer(char *name);
ulong args_get_nonnegative_integer(char *name);
long args_get_bounded_integer(char *name, long min, long max);
double args_get_positive_real(char *name);
double args_get_nonnegative_real(char *name);
double args_get_bounded_real(char *name, double min, double max);
char *args_get_string_no_std(char *name);
ulong args_get_strings_no_std(char ***pstrings);

/* Clean up. None of the above can be called after this point, and, to guard
 * against simple errors, this function cannot be called twice.
 */
void args_cleanup(void);

/* Try to clean up; return false if nothing to do. Should only be used in unit
 * tests.
 */
bool args_try_cleanup();

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* _H */
