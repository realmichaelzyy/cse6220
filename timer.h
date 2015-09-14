#ifndef _STOPWATCH_H_
#define _STOPWATCH_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <sys/time.h>

typedef struct timeval* stopwatch_t;

stopwatch_t stopwatch_start();

long double stopwatch_stop(stopwatch_t);

#ifdef __cplusplus
}
#endif

#endif
