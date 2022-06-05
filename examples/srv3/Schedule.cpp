#include <stdint.h>
#include <time.h>
#include <unistd.h>
#include <queue>
#include <vector>

#include "Schedule.h"

Scheduler::Scheduler()
{
}

Scheduler::~Scheduler()
{
}

// 恢复协程的使用
void Scheduler::ResumeCoroutine(Context *coroutine)
{
    current = coroutine;
    swap_context(&ma, current);
}

// 获得时间戳
uint64_t Scheduler::GetMs()
{
    timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return ts.tv_nsec / 1000000 + ts.tv_sec * 1000ull;
}

void Scheduler::CoroutineSleep(int ms)
{
    // 添加新的任务
    uint64_t expire = GetMs() + ms;
    tasks.push(Task{.expire = expire, .coroutine = current});
    // 把执行权让回主协程
    swap_context(current, &ma);
}

void Scheduler::EventLoop(int timeout_in_seconds)
{
    uint64_t start = GetMs();
    while (true)
    {
        // 1000 毫秒检查一次
        usleep(1000);

        while (!tasks.empty())
        {
            // 如果有任务过期
            if (GetMs() > tasks.top().expire)
            {
                Task task = tasks.top();
                tasks.pop();
                // 取出任务并执行
                ResumeCoroutine(task.coroutine);
            }
            else
            {
                break;
            }
        }
        // 超过了时间则退出
        if ((GetMs() - start) > timeout_in_seconds * 1000)
        {
            break;
        }
    }
}
