#include <unistd.h>
#include "ThreadPool.h"

void mytask(int i)
{
    printf("Info: thread %llu is working on task %d\n", (u_int64_t)pthread_self(), i);
    sleep(1);
    return;
}

int main()
{
    ThreadPool threadpool(20);
    for (int i = 0; i < 100; ++i)
    {
        threadpool.enqueue(mytask, i);
    }
    return 0;
}