#pragma once
#include <arpa/inet.h>

class InetAddress
{
private:
    struct sockaddr_in addr;

public:
    InetAddress();
    InetAddress(const char *_ip, uint16_t _port);
    ~InetAddress();

    void setInetAddr(sockaddr_in _addr);
    sockaddr_in getAddr();
    char *getIp();
    uint16_t getPort();
};

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
    void connect(InetAddress *);
    
    int getFd();
};
