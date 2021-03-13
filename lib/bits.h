/* Bit fiddling routines.
 */
#ifndef BITS_H
#define BITS_H

#ifdef __cplusplus
extern "C" {
#endif

#include "Lock.h"

/* The width, in bits, of unsigned values. Note that the functions are slightly
 * faster as the type being checked gets shorter.
 */
byte ulong_bit_width(ulong u);
byte uint_bit_width(uint u);
byte ushort_bit_width(ushort u);
byte byte_bit_width(byte u);

byte ulong_hexit_width(ulong u);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* _H */
