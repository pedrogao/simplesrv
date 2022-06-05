#ifndef NEBULA_CHANNEL_H_
#define NEBULA_CHANNEL_H_

#include <functional>

class Socket;
class EventLoop;
class Channel
{
public:
    Channel(EventLoop *loop, Socket *socket);
    ~Channel();

    void HandleEvent();
    void EnableRead();
    void EnableWrite();

    Socket *GetSocket();
    int GetListenEvents();
    int GetReadyEvents();
    bool GetExist();
    void SetExist(bool in = true);
    void UseET();

    void SetReadyEvents(int ev);
    void SetReadCallback(std::function<void()> const &callback);
    void SetWriteCallback(std::function<void()> const &callback);

    static const int READ_EVENT;  // NOLINT
    static const int WRITE_EVENT; // NOLINT
    static const int ET;          // NOLINT

private:
    EventLoop *loop_;
    Socket *socket_;
    int listen_events_{0}; // 监听事件
    int ready_events_{0};  // 就绪事件
    bool exist_{false};
    std::function<void()> read_callback_;
    std::function<void()> write_callback_;
};

#endif