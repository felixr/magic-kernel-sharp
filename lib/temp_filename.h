/* Create temporary filenames that are effectively guaranteed to be unique
 * across a variety of scenarios: rapid calls to this library; parallel threads
 * within the one program execution; parallel program executions. The filename
 * always starts with "/tmp/jpc-". A UTC date-time to the microsecond is
 * appended to this prefix, followed by a six-digit number pseudo-randomly
 * generated, serially for each call, originally seeded (with a parallel lock)
 * at the time of the first call (so is unlikely to clash between different
 * program executions.
 */
#ifndef TEMP_FILENAME_H
#define TEMP_FILENAME_H

#ifdef __cplusplus
extern "C" {
#endif

#include "Random.h"

char *string_new_temp_filename(char **pthis);
char *string_new_temp_filename_extension(char **pthis, char *extension);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* _H */
