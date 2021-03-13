/* Simple wrapper around mutex locking for pthreads.
 */
#ifndef LOCK_H
#define LOCK_H

#ifdef __cplusplus
extern "C" {
#endif

#include "jpc.h"
#include <pthread.h>

typedef pthread_mutex_t Lock;

#define LOCK_INITIALIZER PTHREAD_MUTEX_INITIALIZER

void lock_init(Lock *lock);
void lock(Lock *lock);
void unlock(Lock *lock);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* _H */
