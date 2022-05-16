/**
 * @file Socket.cpp
 * @author pedrogao
 * @brief
 * @version 0.1
 * @date 2022-05-15
 *
 * @copyright Copyright (c) 2022
 *
 */

#include "Socket.h"
#include "util.h"
#include <fcntl.h>
#include <sys/socket.h>
#include <cstring>
#include <sstream>
#include <unistd.h>

InetAddress::InetAddress() = default;

InetAddress::InetAddress(const char *ip, uint16_t port)
{
    memset(&addr_, 0, sizeof(addr_));
    addr_.sin_family = AF_INET;
    addr_.sin_addr.s_addr = inet_addr(ip);
    addr_.sin_port = htons(port);
}

void InetAddress::SetAddr(sockaddr_in addr)
{
    addr_ = addr;
}

sockaddr_in InetAddress::GetAddr()
{
    return addr_;
}

const char *InetAddress::GetIp()
{
    return inet_ntoa(addr_.sin_addr);
}

uint16_t InetAddress::GetPort()
{
    return ntohs(addr_.sin_port);
}

std::string InetAddress::ToString()
{
    std::stringstream ss;
    ss << GetIp() << ":" << GetPort();
    return ss.str();
}

Socket::Socket()
{
    fd_ = ::socket(AF_INET, SOCK_STREAM, 0);
    PanicIf(fd_ == -1, "create socket err");
}

Socket::Socket(int fd) : fd_(fd)
{
    PanicIf(fd_ == -1, "create socket err");
}

Socket::~Socket()
{
    if (fd_ != -1)
    {
        ::close(fd_);
    }
}

void Socket::Bind(InetAddress *addr)
{
    auto saddr = addr->GetAddr();
    PanicIf(::bind(fd_, (sockaddr *)&saddr, sizeof(sockaddr)) == -1, "bind socker err");
}

void Socket::Listen()
{
    PanicIf(::listen(fd_, SOMAXCONN) == -1, "listen err");
}

void Socket::SetNonBlocking()
{
    ::fcntl(fd_, F_SETFL, ::fcntl(fd_, F_GETFL) | O_NONBLOCK);
}

bool Socket::IsNonBlocking()
{
    return (::fcntl(fd_, F_GETFL) & O_NONBLOCK) == 1;
}

int Socket::Accept(InetAddress *addr)
{
    int c_fd = -1;
    struct sockaddr_in saddr;
    socklen_t addr_len = sizeof(saddr);
    if (IsNonBlocking())
    {
        while (true)
        {
            c_fd = ::accept(fd_, (sockaddr *)&saddr, &addr_len);
            // 参考：https://stackoverflow.com/questions/4058368/what-does-eagain-mean
            // https://stackoverflow.com/questions/49049430/difference-between-eagain-or-ewouldblock
            if (c_fd == -1 && ((errno == EAGAIN) || (errno == EWOULDBLOCK))) // 资源未准备好
            {
                continue;
            }
            if (c_fd == -1)
            {
                Panic("accept error");
            }
            else
            {
                break;
            }
        }
    }
    else
    {
        c_fd = ::accept(fd_, (sockaddr *)&saddr, &addr_len);
        PanicIf(c_fd == -1, "accept error");
    }
    addr->SetAddr(saddr);
    return c_fd;
}

void Socket::Connect(InetAddress *addr)
{
    auto saddr = addr->GetAddr();
    if (IsNonBlocking())
    {
        while (true)
        {
            int ret = ::connect(fd_, (sockaddr *)&saddr, sizeof(saddr));
            if (ret == 0) // 连接成功
            {
                break;
            }
            if (ret == -1 && (errno == EINPROGRESS)) // 正在连接中
            {
                continue;
            }
            if (ret == -1)
            {
                Panic("connect error");
            }
        }
    }
    else
    {
        PanicIf(::connect(fd_, (sockaddr *)&saddr, sizeof(saddr)) == -1, "connect err");
    }
}

void Socket::Connect(const char *ip, uint16_t port)
{
    InetAddress addr(ip, port);
    Connect(&addr);
}

int Socket::GetFd()
{
    return fd_;
}