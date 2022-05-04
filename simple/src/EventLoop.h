#pragma once
#include <functional>

class Epoll;
class Channel;
class ThreadPool;
// EventLoop 对 epoll 的包装，事件监听器
class EventLoop
{
private:
    Epoll *ep;
    ThreadPool *threadPool;
    bool quit;

public:
    EventLoop();
    ~EventLoop();

    void loop();
    void updateChannel(Channel *);

    void addThread(std::function<void()>);
};