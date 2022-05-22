#include "Socket.h"
#include "Coroutine.h"
#include "fiber.h"
#include <iostream>
#include <unistd.h>

using namespace std;

ef_fiber_sched_t runtime{};

long hello_proc(void *param)
{
    cout << "hello proc\n"
         << endl;
    return ef_fiber_yield(&runtime, 100);
}

int main(int argc, char const *argv[])
{
    // init fiber runtime
    ef_fiber_init_sched(&runtime, 1);
    Coroutine *co1 = new Coroutine(hello_proc, nullptr);
    long retval;
    ef_fiber_resume(&runtime, co1->GetFiber(), 100, &retval);
    cout << "retval: " << retval << endl;

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

    delete co1;

    return 0;
}
