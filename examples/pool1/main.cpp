#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include "threadpool.h"
#include <pthread.h>

#define THREADPOOL_MAX_NUM 30

void *mytask(void *arg)
{
    printf("Info: thread %llu is working on task %d\n", (u_int64_t)pthread_self(), *reinterpret_cast<int *>(arg));
    sleep(1);
    free(arg);
    return NULL;
}

int main(int argc, char *argv[])
{
    threadpool_t pool;
    threadpool_init(&pool, THREADPOOL_MAX_NUM);

    // add task to task queue
    for (int i = 0; i < 100; i++)
    {
        int *arg = reinterpret_cast<int *>(malloc(sizeof(int)));
        *arg = i;
        threadpool_add_task(&pool, mytask, arg);
    }
    threadpool_destroy(&pool);
    return 0;
}