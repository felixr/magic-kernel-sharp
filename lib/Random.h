#ifndef RANDOM_H
#define RANDOM_H

#ifdef __cplusplus
extern "C" {
#endif

#include "utc.h"

typedef struct {
  uint *m;
  int i;
  ulong lastModulus;
  ulong lastLimit;
} Random;

/* Constructor seeded with the current microsecond.
 */
Random *random_new(Random **pthis);

/* Constructor with the given seed.
 */
Random *random_new_seed(Random **pthis, uint seed);

/* Constructor seeded with the current microsecond plus the given offset times
 * a random jump. Useful when more than one random number generator may be
 * created in the same microsecond.
 */
Random *random_new_offset(Random **pthis, uint offset);

/* Destructor.
 */
void random_delete(Random **pthis);

/* Random ulong in [0, modulus).
 */
ulong random_modulo(Random *this, ulong modulus);

/* Random double in [0, 1]. ('cc' = 'closed, closed').
 */
double random_double_cc(Random *this);

/* Random double in [0, 1). ('co' = 'closed, open').
 */
double random_double_co(Random *this);

/* Random double in (0, 1]. ('oc' = 'open, closed').
 */
double random_double_oc(Random *this);

/* Random double in (0, 1). ('oo' = 'open, open').
 */
double random_double_oo(Random *this);

/* Random boolean, equally likely to be true or false.
 */
bool random_bool(Random *this);

/* Random uint in the complete range [0, 0xffffffff].
 */
uint random_uint(Random *this);

/* Random ulong in the complete range [0, 0xffffffffffffffff].
 */
ulong random_ulong(Random *this);

/* Random long in the complete range [-0x8000000000000000, 0x7fffffffffffffff].
 */
long random_long(Random *this);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* _H */
