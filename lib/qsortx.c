#include "qsortx.h"

#define MAX_THRESH_ 4
#define STACK_SIZE_ 64

#define PUSH_(M_l_, M_h_) top->lo = (M_l_); top->hi = (M_h_); top++;
#define POP_(M_l_, M_h_) top--; M_l_ = top->lo; M_h_ = top->hi;
#define min_(x, y) ((x) < (y) ? (x) : (y))
#define SWAP_(a, b) { \
  byte M_tmp_; \
  byte *M_a_; \
  byte *M_b_; \
  ulong M_size_; \
  \
  M_size_ = size; \
  M_a_ = (a); \
  M_b_ = (b); \
  do { \
    M_tmp_ = *M_a_; \
    *M_a_++ = *M_b_; \
    *M_b_++ = M_tmp_; \
  } while (--M_size_ > 0); \
}

typedef struct {
  byte *lo;
  byte *hi;
} Node_;

void qsortxI(
    void *base,
    ulong n,
    ulong size,
    int (*cmp)(void *a, void *b, void *x),
    void *x) {
  byte c;
  byte *b;
  byte *lo;
  byte *hi;
  byte *l;
  byte *m;
  byte *r;
  byte *e;
  byte *t;
  byte *thresh;
  byte *run;
  byte *trav;
  ulong max_thresh;
  Node_ *top;

  b = (byte *)base;

  max_thresh = MAX_THRESH_ * size;

  if (n < 2) {
    return;
  }

  if (n > MAX_THRESH_) {
    Node_ stack[STACK_SIZE_];

    lo = b;
    hi = &lo[size * (n - 1)];
    top = stack;

    PUSH_(null, null);

    while (stack < top) {
      m = lo + size * ((hi - lo) / size >> 1);

      if (cmp(m, lo, x) < 0) {
        SWAP_(m, lo);
      }
      if (cmp(hi, m, x) < 0) {
        SWAP_(m, hi);
        if (cmp(m, lo, x) < 0) {
          SWAP_(m, lo);
        }
      }

      l  = lo + size;
      r = hi - size;

      do {
        while (cmp(l, m, x) < 0) {
          l += size;
        }
        while (cmp(m, r, x) < 0) {
          r -= size;
        }

        if (l < r) {
          SWAP_(l, r);
          if (m == l) {
            m = r;
          }
          else if (m == r) {
            m = l;
          }
          l += size;
          r -= size;
        }
        else if (l == r) {
          l += size;
          r -= size;
          break;
        }
      }
      while (l <= r);

      if (r - lo <= max_thresh) {
        if (hi - l <= max_thresh) {
          POP_(lo, hi);
        } else {
          lo = l;
        }
      }
      else if (hi - l <= max_thresh) {
        hi = r;
      } else if (r - lo > hi - l) {
        PUSH_(lo, r);
        lo = l;
      } else {
        PUSH_(l, hi);
        hi = r;
      }
    }
  }

  e = &b[size * (n - 1)];
  t = b;
  thresh = min_(e, b + max_thresh);

  for (run = t + size; run <= thresh; run += size) {
    if (cmp(run, t, x) < 0) {
      t = run;
    }
  }

  if (t != b) {
    SWAP_(t, b);
  }

  run = b + size;
  while ((run += size) <= e) {
    t = run - size;
    while (cmp(run, t, x) < 0) {
      t -= size;
    }

    t += size;
    if (t != run) {
      trav = run + size;
      while (--trav >= run) {
        c = *trav;
        for (hi = lo = trav; (lo -= size) >= t; hi = lo) {
          *hi = *lo;
        }
        *hi = c;
      }
    }
  }
}

void *bsearchxI(
    void *key,
    void *base,
    ulong n,
    ulong size,
    int (*cmp)(void *a, void *b, void *x),
    void *x) {
  byte *b;
  byte *p;
  int c;

  for (b = base; n != 0; n >>= 1) {
    p = b + (n >> 1) * size;
    c = cmp(key, p, x);
    if (c == 0) {
      return p;
    }
    if (c > 0) {
      b = p + size;
      n--;
    }
  }
  return null;
}

ulong bsearchx_find_indexI(
    void *key,
    void *base,
    ulong n,
    ulong size,
    int (*cmp)(void *a, void *b, void *x),
    void *x) {
  ulong index = 0;

  if (!bsearchx_try_find_indexI(key, base, n, size, cmp, x, &index)) {
    throw("Could not find element");
  }
  return index;
}

bool bsearchx_try_find_indexI(
    void *key,
    void *base,
    ulong n,
    ulong size,
    int (*cmp)(void *a, void *b, void *x),
    void *x,
    ulong *pindex) {
  void *p;

  p = bsearchxI(key, base, n, size, cmp, x);
  if (p == null) {
    return false;
  }
  *pindex = ((byte *)p - (byte *)base) / size;
  return true;
}
