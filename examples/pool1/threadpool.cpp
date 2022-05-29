#include <pthread.h>
#include <cstdlib>
#include <cstdio>
#include <cerrno>
#include <ctime>
#include "threadpool.h"

// thread runtine function
void *thread_routine(void *arg);

int threadpool_init(threadpool_t *pool, int threads_num)
{
    int status = condition_init(&pool->ready);
    if (status == 0)
    {
        printf("Info: initialize the thread pool successfully!\n");
    }
    else
    {
        printf("Error: initialize the thread pool failed, status: %d\n", status);
        return status;
    }

    pool->first = NULL;
    pool->last = NULL;
    pool->counter = 0;
    pool->idle = 0;
    pool->max_threads = threads_num;
    pool->quit = false;

    return 0;
}

int threadpool_add_task(threadpool_t *pool, void *(*run)(void *args), void *arg)
{
    task_t *task = reinterpret_cast<task_t *>(malloc(sizeof(task_t)));
    task->arg = arg;
    task->run = run;
    task->next = NULL;

    // lock
    condition_lock(&pool->ready);
    // add to queue
    if (pool->first == NULL)
    {
        pool->first = pool->last = task;
    }
    else
    {
        // first -> xxxx -> last
        // first -> xxxx -> last -> task
        //                   ↡       ↡
        // first -> xxxx -> xxxx -> last
        pool->last->next = task;
        pool->last = task;
    }
    // if some threads are idle
    if (pool->idle > 0)
    {
        condition_signal(&pool->ready);
    }
    else if (pool->counter < pool->max_threads)
    {
        // add thread
        pthread_t tid;
        pthread_create(&tid, NULL, thread_routine, pool);
        pool->counter++;
    }
    else
    {
        printf("Warning: no idle thread, please wait...\n");
        return -1;
    }
    condition_unlock(&pool->ready);
    return 0;
}

void threadpool_destroy(threadpool_t *pool)
{
    if (pool->quit)
    {
        return;
    }

    condition_lock(&pool->ready);
    pool->quit = true;
    // wait treads done
    if (pool->counter > 0)
    {
        if (pool->idle > 0)
        {
            condition_broadcast(&pool->ready);
        }
        while (pool->counter > 0)
        {
            condition_wait(&pool->ready);
        }

        condition_unlock(&pool->ready);
    }
}

void *thread_routine(void *arg)
{
    struct timespec tick;
    bool timeout;
    printf("Info: create thread, and the thread id is: %llu\n", (u_int64_t)pthread_self());
    threadpool_t *pool = reinterpret_cast<threadpool_t *>(arg);

    while (true)
    {
        timeout = false;
        condition_lock(&pool->ready); // lock
        pool->idle++;

        // 等2s处理任务
        while (pool->first == NULL && !pool->quit)
        {
            printf("Info: thread %llu is waiting for a task\n", (u_int64_t)pthread_self());
            // get the system time
            clock_gettime(CLOCK_REALTIME, &tick);
            tick.tv_sec += 2;
            int status;
            status = condition_timedwait(&pool->ready, &tick); // block for 2 second
            if (status == ETIMEDOUT)
            {
                printf("Info: thread %llu wait timed out\n", (u_int64_t)pthread_self());
                timeout = true;
                break;
            }
        }

        pool->idle--;
        // 有任务了
        if (pool->first != NULL)
        {
            // first -> xxxx
            //           ↡
            //         first
            task_t *t = pool->first;
            pool->first = t->next;
            condition_unlock(&pool->ready);

            // run the task run func
            t->run(t->arg);
            free(t);

            // lock
            condition_lock(&pool->ready);
        }

        // when task queue is clean and quit flag is 1, then destroy the thread
        if (pool->quit && pool->first == NULL)
        {
            pool->counter--;
            // 若线程池中线程数为0，通知等待线程（主线程）全部任务已经完成
            if (pool->counter == 0)
            {
                condition_signal(&pool->ready);
            }
            condition_unlock(&pool->ready);
            break;
        }

        // if visit task queue timeout(means no task in queue), quit destory the thread
        if (timeout)
        {
            pool->counter--;
            condition_unlock(&pool->ready);
            break;
        }

        condition_unlock(&pool->ready);
    }

    printf("Info: thread %llu quit\n", (u_int64_t)pthread_self());
    return NULL;
}