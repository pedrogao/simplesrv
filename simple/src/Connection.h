#pragma once
#include <functional>

class EventLoop;
class Socket;
class Channel;
class Buffer;
class Connection
{
private:
    EventLoop *loop;
    Socket *sock;
    Channel *channel;
    std::function<void(int)> deleteConnectionCallback; // 断开连接回调
    Buffer *readBuffer;

public:
    Connection(EventLoop *_loop, Socket *_sock);
    ~Connection();

    void echo(int sockfd);
    void setDeleteConnectionCallback(std::function<void(int)>);
    void send(int sockfd);
};