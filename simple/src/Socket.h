#pragma once

class InetAddress;
class Socket
{
private:
    int fd;

public:
    Socket();
    Socket(int fd);
    ~Socket();

    void bind(InetAddress *);
    void listen();
    void setnoblocking();

    int accept(InetAddress *);
    int getFd();
};
