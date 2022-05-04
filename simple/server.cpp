#include "src/EventLoop.h"
#include "src/Server.h"

int main(int argc, char const *argv[])
{
    EventLoop *loop = new EventLoop(); // epoll
    Server *server = new Server(loop); // socket server
    loop->loop();

    return 0;
}
