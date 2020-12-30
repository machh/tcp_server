#include "SocketAPI.h"

#include <unistd.h>
#include <sys/time.h>
#include <sys/ioctl.h>                     // for ioctl()
#include <netinet/in.h>
#include <errno.h>			// for errno

extern int errno;

char Error[256];

SOCKET SocketAPI::socket_ex(int domain, int type, int protocol) {
    // 系统函数
    SOCKET s = socket(domain, type, protocol);
    if (s == INVALID_SOCKET) {
        switch (errno) {
            case EPROTONOSUPPORT:
            case EMFILE:
            case ENFILE:
            case EACCES:
            case ENOBUFS:
            default:
                break;
        }//end of switch
    }
    return s;
}

bool SocketAPI::bind_ex(SOCKET s, const struct sockaddr* addr, unsigned int addrlen) {
    if (bind(s, addr, addrlen) == SOCKET_ERROR) {
        switch (errno) {
            case EADDRINUSE:
            case EINVAL:
            case EACCES:
            case ENOTSOCK:
            case EBADF:
            case EROFS:
            case EFAULT:
            case ENAMETOOLONG:
            case ENOENT:
            case ENOMEM:
            case ENOTDIR:
            case ELOOP:
            default:
                break;
        }//end of switch
        return false;
    }
    return true;
}

bool SocketAPI::connect_ex(SOCKET s, const struct sockaddr * addr, unsigned int addrlen) {
    if (connect(s, addr, addrlen) == SOCKET_ERROR) {
        switch (errno) {
            case EALREADY:
            case EINPROGRESS:
            case ECONNREFUSED:
            case EISCONN:
            case ETIMEDOUT:
            case ENETUNREACH:
            case EADDRINUSE:
            case EBADF:
            case EFAULT:
            case ENOTSOCK:
            default:
                break;
        }//end of switch
        return false;
    }
    return true;
}

bool SocketAPI::listen_ex(SOCKET s, unsigned int backlog) {
    if (listen(s, backlog) == SOCKET_ERROR) {
        switch (errno) {
            case EBADF:
            case ENOTSOCK:
            case EOPNOTSUPP:
            default:
                break;
        }//end of switch
        return false;
    }
    return true;
}

SOCKET SocketAPI::accept_ex(SOCKET s, struct sockaddr* addr, unsigned int* addrlen) {
    SOCKET client = accept(s, addr, addrlen);
    if (client == INVALID_SOCKET) {
        switch (errno) {
            case EWOULDBLOCK:
            case ECONNRESET:
            case ECONNABORTED:
            case EPROTO:
            case EINTR:
                // from UNIX Network Programming 2nd, 15.6
                // with nonblocking-socket, ignore above errors
            case EBADF:
            case ENOTSOCK:
            case EOPNOTSUPP:
            case EFAULT:
            default:
                break;
        }//end of switch
    }
    return client;
}

bool SocketAPI::getsockopt_ex(SOCKET s, int level, int optname, void* optval, unsigned int* optlen) {
    if (getsockopt(s, level, optname, optval, optlen) == SOCKET_ERROR) {
        switch (errno) {
            case EBADF:
            case ENOTSOCK:
            case ENOPROTOOPT:
            case EFAULT:
            default:
                break;
        }//end of switch
        return false;
    }
    return true;
}

unsigned int SocketAPI::getsockopt_ex2(SOCKET s, int level, int optname, void* optval, unsigned int* optlen) {
    if (getsockopt(s, level, optname, optval, optlen) == SOCKET_ERROR) {
        switch (errno) {
            case EBADF:
                return 1;
            case ENOTSOCK:
                return 2;
            case ENOPROTOOPT:
                return 3;
            case EFAULT:
                return 4;
            default:
                return 5;
        }//end of switch
    }
    return 0;
}

bool SocketAPI::setsockopt_ex(SOCKET s, int level, int optname, const void* optval, unsigned int optlen) {
    if (setsockopt(s, level, optname, optval, optlen) == SOCKET_ERROR) {
        switch (errno) {
            case EBADF:
            case ENOTSOCK:
            case ENOPROTOOPT:
            case EFAULT:
            default:
                break;
        }//end of switch
        return false;
    }
    return true;
}

unsigned int SocketAPI::send_ex(SOCKET s, const void* buf, unsigned int len, unsigned int flags) {
    int nSent;
    try {
        nSent = send(s, buf, len, flags);
        if (nSent == SOCKET_ERROR) {
            switch (errno) {
                case EWOULDBLOCK:
                    return SOCKET_ERROR_WOULDBLOCK;
                case ECONNRESET:
                case EPIPE:
                case EBADF:
                case ENOTSOCK:
                case EFAULT:
                case EMSGSIZE:
                case ENOBUFS:

                default:
                    break;
            }//end of switch
        } else if (nSent == 0) {
            // TODO 发送的数据为0
        }
    } catch (...) {
    }
    return nSent;
}

unsigned int SocketAPI::sendto_ex(SOCKET s, const void* buf, unsigned int len, unsigned int flags,
                                  const struct sockaddr* to, int tolen) {
    int nSent = sendto(s, buf, len, flags, to, tolen);
    if (nSent == SOCKET_ERROR) {
        switch (errno) {
            case EWOULDBLOCK:
                return 0;
            case ECONNRESET:
            case EPIPE:
            case EBADF:
            case ENOTSOCK:
            case EFAULT:
            case EMSGSIZE:
            case ENOBUFS:
            default:
                break;
        }
    }
    return nSent;
}

unsigned int SocketAPI::recv_ex(SOCKET s, void* buf, unsigned int len, unsigned int flags) {
    int nReceived = recv(s, buf, len, flags);
    if (nReceived == SOCKET_ERROR) {
        switch (errno) {
            case EWOULDBLOCK:
                return SOCKET_ERROR_WOULDBLOCK;
            case ECONNRESET:
            case EPIPE:
            case EBADF:
            case ENOTCONN:
            case ENOTSOCK:
            case EINTR:
            case EFAULT:
            default:
                break;
        }//end of switch
        return 0;
    } else if (nReceived == 0) {
    }
    return nReceived;
}

unsigned int SocketAPI::recvfrom_ex(SOCKET s, void* buf, unsigned int len, unsigned int flags,
                                    struct sockaddr* from, unsigned int* fromlen) {
    int nReceived = recvfrom(s, buf, len, flags, from, fromlen);
    if (nReceived == SOCKET_ERROR) {
        switch (errno) {
            case EWOULDBLOCK:
                return SOCKET_ERROR_WOULDBLOCK;
            case ECONNRESET:
            case EPIPE:
            case EBADF:
            case ENOTCONN:
            case ENOTSOCK:
            case EINTR:
            case EFAULT:

            default:
                break;
        }//end of switch
        return 0;
    } else if (nReceived == 0) {
    }
    return nReceived;
}

bool SocketAPI::closesocket_ex(SOCKET s) {
    close(s);
    return true;
}

bool SocketAPI::ioctlsocket_ex(SOCKET s, long cmd, unsigned long* argp) {
    return true;
}

bool SocketAPI::getsocketnonblocking_ex(SOCKET s) {
    int flags = fcntl(s, F_GETFL, 0);
    if (flags < 0) {
        switch (errno) {
            case EINTR:
            case EINVAL:
            case EBADF:
            case EACCES:
            case EAGAIN:
            case EDEADLK:
            case EMFILE:
            case ENOLCK:
            default:
                break;
        }
        flags = 0;
    }

    return flags | O_NONBLOCK;
}

bool SocketAPI::setsocketnonblocking_ex(SOCKET s, bool on) {
    int flags = fcntl(s, F_GETFL, 0);
    if (flags < 0) {
        switch (errno) {
            case EINTR:
            case EINVAL:
            case EBADF:
            case EACCES:
            case EAGAIN:
            case EDEADLK:
            case EMFILE:
            case ENOLCK:
            default:
                break;
        }
        flags = 0;
    }

    if (on) {
        flags |= O_NONBLOCK; // make nonblocking fd
    }
    else {
        flags &= ~O_NONBLOCK; // make blocking fd
    }

    fcntl(s, F_SETFL, flags);

    return true;
}

unsigned int SocketAPI::availablesocket_ex(SOCKET s) {
    unsigned int arg = 0;
    ioctl(s, FIONREAD, &arg);
    return arg;
}

bool SocketAPI::shutdown_ex(SOCKET s, unsigned int how) {
    if (shutdown(s, how) < 0) {
        switch (errno) {
            case EBADF:
            case ENOTSOCK:
            case ENOTCONN:
            default:
                break;
        }
        return false;
    }
    return true;
}

int SocketAPI::select_ex(int maxfdp1, fd_set* readset, fd_set* writeset, fd_set* exceptset, struct timeval* timeout) {
    int result;
    try {
        result = select(maxfdp1, readset, writeset, exceptset, timeout);
        if (result == SOCKET_ERROR) {
            // TODO
        }
        return result;
    } catch (...) {
    }
    return result;
}