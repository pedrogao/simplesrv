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
#include <cstring>
#include <sstream>

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

Socket::Socket(/* args */)
{
    // TODO socket
}

Socket::~Socket()
{
    // TODO close socket
}