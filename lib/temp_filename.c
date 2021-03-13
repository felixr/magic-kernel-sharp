#include "temp_filename.h"

static Lock lock_ = LOCK_INITIALIZER;
static Random *random_ = null;

char *string_new_temp_filename(char **pthis) {
  char *utc = null;
  ulong random_u;

  lock(&lock_);

  if (random_ == null) {
    random_new(&random_);
  }

  random_u = random_modulo(random_, 1000000);
  unlock(&lock_);

  string_new_utc(&utc, "", "", "", "", true, true, true, true, 6);
  string_new_f(pthis, "/tmp/jpc-%s-%06lu", utc, random_u);
  string_delete(&utc);

  return *pthis;
}

char *string_new_temp_filename_extension(char **pthis, char *extension) {
  string_new_temp_filename(pthis);
  string_renew_concat_f(pthis, ".%s", extension);
  return *pthis;
}
