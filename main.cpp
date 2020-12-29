#include <iostream>
#include <thread>
#include <atomic>
#include <list>
#include <memory>

#include "epollCtrl.h"

#include "Socket.h"

void processClientRead(SOCKET clientsock, std::atomic<bool>* _flag) {
    char buf[256] = {0};
    while (_flag->load()) {
        int n = SocketAPI::recv_ex(clientsock, buf, 256, 0);
        std::cout << "recv:" << buf << std::endl;
        if (strncmp(buf, "exit", 4) == 0) {
            _flag->exchange(false);
        }
        memset(buf, 0, n + 1);
    }
    std::cout << "processClientRead Over" << std::endl;
}

int main() {
    std::cout << "Hello, World!" << std::endl;

    Socket sck("192.168.1.101", 9000);
    sck.bind();
    sck.listen(false);

    struct sockaddr_in addr;
    addr.sin_port = htons(8080);
    inet_pton(AF_INET, "192.168.1.101", &addr.sin_addr);

    for (;;) {
        SOCKET clientSock = sck.accept(nullptr, nullptr ); //(const struct sockaddr *) &addr, sizeof (struct sockaddr_in)

        std::atomic<bool>* _flag = new std::atomic<bool>(true);
        std::thread __readthread(processClientRead, clientSock, _flag);
        __readthread.detach();
    }

    sck.close();

    std::cout << "Done !" << std::endl;

    return 0;
}
