/**
 * @file Socket.h
 * @author pedrogao
 * @brief InetAddress & Socket
 * @version 0.1
 * @date 2022-05-15
 *
 * @copyright Copyright (c) 2022
 *
 */

#ifndef NEBULA_SOCKET_H_
#define NEBULA_SOCKET_H_

#include <arpa/inet.h>
#include <string>

class InetAddress
{
public:
    InetAddress();
    InetAddress(const char *ip, uint16_t port);
    ~InetAddress() = default;

    void SetAddr(sockaddr_in addr);
    sockaddr_in GetAddr();
    const char *GetIp();
    uint16_t GetPort();
    std::string ToString();

private:
    struct sockaddr_in addr_;
};

class Socket
{
public:
    Socket();
    // 在C++中，explicit关键字用来修饰类的构造函数，被修饰的构造函数的类，不能发生相应的隐式类型转换
    explicit Socket(int fd);
    ~Socket();

private:
    int fd{-1};
};

#endif