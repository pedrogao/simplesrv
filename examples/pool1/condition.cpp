#include "condition.h"
#include <cstdio>

int condition_init(condition_t *cond)
{
    int status;
    status = pthread_mutex_init(&cond->pmutex, NULL);
    if (status != 0)
    {
        printf("Error: pthread mutex init failed, return value: %d\n", status);
        return status;
    }
    status = pthread_cond_init(&cond->pcond, NULL);
    if (status != 0)
    {
        printf("Error: pthread condition init failed, return value: %d\n", status);
        return status;
    }
    return 0;
}

int condition_lock(condition_t *cond)
{
    return pthread_mutex_lock(&cond->pmutex);
}

int condition_unlock(condition_t *cond)
{
    return pthread_mutex_unlock(&cond->pmutex);
}

int condition_wait(condition_t *cond)
{
    return pthread_cond_wait(&cond->pcond, &cond->pmutex);
}

int condition_timedwait(condition_t *cond, const timespec *abstime)
{
    return pthread_cond_timedwait(&cond->pcond, &cond->pmutex, abstime);
}

int condition_signal(condition_t *cond)
{
    return pthread_cond_signal(&cond->pcond);
}

int condition_broadcast(condition_t *cond)
{
    return pthread_cond_broadcast(&cond->pcond);
}

int condition_destroy(condition_t *cond)
{
    int status;
    status = pthread_mutex_destroy(&cond->pmutex);
    if (status != 0)
    {
        return status;
    }

    status = pthread_cond_destroy(&cond->pcond);
    if (status != 0)
    {
        return status;
    }
    return 0;
}