#include <stdint.h>
#include <time.h>
#include <unistd.h>
#include <iostream>
#include <queue>
#include <vector>

#define STACK_SIZE 1024

struct Context
{
    long rsp = 0; // 栈指针
    long rbp = 0; // 帧地址
    // callee saved
    long rbx = 0;
    long r12 = 0;
    long r13 = 0;
    long r14 = 0;
    long r15 = 0;
    // call stack
    long *stack;
    Context(void (*entry)() = nullptr)
    {
        // 16 字节对齐
        long *stack = (long *)malloc(STACK_SIZE / sizeof(long));
        this->stack = (long *)((long)stack & (long)~15);
        long *base = &this->stack[STACK_SIZE / sizeof(long) - 1]; // 栈顶
        if (entry != nullptr)
        {
            *(--base) = (long)entry;
        }
        this->rsp = (long)base;
    }

    ~Context()
    {
        free(stack);
    }
} ma;

extern "C" void swap_context(Context *, Context *);

// current coroutine context
Context *current = nullptr;

// 恢复协程的使用
void resume_coroutine(Context *coroutine)
{
    current = coroutine;
    swap_context(&ma, current);
}

// 获得时间戳
uint64_t GetMs()
{
    timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return ts.tv_nsec / 1000000 + ts.tv_sec * 1000ull;
}

struct Task
{
    uint64_t expire;                                                          // 过期时间
    Context *coroutine;                                                       // 当前任务上的协程
    bool operator<(const Task &other) const { return expire > other.expire; } // 优先队列将 expire 最小的放到最前面
};

std::priority_queue<Task> tasks;

void coroutine_sleep(int ms)
{
    // 添加新的任务
    uint64_t expire = GetMs() + ms;
    tasks.push(Task{.expire = expire, .coroutine = current});
    // 把执行权让回主协程
    swap_context(current, &ma);
}

void event_loop(int timeout_in_seconds)
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
                resume_coroutine(task.coroutine);
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

void func1()
{
    while (true)
    {
        std::cout << "Coroutine 1 print per 500ms" << std::endl;
        coroutine_sleep(500);
    }
}

void func2()
{
    while (true)
    {
        std::cout << "Coroutine 2 print per 1000ms" << std::endl;
        coroutine_sleep(1000);
    }
}

int main(int argc, char const *argv[])
{
    Context co1(func1);
    resume_coroutine(&co1);

    Context co2(func2);
    resume_coroutine(&co2);
    // 5 秒
    event_loop(5);
    return 0;
}
