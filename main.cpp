#include <iostream>
#include <thread>
#include <atomic>
#include <list>
#include <memory>
#include <string.h>

#include "epollCtrl.h"

#include "Socket.h"
using namespace SocketAPI;

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

    SOCKET socketID = SocketAPI::socket_ex(AF_INET, SOCK_STREAM, 0);

    std::cout << "socketID = " << socketID<< std::endl;

    SOCKADDR_IN m_SockAddr;
    m_SockAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    m_SockAddr.sin_port = htons(7700);

    bool bok = SocketAPI::bind_ex(socketID, (const struct sockaddr *) &m_SockAddr, sizeof (m_SockAddr));
    if(!bok){
        std::cout << "bind_ex error !" << std::endl;
        return -1;
    }
    std::cout << "bind_ex = " << bok<< std::endl;

    listen_ex(socketID, false);

    for (;;){
        sockaddr addr;
        unsigned int len = sizeof(sockaddr);
        SOCKET clientSock = SocketAPI::accept_ex(socketID, &addr, &len);

        std::atomic<bool>* _flag = new std::atomic<bool>(true);
        std::thread __readthread(processClientRead, clientSock, _flag);
        __readthread.detach();

    }

    std::cout << "Done !" << std::endl;
    return 0;


#if 0
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
#endif

    return 0;
}
