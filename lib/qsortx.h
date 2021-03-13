/* qsort() and bsearch() with a pass-through object and auto sizeof.
 */
#ifndef QSORTX_H
#define QSORTX_H

#ifdef __cplusplus
extern "C" {
#endif

#include "jpc.h"

/* There's a quarter-century of mess there trying to get a pass-through object
 * into qsort() and bsearch(). It isn't in C89, so add it here. Also, as with
 * new() and renew(), these versions also figure out the size of each element
 * for you, avoiding boilerplate error-prone sizeof() copypasta.
 */
#define qsortx(base, n, cmp, x) \
  qsortxI((base), (n), sizeof(*(base)), (cmp), (x))
#define bsearchx(key, base, n, cmp, x) \
  bsearchxI((key), (base), (n), sizeof(*(base)), (cmp), (x))
void qsortxI(
    void *base,
    ulong n,
    ulong size,
    int (*cmp)(void *a, void *b, void *x),
    void *x);
void *bsearchxI(
    void *key,
    void *base,
    ulong n,
    ulong size,
    int (*cmp)(void *a, void *b, void *x),
    void *x);

/* These variants allow you to specify the width, in bytes, rather than using
 * the width of the type in the array. Useful when the array is an array of
 * bytes, and your actual objects are wider than one byte.
 */
#define qsortxn(base, n, width, cmp, x) \
  qsortxI((base), (n), (width), (cmp), (x));
#define bsearchxn(key, base, n, width, cmp, x) \
  bsearchxI((key), (base), (n), (width), (cmp), (x));

/* Find the index of an element.
 */
#define bsearchx_find_index(key, base, n, cmp, x) \
  bsearchx_find_indexI((key), (base), (n), sizeof(*(base)), (cmp), (x))
#define bsearchx_try_find_index(key, base, n, cmp, x, pindex) \
  bsearchx_try_find_indexI( \
      (key), \
      (base), \
      (n), \
      sizeof(*(base)), \
      (cmp), \
      (x), \
      (pindex))
ulong bsearchx_find_indexI(
    void *key,
    void *base,
    ulong n,
    ulong size,
    int (*cmp)(void *a, void *b, void *x),
    void *x);
bool bsearchx_try_find_indexI(
    void *key,
    void *base,
    ulong n,
    ulong size,
    int (*cmp)(void *a, void *b, void *x),
    void *x,
    ulong *pindex);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* _H */
