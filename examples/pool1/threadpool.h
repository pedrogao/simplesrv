#ifndef THREAD_POLL_H_
#define THREAD_POLL_H_

#include "condition.h"

typedef struct task
{
    void *(*run)(void *args); // job
    void *arg;                // arg
    struct task *next;        // next task
} task_t;

typedef struct threadpool
{
    condition_t ready; // ready condition
    task_t *first;     // first elem
    task_t *last;      // last elem
    int counter;       // total
    int idle;          // idle
    int max_threads;   // max
    bool quit;
} threadpool_t;

int threadpool_init(threadpool_t *pool, int threads_num);

int threadpool_add_task(threadpool_t *pool, void *(*run)(void *args), void *arg);

void threadpool_destroy(threadpool_t *pool);

#endif