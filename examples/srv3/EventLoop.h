#ifndef NEBULA_EVENTLOOP_H_
#define NEBULA_EVENTLOOP_H_

#include <functional>

class Poller;
class Channel;
class EventLoop
{
public:
    EventLoop();
    ~EventLoop();

    void Loop();
    void UpdateChannel(Channel *ch);
    void DeleteChannel(Channel *ch);

private:
    Poller *poller_{nullptr};
    bool quit_{false};
};

#endif