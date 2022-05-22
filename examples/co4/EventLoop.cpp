#include "EventLoop.h"
#include <vector>
#include "Channel.h"
#include "Poller.h"

EventLoop::EventLoop() { poller_ = new Poller(); }

EventLoop::~EventLoop() { delete poller_; }

void EventLoop::Loop()
{
    while (!quit_)
    {
        std::vector<Channel *> chs;
        chs = poller_->Poll();
        for (auto &ch : chs)
        {
            ch->HandleEvent();
        }
    }
}

void EventLoop::UpdateChannel(Channel *ch) { poller_->UpdateChannel(ch); }
void EventLoop::DeleteChannel(Channel *ch) { poller_->DeleteChannel(ch); }