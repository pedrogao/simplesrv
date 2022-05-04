#pragma once

#include <map>

class EventLoop;
class Socket;
class Acceptor;
class Connection;
class Server
{
private:
    EventLoop *loop;
    Acceptor *acceptor;                      // TCP连接器
    std::map<int, Connection *> connections; // 所有TCP连接

public:
    Server(EventLoop *);
    ~Server();

    void handleReadEvent(int);
    void newConnection(Socket *sock);  // 新建TCP连接
    void deleteConnection(int sockfd); // 断开TCP连接
};