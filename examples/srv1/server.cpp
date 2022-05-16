#include "Socket.h"
#include <iostream>
#include <unistd.h>

using namespace std;

int main(int argc, char const *argv[])
{
    InetAddress addr("127.0.0.1", 8080);
    cout << addr.ToString() << endl;

    Socket socket;
    // bind
    socket.Bind(&addr);
    // listen
    socket.Listen();
    // accept
    while (true)
    {
        InetAddress caddr;
        int cfd = socket.Accept(&caddr);
        if (cfd == -1)
        {
            cout << "accept err: " << caddr.ToString() << endl;
            continue;
        }
        cout << "accept successful, fd: " << cfd << endl;
        // close instantly
        ::close(cfd);
    }

    return 0;
}
