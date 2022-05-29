#ifndef CONDITION_H_
#define CONDITION_H_

#include <pthread.h>

typedef struct condition
{
    pthread_mutex_t pmutex; // mutex
    pthread_cond_t pcond;   // condition
} condition_t;

int condition_init(condition_t *cond);

int condition_lock(condition_t *cond);

int condition_unlock(condition_t *cond);

int condition_wait(condition_t *cond);

int condition_timedwait(condition_t *cond, const timespec *abstime);

int condition_signal(condition_t *cond);

int condition_broadcast(condition_t *cond);

int condition_destroy(condition_t *cond);

#endif