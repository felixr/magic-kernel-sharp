/* Simple tools to figure out how many processors we might have available.
 */
#ifndef PROCESSORS_H
#define PROCESSORS_H

#ifdef __cplusplus
extern "C" {
#endif

#include "TextFile.h"
#include "numbers.h"
#include "spawn.h"

/* Number of processors in total.
 */
ulong processors_get_total(void);

/* Total amount of memory per processor, in bytes.
 */
ulong processors_get_memory_per(void);

/* An estimate of the number of processors free, based on one-second
 * utilization.
 */
ulong processors_get_free(void);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* _H */
