#include "args.h"
#include <ctype.h>
#include <limits.h>

/* For the help output.
 */
#define WIDTH_ 80

/* Indent for wrap of top-level.
 */
#define I0W_ 4

/* Usage, Options, and Arguments indent and wrap.
 */
#define I1I_ 2
#define I1W_ 6

/* Tabstop width For tabs within the description.
 */
#define TAB_ 2

/* Extra indent For wrap of multi-line descriptions.
 */
#define MW_ 2

/* Option descriptions indent, margin, and extra wrap indent.
 */
#define I2I_ 30
#define I2M_ 2
#define I2W_ 0

/* Argument descriptions indent, margin, and extra wrap indent.
 */
#define I3I_ 20
#define I3M_ 2
#define I3W_ 0

/* Options for help.
 */
#define HELP_SHORT_ '?'
#define HELP_LONG_ "help"

typedef enum {
  ARG_ERROR_,
  ARG_FLAG_,
  ARG_STRING_,
  ARG_INTEGER_,
  ARG_REAL_,
  ARG_STRINGS_
} Type_;

typedef struct {
  bool flag;
  bool optional;
  bool set;
  char shrt;
  char *description;
  char *name;
  char *parameter;
  char *string;
  char *stringDefault;
  char **strings;
  ulong numStrings;
  long integer;
  long integerDefault;
  double real;
  double realDefault;
  Type_ type;
} Arg_;

static bool ever_called_ = false;
static bool parsed_ = false;
static char *description_ = null;
static char *program_ = null;
static ulong n_;
static Arg_ *strings_ = null;
static Arg_ **args_ = null;

static void check1_(void);
static void check1a_(void);
static void check2_(void);
static Arg_ *arg_new_(Arg_ **pthis, char *name, char *description, Type_ type);
static void arg_delete(Arg_ **pthis);
static Arg_ *add_pos_(char *name, char *description, Type_ type);
static Arg_ *add_opt_(char *name, char *description, Type_ type, char shrt);
static Arg_ *find_shrt_(char shrt);
static Arg_ *find_name_(char *name);
static void print_help_(void);
static ulong next_pos_a_(ulong pos_a);
static bool valid_short_(char shrt);
static bool valid_command_short_(char shrt);
static char *parse_value_(char *p, Arg_ *arg, bool take_all);
static char *find_unquoted_space_or_null_(char *s, bool take_all);
static void do_description_(char **pthis, Arg_ *arg, bool *wrapped);

void args_description(char *description) {
  string_new_copy(&description_, description);
  new(&args_, n_ = 0);
  args_flag(HELP_LONG_, HELP_SHORT_, "Print this help page and exit");
  ever_called_ = true;
}

void args_string(char *name, char *fmt, ...) {
  char *description = null;
  va_list ap1;
  va_list ap2;

  check1a_();
  va_start(ap1, fmt);
  va_start(ap2, fmt);
  string_new_vf(&description, fmt, ap1, ap2);
  va_end(ap1);
  va_end(ap2);
  add_pos_(name, description, ARG_STRING_);
  string_delete(&description);
}

void args_integer(char *name, char *fmt, ...) {
  char *description = null;
  va_list ap1;
  va_list ap2;

  check1a_();
  va_start(ap1, fmt);
  va_start(ap2, fmt);
  string_new_vf(&description, fmt, ap1, ap2);
  va_end(ap1);
  va_end(ap2);
  add_pos_(name, description, ARG_INTEGER_);
  string_delete(&description);
}

void args_real(char *name, char *fmt, ...) {
  char *description = null;
  va_list ap1;
  va_list ap2;

  check1a_();
  va_start(ap1, fmt);
  va_start(ap2, fmt);
  string_new_vf(&description, fmt, ap1, ap2);
  va_end(ap1);
  va_end(ap2);
  add_pos_(name, description, ARG_REAL_);
  string_delete(&description);
}

void args_strings(char *name, char *fmt, ...) {
  char *description = null;
  va_list ap1;
  va_list ap2;

  check1a_();
  va_start(ap1, fmt);
  va_start(ap2, fmt);
  string_new_vf(&description, fmt, ap1, ap2);
  va_end(ap1);
  va_end(ap2);
  add_pos_(name, description, ARG_STRINGS_);
  string_delete(&description);
}

void args_flag(char *name, char shrt, char *fmt, ...) {
  char *description = null;
  va_list ap1;
  va_list ap2;

  va_start(ap1, fmt);
  va_start(ap2, fmt);
  string_new_vf(&description, fmt, ap1, ap2);
  va_end(ap1);
  va_end(ap2);
  add_opt_(name, description, ARG_FLAG_, shrt);
  string_delete(&description);
}

void args_optional_string(char *name, char shrt, char *dflt, char *fmt, ...) {
  char *description = null;
  va_list ap1;
  va_list ap2;
  Arg_ *arg;

  va_start(ap1, fmt);
  va_start(ap2, fmt);
  string_new_vf(&description, fmt, ap1, ap2);
  va_end(ap1);
  va_end(ap2);
  arg = add_opt_(name, description, ARG_STRING_, shrt);
  string_delete(&description);

  if (dflt != null) {
    string_new_copy(&arg->stringDefault, dflt);
    string_new_copy(&arg->string, dflt);
  }
}

void args_optional_integer(char *name, char shrt, long dflt, char *fmt, ...) {
  char *description = null;
  va_list ap1;
  va_list ap2;
  Arg_ *arg;

  va_start(ap1, fmt);
  va_start(ap2, fmt);
  string_new_vf(&description, fmt, ap1, ap2);
  va_end(ap1);
  va_end(ap2);
  arg = add_opt_(name, description, ARG_INTEGER_, shrt);
  string_delete(&description);
  arg->integerDefault = dflt;
  arg->integer = dflt;
}

void args_optional_real( char *name, char shrt, double dflt, char *fmt, ...) {
  char *description = null;
  va_list ap1;
  va_list ap2;
  Arg_ *arg;

  va_start(ap1, fmt);
  va_start(ap2, fmt);
  string_new_vf(&description, fmt, ap1, ap2);
  va_end(ap1);
  va_end(ap2);
  arg = add_opt_(name, description, ARG_REAL_, shrt);
  string_delete(&description);
  arg->realDefault = dflt;
  arg->real = dflt;
}

static ulong next_pos_a_(ulong pos_a) {
  Arg_ *arg;

  if (pos_a == ULONG_MAX) {
    pos_a = 0;
  } else {
    pos_a++;
  }
  for (; pos_a < n_; pos_a++) {
    if (!(arg = args_[pos_a])->optional && arg->type != ARG_STRINGS_) {
      return pos_a;
    }
  }
  return pos_a;
}

void args_parse(int argc, char *argv[]) {
  bool flag_run;
  bool finished_options;
  bool waiting_for_value;
  char c;
  char *clarg;
  char *p;
  char *q;
  char *r;
  char *t;
  char *z;
  char *error = null;
  char *name = null;
  ulong a;
  ulong cla;
  ulong k;
  ulong m;
  ulong pos_a;
  Arg_ *arg;

  check1_();

  for (a = 0; a < n_; a++) {
    arg = args_[a];
    if (arg->type == ARG_ERROR_) {
      throw("impossible");
    }
  }

  string_new_filename_from_path(&program_, argv[0]);
  parsed_ = true;

  if (n_ > 1 && argc == 1) {
    print_help_();
    args_cleanup();
    exit_normally();
  }

  pos_a = next_pos_a_(ULONG_MAX);

  finished_options = false;
  waiting_for_value = false;

  for (cla = 1; cla < argc; cla++) {
    clarg = argv[cla];
    flag_run = false;
    for (p = clarg; *p != '\0';) {
      if (waiting_for_value) {
        p = parse_value_(p, arg, true);
        waiting_for_value = false;
        continue;
      }

      for (z = p; *p == ' '; p++) {
        flag_run = false;
      }

      if (flag_run || (!finished_options && *p == '-')) {
        if (!flag_run) {
          p++;
        }

        c = *p;

        if (c == '\0' || c == ' ') {
          if (pos_a == n_) {
            if (strings_ == null) {
              throw("Specified stdin value '-' but no positional "
                  "arguments left");
            }

            p = parse_value_("-", strings_, true);
          } else {
            arg = args_[pos_a];

            if (arg->type != ARG_STRING_) {
              throw(
                  "Supplied stdin value '-' for non-string argument <%s>",
                  arg->parameter);
            }

            string_renew_copy(&args_[pos_a]->string, "-");
            arg->set = true;
            pos_a = next_pos_a_(pos_a);
          }
        } else if (c == '-') {
          if (flag_run) {
            throw("Invalid '-' after flag option");
          }

          if ((c = *++p) == '\0' || c == ' ') {
            finished_options = true;

            if (c == ' ') {
              p++;
            }
          } else {
            if (!isalpha(c)) {
              throw("Invalid long option name in argument '%s'", clarg);
            }

            for (q = p; (c = *q) != '\0' && c != ' ' && c != '='; q++) {
              if (!isalnum(c) && c != '-') {
                throw("Invalid long option name in argument '%s'", clarg);
              }
            }

            new(&name, q - p + 1);

            for (r = p, t = name; r < q; r++, t++) {
              *t = *r;
            }

            *t = '\0';

            if (string_equals(name, HELP_LONG_)) {
              print_help_();
              args_cleanup();
              exit_normally();
            }

            arg = find_name_(name);

            if (arg == null) {
              throw("Invalid option '--%s'", name);
            }

            if (arg->set) {
              throw("Repeated option '-%c', '--%s'", arg->shrt, arg->name);
            }

            arg->set = true;
            string_delete(&name);

            if (arg->type == ARG_FLAG_) {
              arg->flag = true;
              p = q;

              if (c != '\0') {
                p++;
              }

              for (p = q; *p == ' '; p++) {
              }

              if (*p == '\0') {
                break;
              }
            } else {
              p = q;

              if (c != '\0') {
                p++;
              }

              for (; *p == ' '; p++) {
              }

              if (*p == '\0') {
                waiting_for_value = true;
                continue;
              }
              p = parse_value_(p, arg, false);
            }
          }
        } else if (c == HELP_SHORT_) {
          print_help_();
          args_cleanup();
          exit(0);
        } else {
          if (!valid_command_short_(c)) {
            throw("Invalid short option name '-%c'", c);
          }

          arg = find_shrt_(c);

          if (arg == null) {
            throw("Invalid option '-%c'", c);
          }

          if (arg->set) {
            throw("Repeated option '-%c', '--%s'", c, arg->name);
          }

          arg->set = true;

          if (arg->type == ARG_FLAG_) {
            arg->flag = true;
            flag_run = true;

            if (*++p == '\0') {
              break;
            }
          } else {
            flag_run = false;

            for (p++; *p == ' '; p++) {
            }

            if (*p == '\0') {
              waiting_for_value = true;
              continue;
            }

            p = parse_value_(p, arg, false);
          }
        }
      } else {
        p = z;

        if (pos_a == n_) {
          if (strings_ == null) {
            throw(
                "Positional value '%s' supplied but no positional arguments "
                "left",
                p);
          }

          p = parse_value_(p, strings_, true);
        } else {
          arg = args_[pos_a];
          p = parse_value_(p, arg, true);
          pos_a = next_pos_a_(pos_a);
        }
      }
    }
  }

  if (pos_a != n_) {
    for (m = 1, a = next_pos_a_(pos_a); a != n_; m++, a = next_pos_a_(a)) {
    }

    string_new_f(
        &error,
        "No value%s supplied for the positional argument%s",
        plural_s(m),
        plural_s(m));

    for (k = 0; pos_a != n_; k++, pos_a = next_pos_a_(pos_a)) {
      string_renew_concat_f(
          &error,
          "%s <%s>",
          k > 0 ? (m == 2 ? " and" : (k == m - 1 ? ", and" : ",")) : "",
          args_[pos_a]->parameter);
    }
    throw(error);
  }
}

bool args_set(char *name) {
  Arg_ *arg;

  check2_();
  arg = find_name_(name);

  if (arg == null) {
    throw("No argument named '%s'", name);
  }

  if (!arg->optional) {
    throw("Can't call args_set() on the positional argument --%s", name);
  }

  return arg->set;
}

bool args_set_veto(char *name, char *vetoed) {
  if (!args_set(name)) {
    return false;
  }

  if (args_set(vetoed)) {
    throw("Cannot specify both --%s and --%s", name, vetoed);
  }

  return true;
}

char *args_get_string(char *name) {
  Arg_ *arg;

  check2_();
  arg = find_name_(name);

  if (arg == null) {
    throw("No argument named '%s'", name);
  }

  if (arg->type != ARG_STRING_) {
    throw(
        "Argument '%s' is not a string",
        arg->optional ? arg->name : arg->parameter);
  }

  return arg->string;
}

long args_get_integer(char *name) {
  Arg_ *arg;

  check2_();
  arg = find_name_(name);

  if (arg == null) {
    throw("No argument named '%s'", name);
  }

  if (arg->type != ARG_INTEGER_) {
    throw(
        "Argument '%s' is not an integer",
        arg->optional ? arg->name : arg->parameter);
  }

  return arg->integer;
}

ulong args_get_positive_integer(char *name) {
  long l;
  Arg_ *arg;

  l = args_get_integer(name);

  if (l <= 0) {
    arg = find_name_(name);
    throw(
        "Argument %s must be positive, but the value %ld was specified",
        arg->parameter,
        l);
  }

  return (ulong)l;
}

ulong args_get_nonnegative_integer(char *name) {
  long l;
  Arg_ *arg;

  l = args_get_integer(name);

  if (l < 0) {
    arg = find_name_(name);
    throw(
        "Argument %s must be nonnegative, but the value %ld was specified",
        arg->parameter,
        l);
  }

  return (ulong)l;
}

long args_get_bounded_integer(char *name, long min, long max) {
  long l;
  Arg_ *arg;

  if (max < min) {
    throw("Maximum %ld less than minimum %ld", max, min);
  }

  l = args_get_integer(name);

  if (l < min || l > max) {
    arg = find_name_(name);
    throw(
        "Argument %s must be in the range [%ld, %ld], but the value %ld "
        "was specified",
        arg->parameter,
        min,
        max,
        l);
  }

  return l;
}

double args_get_real(char *name) {
  Arg_ *arg;

  check2_();
  arg = find_name_(name);

  if (arg == null) {
    throw("No argument named '%s'", name);
  }

  if (arg->type != ARG_REAL_) {
    throw(
        "Argument '%s' is not a real",
        arg->optional ? arg->name : arg->parameter);
  }

  return arg->real;
}

double args_get_positive_real(char *name) {
  double d;
  Arg_ *arg;

  d = args_get_real(name);

  if (d <= 0) {
    arg = find_name_(name);
    throw(
        "Argument %s must be positive, but the value %g was specified",
        arg->parameter,
        d);
  }
  return d;
}

double args_get_nonnegative_real(char *name) {
  double d;
  Arg_ *arg;

  d = args_get_real(name);

  if (d < 0) {
    arg = find_name_(name);
    throw(
        "Argument %s must be nonnegative, but the value %g was specified",
        arg->parameter,
        d);
  }

  return d;
}

double args_get_bounded_real(char *name, double mn, double mx) {
  double d;
  Arg_ *arg;

  if (mx < mn) {
    throw("Maximum %g less than minimum %g", mx, mn);
  }

  d = args_get_real(name);

  if (d < mn || d > mx) {
    arg = find_name_(name);
    throw(
        "Argument %s must be in the range [%g, %g], but the value %g "
        "was specified",
        arg->parameter,
        mn,
        mx,
        d);
  }
  return d;
}

bool args_get_flag(char *name) {
  Arg_ *arg;

  check2_();
  arg = find_name_(name);

  if (arg == null) {
    throw("No argument named '%s'", name);
  }

  if (arg->type != ARG_FLAG_) {
    throw(
        "Argument '%s' is not a flag",
        arg->optional ? arg->name : arg->parameter);
  }

  return arg->flag;
}

ulong args_get_strings(char ***pstrings) {
  if (strings_ == null) {
    throw("No strings argument specified");
  }

  *pstrings = strings_->strings;

  return strings_->numStrings;
}

char *args_get_string_no_std(char *name) {
  char *string;
  Arg_ *arg;

  string = args_get_string(name);

  if (string_equals(string, "-")) {
    arg = find_name_(name);
    throw("Argument %s cannot be '-'", arg->parameter);
  }

  return string;
}

ulong args_get_strings_no_std(char ***pstrings) {
  char **strings;
  ulong num_strings;
  ulong u;

  num_strings = args_get_strings(pstrings);
  strings = *pstrings;

  for (u = 0; u < num_strings; u++) {
    if (string_equals(strings[u], "-")) {
      throw("Argument %s cannot contain the string '-'", strings_->parameter);
    }
  }

  return num_strings;
}

bool args_get_flag_veto(char *name, char *vetoed) {
  if (!args_get_flag(name)) {
    return false;
  }

  if (args_set(vetoed)) {
    throw("Cannot specify both --%s and --%s", name, vetoed);
  }

  return true;
}

void args_cleanup(void) {
  if (description_ == null) {
    if (ever_called_) {
      throw("args_cleanup() has already been called");
    } else {
      throw("args_description() was never called");
    }
  }

  string_delete(&description_);
  string_delete(&program_);
  delete(&args_, n_, arg_delete);
  parsed_ = false;
  strings_ = null;
}

bool args_try_cleanup(void) {
  if (description_ == null) {
    return false;
  }

  args_cleanup();
  return true;
}

void args_program(char *program) {
  string_renew_filename_from_path(&program_, program);
}

char *args_help(char **pthis) {
  bool pos;
  bool opt;
  bool wrapped;
  char *s = null;
  ulong a;
  ulong len;
  Type_ type;
  Arg_ *arg;

  if (program_ == null) {
    throw("Must call args_parse() or args_program() before args_help()");
  }

  string_new_f_wrap(
      pthis,
      0,
      WIDTH_,
      I0W_,
      "%s: %s",
      program_,
      description_);
  string_renew_concat_f(pthis, "\n\nUsage:\n%*s", I1I_, "");

  string_new_empty(&s);

  for (a = 0, pos = false, opt = false; a < n_; a++) {
    arg = args_[a];

    if (arg->optional) {
      opt = true;
    } else {
      if (arg->type == ARG_STRINGS_) {
        string_renew_concat_f(&s, " [<%s...>]", arg->parameter);
      } else {
        string_renew_concat_f(&s, " <%s>", arg->parameter);
      }

      pos = true;
    }
  }

  string_renew_concat_f_wrap(
      pthis,
      I1I_,
      WIDTH_,
      I1I_ + I1W_,
      "%s%s%s",
      program_,
      opt ? " [options]" : "",
      s);
  string_delete(&s);
  string_renew_concat(pthis, "\n");

  wrapped = false;

  if (opt) {
    string_renew_concat(pthis, "\nOptions:\n");

    for (a = 0; a < n_; a++) {
      arg = args_[a];

      if (arg->optional) {
        string_renew_concat_f(pthis, "%*s", I1I_, "");
        string_new_empty(&s);

        if (arg->shrt != ' ') {
          string_renew_concat_f(&s, "-%c", arg->shrt);

          if (arg->type != ARG_FLAG_) {
            string_renew_concat_f(&s, " %s", arg->parameter);
          }

          string_renew_concat(&s, ", ");
        }
        string_renew_concat_f(&s, "--%s", arg->name);

        if ((type = arg->type) != ARG_FLAG_) {
          string_renew_concat_f(&s, "=%s", arg->parameter);
        }

        string_renew_concat_wrap(pthis, s, I1I_, WIDTH_, I1I_ + I1W_);
        len = I1I_ + string_length(s);
        string_delete(&s);

        if (len > I2I_ - I2M_) {
          string_renew_concat(pthis, "\n");
          wrapped = true;
        } else {
          string_renew_concat_f(pthis, "%*s", I2I_ - len, "");
          wrapped = false;
        }

        do_description_(pthis, arg, &wrapped);
      }
    }
  }

  if (pos) {
    if (!wrapped) {
      string_renew_concat(pthis, "\n");
    }

    string_renew_concat(pthis, "Arguments:\n");

    for (a = 0; a < n_; a++) {
      arg = args_[a];

      if (!arg->optional) {
        string_renew_concat_f(pthis, "%*s", I1I_, "");

        if (arg->type == ARG_STRINGS_) {
          string_renew_concat_f_wrap(
              pthis,
              I1I_,
              WIDTH_,
              I1I_ + I1W_,
              "[%s...]",
              arg->parameter);
          len = I1I_ + string_length(arg->parameter) + 5;
        } else {
          string_renew_concat_wrap(
              pthis,
              arg->parameter,
              I1I_,
              WIDTH_,
              I1I_ + I1W_);
          len = I1I_ + string_length(arg->parameter);
        }

        if (len > I3I_ - I3M_) {
          string_renew_concat(pthis, "\n");
          wrapped = true;
        } else {
          string_renew_concat_f(pthis, "%*s", I3I_ - len, "");
          wrapped = false;
        }

        do_description_(pthis, arg, &wrapped);
      }
    }
  }

  return *pthis;
}

static void check1_(void) {
  if (description_ == null) {
    throw("args_description() not called");
  }
  if (parsed_) {
    throw("args_parse() already called");
  }
}

static void check1a_(void) {
  if (strings_ != null) {
    throw("Cannot call after args_strings() called");
  }
}

static void check2_(void) {
  if (!parsed_) {
    throw("args_parse() has not been called");
  }

  if (args_ == null) {
    throw("args_cleanup() has already been called");
  }
}

static Arg_ *arg_new_(Arg_ **pthis, char *name, char *description, Type_ type) {
  char c;
  char *p;
  Arg_ *this;

  check1_();

  if ((c = *name) == '\0') {
    throw("Name cannot be empty");
  }

  if (!isalpha(c)) {
    throw("Invalid name '%s': must start with alpha", name);
  }

  if (*description == '\0') {
    throw("Description cannot be empty");
  }

  for (p = name; (c = *p) != '\0'; p++) {
    if (c != '-' && !isalnum(c)) {
      throw(
          "Invalid name '%s': can only include alphanumeric or '-'",
          name);
    }
  }

  if (find_name_(name) != null) {
    throw("Repeated name '%s'", name);
  }

  this = newo(pthis);

  string_new_copy(&this->name, name);
  string_new_copy(&this->parameter, name);

  for (p = this->parameter; *p != '\0'; p++) {
    if ((c = *p) == '-') {
      *p = '_';
    } else {
      *p = toupper(c);
    }
  }

  if (type == ARG_FLAG_) {
    string_new_copy(&this->description, description);
  } else {
    string_new_replace_char_with_string(
        &this->description,
        description,
        '$',
        this->parameter);
  }

  this->type = type;
  new(&this->strings, 0);

  return this;
}

static void arg_delete(Arg_ **pthis) {
  Arg_ *this;

  if ((this = *pthis) == null) {
    return;
  }

  string_delete(&this->name);
  string_delete(&this->description);
  string_delete(&this->parameter);
  string_delete(&this->string);
  string_delete(&this->stringDefault);
  delete(&this->strings, this->numStrings, string_delete);

  deleteo(pthis);
}

static Arg_ *add_pos_(char *name, char *description, Type_ type) {
  Arg_ *arg;

  check1_();

  renewup(&args_, n_, n_ + 1);
  arg = arg_new_(&args_[n_], name, description, type);
  n_++;

  if (type == ARG_STRINGS_) {
    strings_ = arg;
  }

  return arg;
}

static bool valid_short_(char shrt) {
  return valid_command_short_(shrt) || shrt == ' ';
}

static bool valid_command_short_(char shrt) {
  return isalnum(shrt) || shrt == HELP_SHORT_;
}

static Arg_ *add_opt_(char *name, char *description, Type_ type, char shrt) {
  Arg_ *arg;

  check1_();

  if (!valid_short_(shrt)) {
    throw(
        "Invalid short option '%c': must be alphanumeric, or space for none",
        shrt);
  }

  if (shrt != ' ' && find_shrt_(shrt) != null) {
    throw("Repeated short option '-%c'", shrt);
  }

  arg = add_pos_(name, description, type);
  arg->optional = true;
  arg->shrt = shrt;

  return arg;
}

Arg_ *find_shrt_(char shrt) {
  ulong a;
  Arg_ *arg;

  for (a = 0; a < n_; a++) {
    arg = args_[a];

    if (arg->shrt == shrt) {
      return arg;
    }
  }

  return null;
}

Arg_ *find_name_(char *name) {
  ulong a;
  Arg_ *arg;

  for (a = 0; a < n_; a++) {
    arg = args_[a];

    if (string_equals(arg->name, name)) {
      return arg;
    }
  }

  return null;
}

static void print_help_(void) {
  char *s = null;

  args_help(&s);
  printf("%s", s);
  string_delete(&s);
}

static char *parse_value_(char *p, Arg_ *arg, bool take_all) {
  char *q;
  char *s = null;

  string_new_copy(&s, p);
  q = find_unquoted_space_or_null_(s, take_all);
  *q = '\0';

  for (p += q - s; *p == ' '; p++) {
  }

  switch (arg->type) {
    case ARG_STRING_:
      string_renew_copy(&arg->string, s);
      break;
    case ARG_INTEGER_:
      if (!try_parse_long(&arg->integer, s)) {
        throw(
            "Could not parse '%s' as an integer for argument <%s>",
            s,
            arg->parameter);
      }
      break;
    case ARG_REAL_:
      if (!try_parse_double(&arg->real, s)) {
        throw(
            "Could not parse '%s' as a real for argument <%s>",
            s,
            arg->parameter);
      }
      break;
    case ARG_STRINGS_:
      renewup(&arg->strings, arg->numStrings, arg->numStrings + 1);
      string_new_copy(&arg->strings[arg->numStrings], s);
      arg->numStrings++;
      break;
    case ARG_ERROR_:
      throw("Error: impossible");
    default:
      throw("Bad type %d", arg->type);
  }

  string_delete(&s);
  arg->set = true;
  return p;
}

static char *find_unquoted_space_or_null_(char *s, bool take_all) {
  bool in_single_quotes = false;
  bool in_double_quotes = false;
  char c;
  char *q;

  for (q = s; (c = *q) != '\0'; q++) {
    if (!take_all&& !in_single_quotes && !in_double_quotes && c == ' ') {
      return q;
    }

    if (c == '\'') {
      in_single_quotes = !in_single_quotes;
    }

    if (c == '"') {
      in_double_quotes = !in_double_quotes;
    }
  }
  return q;
}

static void do_description_(char **pthis, Arg_ *arg, bool *wrapped) {
  bool opt;
  byte wrap;
  char c;
  char *def = null;
  char *l;
  char *p;
  char *s = null;
  char **lines = null;
  ulong indent;
  ulong line;
  ulong num_lines;
  ulong tabs;
  Type_ type;

  opt = arg->optional;
  type = arg->type;

  num_lines = string_split(arg->description, '\n', &lines);

  if (opt) {
    if (type != ARG_FLAG_) {
      switch (type) {
        case ARG_STRING_:
          if (arg->stringDefault != ARGS_STRING_NO_DEFAULT) {
            string_new_f(&def, "(default\f=\f'%s')", arg->stringDefault);
          }
          break;
        case ARG_INTEGER_:
          if (arg->integerDefault != ARGS_INTEGER_NO_DEFAULT) {
            string_new_f(&def, "(default\f=\f%ld)", arg->integerDefault);
          }
          break;
        case ARG_REAL_:
          if (arg->realDefault != ARGS_REAL_NO_DEFAULT) {
            string_new_f(&def, "(default\f=\f%g)", arg->realDefault);
          }
          break;
        default:
          throw("impossible %d", type);
      }

      if (def != null) {
        if (num_lines > 1) {
          renewup(&lines, num_lines, num_lines + 1);
          string_new_copy(&lines[num_lines], def);
          num_lines++;
        } else {
          string_renew_concat_until_null(&lines[0], " ", def, null);
        }

        string_delete(&def);
      }
    }
  }

  string_new_empty(&s);

  for (line = 0; line < num_lines; line++) {
    for (p = lines[line], tabs = 0; *p == '\t'; p++) {
      tabs++;
    }

    l = p;

    if (tabs != 0 && line == 0) {
      throw("Can't have tabs on the first line");
    }

    for (; (c = *p) != '\0'; p++) {
      if (c == '\t') {
        throw("Can't have tabs in the middle of a line");
      }
    }

    indent = (opt ? I2I_ : I3I_) + tabs * TAB_;
    wrap = indent + (opt ? I2W_ : I3W_) + (num_lines > 1 ? MW_ : 0);

    if (line != 0) {
      *wrapped = true;
    }

    if (*wrapped) {
      string_renew_concat_f(&s, "%*s", indent, "");
    }

    if (string_renew_concat_wrap(&s, l, indent, WIDTH_, wrap)) {
      *wrapped = true;
    }

    string_renew_concat(&s, "\n");
  }

  string_replace_chars(s, "\f", ' ');
  string_renew_concat(pthis, s);
  string_delete(&s);

  delete(&lines, num_lines, string_delete);
}
