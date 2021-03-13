/* See README and jpc.h for documentation.
*/
#define JPC_C
#include "jpc.h"
#include <math.h>
#include <stdarg.h>
#include <string.h>

#define INDENT_ 2
#define MAX_INDENT_ 100

#define throw_ \
  g_JPC_internal_throw_ = true, \
  g_JPC_file_2_ = g_JPC_file_1_, \
  g_JPC_line_2_ = g_JPC_line_1_, \
  g_JPC_file_1_ = file, \
  g_JPC_line_1_ = line, \
  throwI

bool g_JPC_internal_throw_ = false;
bool g_JPC_trace_ = false;
byte g_JPC_try_ = 0;
char *exception = null;
char *g_JPC_file_1_ = null;
char *g_JPC_file_2_ = null;
char *g_JPC_orig_exception_ = null;
int g_JPC_line_1_ = -1;
int g_JPC_line_2_ = -1;
jmp_buf g_JPC_jmp_buf_;

static bool allow_exit_normally_ = true;

void exit_normally() {
  if (allow_exit_normally_) {
    exit(0);
  }
  throw("exit_normally() was called, but this has been disallowed");
}

void allow_exit_normally(bool allow) {
  allow_exit_normally_ = allow;
}

void throwI(char *fmt, ...) {
  char *space = null;
  va_list ap1;
  va_list ap2;

  if (g_JPC_try_ == 2 && !g_JPC_internal_throw_) {
    string_renew_copy(&g_JPC_orig_exception_, exception);
  }

  string_delete(&exception);
  va_start(ap1, fmt);
  va_start(ap2, fmt);
  string_new_vf(&exception, fmt, ap1, ap2);
  va_end(ap1);
  va_end(ap2);

  string_renew_commaize(&exception);

  if (g_JPC_try_ == 1) {
    g_JPC_try_ = 2;
    longjmp(g_JPC_jmp_buf_, 1);
  }

  string_new_copy(&space, string_ends_with(exception, "\n") ? "" : " ");
  fprintf(
      stderr,
      "\nERROR: %s%s(%s:%d)\n\n",
      exception,
      space,
      g_JPC_file_1_,
      g_JPC_line_1_);
  string_delete(&space);

  if (g_JPC_try_ == 2) {
    string_new_copy(
        &space,
        string_ends_with(g_JPC_orig_exception_, "\n") ? "" : " ");
    fprintf(
        stderr,
        "RETHROWN AFTER: %s%s(%s:%d)\n\n",
        g_JPC_orig_exception_,
        space,
        g_JPC_file_2_,
        g_JPC_line_2_);
    string_delete(&space);
  }

#ifdef JPC_DEBUG
  /* Write to address zero to leverage ASAN's printing of the stack trace.
   */
  *(volatile char *)null = '\0';
#else
  exit(1);
#endif
}

void *newI(void **pthis, size_t bytes, char *file, int line) {
  if (pthis == null) {
    throw_("Pointer to pointer null in new() or newo()");
  }
  if (*pthis != null) {
    throw_("Pointer not empty in new() or newo()");
  }

  if ((*pthis = calloc(bytes > 0 ? bytes : 1, 1)) == null) {
    throw_("Out of memory allocating %lu bytes in new() or newo()", bytes);
  }

  return *pthis;
}

void deleteoI(
    void **pthis,
    char *file,
    int line) {
  void *this;

  if (pthis == null) {
    throw_("Pointer to pointer null in delete()");
  }

  if ((this = *pthis) == null) {
    return;
  }

  free(this);
  *pthis = null;
}

void deleteI(
    void **pthis,
    size_t n,
    void (*destructor)(void **pthis),
    char *file,
    int line) {
  void *this;
  void **p;
  void **e;

  if (destructor == null) {
    throw_("Destructor null in delete()");
  }

  if (pthis == null) {
    throw_("Pointer to pointer null in delete()");
  }

  if ((this = *pthis) == null) {
    return;
  }

  for (p = (void **)this, e = p + n; p < e; p++) {
    if (*p != null) {
      destructor(p);
    }
  }

  deletev(pthis);
}

void deletevcI(
    void **pthis,
    size_t n,
    size_t size,
    void (*cleanup)(void *this),
    char *file,
    int line) {
  byte *p;
  byte *e;
  void *this;

  if (pthis == null) {
    throw_("Pointer to pointer null in deletevc()");
  }

  if ((this = *pthis) == null) {
    return;
  }

  if (cleanup != null) {
    for (p = this, e = p + n * size; p < e; p += size) {
      cleanup(p);
    }
  }

  deletev(pthis);
}

void *renewI(
    void **pthis,
    size_t old_n,
    size_t new_n,
    size_t size,
    void (*destructor)(void **pthis),
    char *file,
    int line) {
  void *this;
  void **p;
  void **e;
  ulong new_bytes;
  ulong old_bytes;

  if (pthis == null) {
    throw_("Pointer to pointer null in renew()");
  }
  if ((this = *pthis) == null) {
    throw_("Pointer empty in renew()");
  }

  if (new_n == old_n) {
    return this;
  }

  if (new_n < old_n && destructor != null) {
    for (p = (void **)this + new_n, e = p + old_n; p < e; p++) {
      if (*p != null) {
        destructor(p);
      }
    }
  }

  new_bytes = new_n * size;

  if ((this = *pthis = realloc(this, new_bytes > 0 ? new_bytes : 1)) == null) {
    throw_(
        "Out of memory reallocating %lu -> %lu bytes in renew()",
        old_n * size,
        new_bytes);
  }

  if (new_n > old_n) {
    old_bytes = old_n * size;
    memset((byte *)this + old_bytes, 0, new_bytes - old_bytes);
  }

  return this;
}

void *renewupI(
    void **pthis,
    size_t old_n,
    size_t new_n,
    size_t size,
    char *file,
    int line) {
  if (new_n < old_n) {
    throw_("Decrease from %lu to %lu in renewup()", old_n, new_n);
  }
  return renewI(pthis, old_n, new_n, size, null, file, line);
}

void *donateI(void **psrc, void **pdst, char *file, int line) {
  if (psrc == null) {
    throw_("Pointer to source pointer null in donate()");
  }
  if (pdst == null) {
    throw_("Pointer to destination pointer null in donate()");
  }
  if (*psrc == null) {
    throw_("Source pointer empty in donate()");
  }
  if (*pdst != null) {
    throw_("Destination pointer not empty in donate()");
  }

  *pdst = *psrc;
  *psrc = null;
  return *pdst;
}

void value_array_delete(void **pthis) {
  deletev(pthis);
}

void *newmI(void ***pthis, size_t m, size_t bytes, char *file, int line) {
  void **this;
  void **p;
  void **e;

  this = newI((void **)pthis, m * sizeof(void *), file, line);
  for (p = this, e = p + m; p < e; p++) {
    newI(p, bytes, file, line);
  }
  return *pthis;
}

void deletemI(
    void ***pthis,
    size_t m,
    size_t n,
    void (*destructor)(void **pthis),
    char *file,
    int line) {
  void **this;
  void **p;
  void **e;

  if (pthis == null) {
    throw_("Pointer to pointer null in delete()");
  }

  if ((this = *pthis) == null) {
    return;
  }

  for (p = this, e = p + m; p < e; p++) {
    deleteI(p, n, destructor, file, line);
  }

  free(this);
  *pthis = null;
}

void deletemvI(
    void ***pthis,
    size_t m,
    char *file,
    int line) {
  void **this;
  void **p;
  void **e;

  if (pthis == null) {
    throw_("Pointer to pointer null in delete()");
  }

  if ((this = *pthis) == null) {
    return;
  }

  for (p = this, e = p + m; p < e; p++) {
    deleteoI(p, file, line);
  }

  free(this);
  *pthis = null;
}

void *newt3I(
    void ****pthis,
    size_t n0,
    size_t n1,
    size_t bytes,
    char *file,
    int line) {
  void ***this;
  void ***p;
  void ***e;

  this = newI((void **)pthis, n0 * sizeof(void *), file, line);
  for (p = this, e = p + n0; p < e; p++) {
    newmI(p, n1, bytes, file, line);
  }
  return *pthis;
}

void deletet3I(
    void ****pthis,
    size_t n0,
    size_t n1,
    size_t n2,
    void (*destructor)(void **pthis),
    char *file,
    int line) {
  void ***this;
  void ***p;
  void ***e;

  if (pthis == null) {
    throw_("Pointer to pointer null in delete()");
  }

  if ((this = *pthis) == null) {
    return;
  }

  for (p = this, e = p + n0; p < e; p++) {
    deletemI(p, n1, n2, destructor, file, line);
  }

  free(this);
  *pthis = null;
}

void deletet3vI(
    void ****pthis,
    size_t n0,
    size_t n1,
    char *file,
    int line) {
  void ***this;
  void ***p;
  void ***e;

  if (pthis == null) {
    throw_("Pointer to pointer null in delete()");
  }

  if ((this = *pthis) == null) {
    return;
  }

  for (p = this, e = p + n0; p < e; p++) {
    deletemvI(p, n1, file, line);
  }

  free(this);
  *pthis = null;
}

void *newt4I(
    void *****pthis,
    size_t n0,
    size_t n1,
    size_t n2,
    size_t bytes,
    char *file,
    int line) {
  void ****this;
  void ****p;
  void ****e;

  this = newI((void **)pthis, n0 * sizeof(void *), file, line);
  for (p = this, e = p + n0; p < e; p++) {
    newt3I(p, n1, n2, bytes, file, line);
  }
  return *pthis;
}

void deletet4I(
    void *****pthis,
    size_t n0,
    size_t n1,
    size_t n2,
    size_t n3,
    void (*destructor)(void **pthis),
    char *file,
    int line) {
  void ****this;
  void ****p;
  void ****e;

  if (pthis == null) {
    throw_("Pointer to pointer null in delete()");
  }

  if ((this = *pthis) == null) {
    return;
  }

  for (p = this, e = p + n0; p < e; p++) {
    deletet3I(p, n1, n2, n3, destructor, file, line);
  }

  free(this);
  *pthis = null;
}

void deletet4vI(
    void *****pthis,
    size_t n0,
    size_t n1,
    size_t n2,
    char *file,
    int line) {
  void ****this;
  void ****p;
  void ****e;

  if (pthis == null) {
    throw_("Pointer to pointer null in delete()");
  }

  if ((this = *pthis) == null) {
    return;
  }

  for (p = this, e = p + n0; p < e; p++) {
    deletet3vI(p, n1, n2, file, line);
  }

  free(this);
  *pthis = null;
}

void fprintfn(FILE *f, byte n, char *fmt, ...) {
  char *s1 = null;
  char *s2 = null;
  va_list ap1;
  va_list ap2;

  if (n > MAX_INDENT_) {
    throw(
        "Aborting print: indent level got to %u, which is crazy, and "
        "probably broken",
        n);
  }

  va_start(ap1, fmt);
  va_start(ap2, fmt);
  string_new_vf(&s1, fmt, ap1, ap2);
  va_end(ap1);
  va_end(ap2);

  string_new_f(&s2, "%*s%s", n * INDENT_, "", s1);
  string_delete(&s1);

  string_renew_commaize(&s2);

  if (f == null) {
    dlog(s2);
  } else {
    fprintf(f, "%s\n", s2);
  }

  string_delete(&s2);
}

double round(double x) {
  return x < 0.0 ? -floor(-x + 0.5) : floor(x + 0.5);
}

void defrag_pointer_arrayI(void ***pthis, ulong *pn) {
  void **a;
  void **b;
  void **e;
  void **this;
  ulong n;

  this = *pthis;
  n = *pn;

  for (a = this, b = a, e = b + n; b < e; b++) {
    if (*b != null) {
      *a++ = *b;
    }
  }
  n = a - this;
  renew(pthis, *pn, n, null);
  *pn = n;
}

void set_global_trace(bool b) {
  g_JPC_trace_ = b;
}

bool get_global_trace(void) {
  return g_JPC_trace_;
}
