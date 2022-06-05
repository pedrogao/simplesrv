#include "Socket.h"
#include "EventLoop.h"
#include "Channel.h"
#include <iostream>
#include <unistd.h>

using namespace std;

void ReadNonBlocking(Socket *sock, Channel *ch, EventLoop *loop)
{
    int sockfd = sock->GetFd();
    char buf[1024]; // 这个buf大小无所谓
    while (true)
    {
        // 使用非阻塞IO，读取客户端buffer，一次读取buf大小数据，直到全部读取完毕
        memset(buf, 0, sizeof(buf));
        ssize_t bytes_read = read(sockfd, buf, sizeof(buf));
        if (bytes_read > 0)
        {
            printf("read:\n %s\n", buf);
        }
        else if (bytes_read == -1 && errno == EINTR)
        {
            // 程序正常中断、继续读取
            printf("continue reading\n");
            continue;
        }
        else if (bytes_read == -1 &&
                 ((errno == EAGAIN) || (errno == EWOULDBLOCK)))
        {
            // 非阻塞IO，这个条件表示数据全部读取完毕
            break;
        }
        else if (bytes_read == 0)
        {
            // EOF，客户端断开连接
            printf("read EOF, client fd %d disconnected\n", sockfd);
            loop->DeleteChannel(ch);
            delete sock;
            delete ch;
            break;
        }
        else
        {
            printf("Other error on client fd %d\n", sockfd);
            loop->DeleteChannel(ch);
            delete sock;
            delete ch;
            break;
        }
    }
}

void AcceptConnection(Socket *sock, EventLoop *loop)
{
    InetAddress *clnt_addr = new InetAddress();
    Socket *clnt_sock = new Socket(sock->Accept(clnt_addr));
    printf("new client fd %d! IP: %s Port: %d\n", clnt_sock->GetFd(), clnt_addr->GetIp(), clnt_addr->GetPort());
    clnt_sock->SetNonBlocking(); // 新接受到的连接设置为非阻塞式
    Channel *clnt_ch = new Channel(loop, clnt_sock);
    clnt_ch->EnableRead();
    std::function<void()> cb = std::bind(ReadNonBlocking, clnt_sock, clnt_ch, loop);
    clnt_ch->SetReadCallback(cb);

    delete clnt_addr;
}

int main(int argc, char const *argv[])
{
    EventLoop loop;
    InetAddress addr("127.0.0.1", 8080);
    cout << addr.ToString() << endl;
    // server socket is blocking
    Socket socket;
    // bind
    socket.Bind(&addr);
    // listen
    socket.Listen();
    // server channel
    Channel serv_ch(&loop, &socket);
    // refer: https://www.cnblogs.com/DswCnblog/p/5629165.html
    std::function<void()> cb = std::bind(AcceptConnection, &socket, &loop);
    serv_ch.SetReadCallback(cb);
    serv_ch.EnableRead();
    // loop events
    loop.Loop();

    return 0;
}
