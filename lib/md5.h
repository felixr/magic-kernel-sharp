/* MD5 hashes.
 */
#ifndef MD5_H
#define MD5_H

#ifdef __cplusplus
extern "C" {
#endif

#include "jpc.h"

char *string_new_md5(char **pthis, ulong len, byte *blob);
void md5_ulongs(ulong len, byte *blob, ulong *hi, ulong *lo);
void md5_bytes(ulong len, byte *blob, byte *md5);

void md5_bytes_to_ulongs(byte *md5, ulong *hi, ulong *lo);
char *string_new_md5_from_ulongs(char **pthis, ulong hi, ulong lo);
char *string_new_md5_from_bytes(char **pthis, byte *md5);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* _H */
