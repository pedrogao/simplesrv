#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/epoll.h>
#include <errno.h>

void errif(bool condition, const char *errmsg)
{
    if (condition)
    {
        perror(errmsg);
        exit(EXIT_FAILURE);
    }
}

int main(int argc, char const *argv[])
{
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    errif(sockfd == -1, "create socket err");

    struct sockaddr_in serv_addr;
    bzero(&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    serv_addr.sin_port = htons(8888);

    // bind & listen
    int ret;
    ret = bind(sockfd, (sockaddr *)&serv_addr, sizeof(serv_addr));
    errif(ret == -1, "socket bind error");

    ret = listen(sockfd, SOMAXCONN);
    errif(ret == -1, "socket listen error");

    // accept
    struct sockaddr_in clnt_addr;
    socklen_t clnt_addr_len = sizeof(clnt_addr);
    bzero(&clnt_addr, sizeof(clnt_addr));
    int clnt_sockfd = accept(sockfd, (sockaddr *)&clnt_addr, &clnt_addr_len);
    errif(clnt_sockfd == -1, "socket accept error");
    printf("new client fd %d! IP: %s Port: %d\n", clnt_sockfd, inet_ntoa(clnt_addr.sin_addr), ntohs(clnt_addr.sin_port));

    while (true)
    {
        char buf[1024];
        bzero(&buf, sizeof(buf));
        ssize_t bytes_read = read(clnt_sockfd, buf, sizeof(buf));
        if (bytes_read > 0)
        {
            printf("message from client fd %d: %s\n", clnt_sockfd, buf);
            write(clnt_sockfd, buf, sizeof(buf)); // echo
        }
        else if (bytes_read == 0)
        {
            printf("client fd %d disconnected\n", clnt_sockfd);
            close(clnt_sockfd);
            break;
        }
        else if (bytes_read == -1) // read err
        {
            close(clnt_sockfd);
            errif(true, "socket read error");
        }
    }

    return 0;
}
