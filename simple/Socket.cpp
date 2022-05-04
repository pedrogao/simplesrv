#include <unistd.h>
#include <fcntl.h>
#include <sys/socket.h>

#include "Socket.h"
#include "InetAddress.h"
#include "util.h"

Socket::Socket() : fd(-1)
{
    fd = socket(AF_INET, SOCK_STREAM, 0);
    errif(fd == -1, "create socket error");
}

Socket::Socket(int fd) : fd(fd)
{
    errif(fd == -1, "create socket error");
}

Socket::~Socket()
{
    if (fd != -1)
    {
        close(fd);
        fd = -1;
    }
}

void Socket::bind(InetAddress *addr)
{
    int ret = ::bind(fd, (sockaddr *)&addr->addr, addr->addr_len);
    errif(ret == -1, "bind socket err");
}

void Socket::listen()
{
    int ret = ::listen(fd, SOMAXCONN);
    errif(ret == -1, "listen socket err");
}

void Socket::setnoblocking()
{
    ::fcntl(fd, F_SETFL, fcntl(fd, F_GETFL) | O_NONBLOCK);
}

int Socket::accept(InetAddress *addr)
{
    int clnt_sockfd = ::accept(fd, (sockaddr *)&addr->addr, &addr->addr_len);
    errif(clnt_sockfd == -1, "accept socket error");
    return clnt_sockfd;
}

int Socket::getFd()
{
    return fd;
}