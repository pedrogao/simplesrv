#pragma once

class Epoll;
class Channel;
// EventLoop 对 epoll 的包装，事件监听器
class EventLoop
{
private:
    Epoll *ep;
    bool quit;

public:
    EventLoop();
    ~EventLoop();

    void loop();
    void updateChannel(Channel *);
};