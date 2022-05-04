#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include "util.h"

#define BUFFER_SIZE 1024

int main(int argc, char const *argv[])
{
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    errif(sockfd == -1, "socket create error");

    struct sockaddr_in serv_addr;
    bzero(&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    serv_addr.sin_port = htons(8888);

    int ret = connect(sockfd, (sockaddr *)&serv_addr, sizeof(serv_addr));
    errif(ret == -1, "socket connect error");

    while (true)
    {
        char buf[BUFFER_SIZE];
        bzero(&buf, sizeof(buf));
        scanf("%s", buf);                                      // 从键盘输入要传到服务器的数据
        ssize_t write_bytes = write(sockfd, buf, sizeof(buf)); // 发送缓冲区中的数据到服务器socket，返回已发送数据大小
        if (write_bytes == -1)
        {
            printf("socket already disconnected, can't write any more!\n");
            break;
        }
        bzero(&buf, sizeof(buf));                            // 清空缓冲区
        ssize_t read_bytes = read(sockfd, buf, sizeof(buf)); // 从服务器socket读到缓冲区，返回已读数据大小
        if (read_bytes > 0)
        {
            printf("message from server: %s\n", buf);
        }
        else if (read_bytes == 0)
        {
            // read返回0，表示EOF，通常是服务器断开链接，等会儿进行测试
            printf("server socket disconnected!\n");
            break;
        }
        else if (read_bytes == -1)
        {
            // read返回-1，表示发生错误，按照上文方法进行错误处理
            close(sockfd);
            errif(true, "socket read error");
        }
    }
    close(sockfd);
    return 0;
}
