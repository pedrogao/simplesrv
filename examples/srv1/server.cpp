#include <iostream>
#include "Socket.h"

using namespace std;

int main(int argc, char const *argv[])
{
    InetAddress addr("127.0.0.1", 8080);

    cout << addr.ToString() << endl;

    return 0;
}
