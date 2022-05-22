/**
 * @file Poller.h
 * @author pedrogao
 * @brief
 * @version 0.1
 * @date 2022-05-18
 *
 * @copyright Copyright (c) 2022
 */

#ifndef NEBULA_POLLER_H_
#define NEBULA_POLLER_H_

#include <vector>

#ifdef OS_LINUX
#include <sys/epoll.h>
#endif

#ifdef OS_MACOS
#include <sys/event.h>
#endif

class Channel;
class Poller
{
public:
    Poller();
    ~Poller();

    void UpdateChannel(Channel *ch);
    void DeleteChannel(Channel *ch);

    /**
     * @brief poll and get events
     *
     * @param timeout -1 resp infinate time
     * @return std::vector<Channel *>
     */
    std::vector<Channel *> Poll(int timeout = -1);

private:
    int fd_{-1};

#ifdef OS_LINUX
    struct epoll_event *events_{nullptr};
#endif

#ifdef OS_MACOS
    struct kevent *events_{nullptr};
#endif
};

#endif