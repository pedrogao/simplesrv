#pragma once
#include <sys/epoll.h>

class Epoll;
// Channel(通道) 对 fd 监听事件的抽象
class Channel
{
private:
    Epoll *ep;
    int fd;
    uint32_t events;
    uint32_t revents;
    bool inEpoll;

public:
    Channel(Epoll *_ep, int _fd);
    ~Channel();

    void enableReading();

    int getFd();
    uint32_t getEvents();
    uint32_t getRevents();
    bool getInEpoll();
    void setInEpoll();

    void setRevents(uint32_t);
};
