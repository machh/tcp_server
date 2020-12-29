//
// Created by machh on 2020/12/23.
//

#include "Socket.h"
#include "Socket.h"
#include <string.h>

Socket::Socket() {
    // 设置socket fd
    m_SocketID = INVALID_SOCKET;
    // 初始化addr
    memset(&m_SockAddr, 0, sizeof (SOCKADDR_IN));
    memset(m_Host, 0, IP_SIZE);
    m_Port = 0;
}

Socket::Socket(const char* host, unsigned int port) {
    // 初始化host port
    strncpy(m_Host, host, IP_SIZE - 1);
    m_Port = port;

    // 创建socket
    create();
}

Socket::~Socket() {
    // 关闭socket
    close();
}

bool Socket::create() {
    // 创建socket
    m_SocketID = SocketAPI::socket_ex(AF_INET, SOCK_STREAM, 0);
    // 设置addr
    memset(&m_SockAddr, 0, sizeof (m_SockAddr));
    m_SockAddr.sin_family = AF_INET;

    if (isValid())
        return true;
    else
        return false;
}

bool Socket::connect() {
    m_SockAddr.sin_addr.s_addr = inet_addr(m_Host);

    // set sockaddr's port
    m_SockAddr.sin_port = htons(m_Port);

    // try to connect to peer host
    return SocketAPI::connect_ex(m_SocketID, (const struct sockaddr *) &m_SockAddr, sizeof (m_SockAddr));
}

bool Socket::connect(const char* host, unsigned int port) {
    strncpy(m_Host, host, IP_SIZE - 1);
    m_Port = port;
    return connect();
}

bool Socket::reconnect(const char* host, unsigned int port) {
    // delete old socket impl object
    close();

    // create new socket impl object
    strncpy(m_Host, host, IP_SIZE - 1);
    m_Port = port;

    create();

    // try to connect
    return connect();
}

void Socket::close() {
    if (isValid() && !isSockError()) {
        try {
            SocketAPI::closesocket_ex(m_SocketID);
        } catch (...) {
        }
    }

    m_SocketID = INVALID_SOCKET;
    memset(&m_SockAddr, 0, sizeof (SOCKADDR_IN));
    memset(m_Host, 0, IP_SIZE);
    m_Port = 0;
}

bool Socket::bind() {
    m_SockAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    m_SockAddr.sin_port = htons(m_Port);

    return SocketAPI::bind_ex(m_SocketID, (const struct sockaddr *) &m_SockAddr, sizeof (m_SockAddr));
}

bool Socket::bind(unsigned int port) {
    m_Port = port;
    m_SockAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    m_SockAddr.sin_port = htons(m_Port);
    return SocketAPI::bind_ex(m_SocketID, (const struct sockaddr *) &m_SockAddr, sizeof (m_SockAddr));
}

bool Socket::listen(int backlog) {
    return SocketAPI::listen_ex(m_SocketID, backlog);
}

SOCKET Socket::accept(sockaddr* addr, unsigned int* addrlen) {
    return SocketAPI::accept_ex(m_SocketID, addr, addrlen);
}

unsigned int Socket::send(const void* buf, unsigned int len, unsigned int flags) {
    return SocketAPI::send_ex(m_SocketID, buf, len, flags);
}

unsigned int Socket::receive(void* buf, unsigned int len, unsigned int flags) {
    return SocketAPI::recv_ex(m_SocketID, buf, len, flags);
}

unsigned int Socket::available() const {
    return SocketAPI::availablesocket_ex(m_SocketID);
}

unsigned int Socket::getReceiveBufferSize() const {
    unsigned int ReceiveBufferSize;
    unsigned int size = sizeof (ReceiveBufferSize);
    SocketAPI::getsockopt_ex(m_SocketID, SOL_SOCKET, SO_RCVBUF, &ReceiveBufferSize, &size);
    return ReceiveBufferSize;
}

bool Socket::setReceiveBufferSize(unsigned int size) {
    return (bool) (SocketAPI::setsockopt_ex(m_SocketID, SOL_SOCKET, SO_RCVBUF, &size, sizeof (unsigned int)));
}

unsigned int Socket::getSendBufferSize() const {
    unsigned int SendBufferSize;
    unsigned int size = sizeof (SendBufferSize);
    SocketAPI::getsockopt_ex(m_SocketID, SOL_SOCKET, SO_SNDBUF, &SendBufferSize, &size);
    return SendBufferSize;
}

bool Socket::setSendBufferSize(unsigned int size) {
    return (bool)(SocketAPI::setsockopt_ex(m_SocketID, SOL_SOCKET, SO_SNDBUF, &size, sizeof (unsigned int)));
}

unsigned int Socket::getLinger() const {
    struct linger ling;
    unsigned int len = sizeof (ling);
    SocketAPI::getsockopt_ex(m_SocketID, SOL_SOCKET, SO_LINGER, &ling, &len);
    return ling.l_linger;
}

bool Socket::setLinger(unsigned int lingertime) {
    struct linger ling;
    ling.l_onoff = lingertime > 0 ? 1 : 0;
    ling.l_linger = lingertime;
    return SocketAPI::setsockopt_ex(m_SocketID, SOL_SOCKET, SO_LINGER, &ling, sizeof (ling));
}

unsigned int Socket::getSockError() const {
    return isSockError();
}

bool Socket::isNonBlocking() const {
    return SocketAPI::getsocketnonblocking_ex(m_SocketID);
}

bool Socket::setNonBlocking(bool on) {
    return SocketAPI::setsocketnonblocking_ex(m_SocketID, on);
}

unsigned int Socket::getPort() const {
    return m_Port;
}

IP_t Socket::getHostIP() const {
    return (IP_t) (m_SockAddr.sin_addr.s_addr);
}

bool Socket::isValid() const {
    return m_SocketID != INVALID_SOCKET;
}

SOCKET Socket::getSOCKET() const {
    return m_SocketID;
}

bool Socket::isSockError() const {
    int error;
    unsigned int len = sizeof (error);

    int result = SocketAPI::getsockopt_ex2(m_SocketID, SOL_SOCKET, SO_ERROR, &error, &len);
    if (result == 0)
        return false;
    else
        return true;
}

bool Socket::isReuseAddr() const {
    int reuse;
    unsigned int len = sizeof (reuse);
    SocketAPI::getsockopt_ex(m_SocketID, SOL_SOCKET, SO_REUSEADDR, &reuse, &len);
    return reuse == 1;
}

bool Socket::setReuseAddr(bool on) {
    int opt = (on == true ? 1 : 0);

    return SocketAPI::setsockopt_ex(m_SocketID, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof (opt));
}