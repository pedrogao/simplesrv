#include <iostream>
#include "Schedule.h"

// global scheduler
Scheduler scheduler;

void func1()
{
    while (true)
    {
        std::cout << "Coroutine 1 print per 500ms" << std::endl;
        scheduler.CoroutineSleep(500);
    }
}

void func2()
{
    while (true)
    {
        std::cout << "Coroutine 2 print per 1000ms" << std::endl;
        scheduler.CoroutineSleep(1000);
    }
}

int main(int argc, char const *argv[])
{
    Context co1(func1);
    scheduler.ResumeCoroutine(&co1);

    Context co2(func2);
    scheduler.ResumeCoroutine(&co2);
    // 5 秒
    scheduler.EventLoop(5);
    return 0;
}