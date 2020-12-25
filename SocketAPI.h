#ifndef CN_VICKY__SOCKETAPI_H
#define	CN_VICKY__SOCKETAPI_H

// https://gitee.com/eclipser/socket

#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <fcntl.h>

#define _ESIZE 256

typedef int SOCKET;
#define INVALID_SOCKET   -1
#define	SOCKET_ERROR	 -1

static const int SOCKET_ERROR_WOULDBLOCK = -100;

typedef struct sockaddr SOCKADDR;
typedef struct sockaddr_in SOCKADDR_IN;
static const unsigned int szSOCKADDR_IN = sizeof (SOCKADDR_IN);

namespace SocketAPI {

    /**
     * 创建socket描述符
     * @param domain    AF_INET(internet socket), AF_UNIX(internal socket), ...
     * @param type      SOCK_STREAM(TCP), SOCK_DGRAM(UDP), ...
     * @param protocol  0
     * @return          socket descriptor
     */
    SOCKET socket_ex(int domain, int type, int protocol);

    /**
     * 将socket绑定到对应的地址
     * @param s         socket descriptor
     * @param addr      socket address structure ( normally struct sockaddr_in )
     * @param addrlen   length of socket address structure
     * @return          true:SUCCESS false:FAIL
     */
    bool bind_ex(SOCKET s, const struct sockaddr* addr, unsigned int addrlen);

    /**
     * 尝试连接地址
     * @param s         socket descriptor
     * @param name      socket address structure
     * @param namelen   length of socket address structure
     * @return          true:SUCCESS false:FAIL
     */
    bool connect_ex(SOCKET s, const struct sockaddr* name, unsigned int namelen);

    /**
     * server socket 监听
     * @param s         socket descriptor
     * @param backlog   waiting queue length
     * @return          true:SUCCESS false:FAIL
     */
    bool listen_ex(SOCKET s, unsigned int backlog);

    /**
     * 连接建立成功
     * @param s         socket descriptor
     * @param addr      socket address structure
     * @param addrlen   length of socket address structure
     * @return
     */
    SOCKET accept_ex(SOCKET s, struct sockaddr* addr, unsigned int* addrlen);

    /**
     * 获得socket状态
     * @param s         socket descriptor
     * @param level     socket option level ( SOL_SOCKET , ... )
     * @param optname   socket option name ( SO_REUSEADDR , SO_LINGER , ... )
     * @param optval    pointer to contain option value
     * @param optlen    length of optval
     * @return
     */
    bool getsockopt_ex(SOCKET s, int level, int optname, void* optval, unsigned int* optlen);

    /**
     * 获得socket状态, 1-5为具体的错误信息
     * @param s
     * @param s         socket descriptor
     * @param level     socket option level ( SOL_SOCKET , ... )
     * @param optname   socket option name ( SO_REUSEADDR , SO_LINGER , ... )
     * @param optval    pointer to contain option value
     * @param optlen    length of optval
     * @return          0:SUCCESS 1,2,3,4,5：ERRORNO
     */
    unsigned int getsockopt_ex2(SOCKET s, int level, int optname, void* optval, unsigned int* optlen);

    /**
     * 设置socket状态
     * @param s         socket descriptor
     * @param level     socket option level ( SOL_SOCKET , ... )
     * @param optname   socket option name ( SO_REUSEADDR , SO_LINGER , ... )
     * @param optval    pointer to contain option value
     * @param optlen    length of optval
     * @return          true:SUCCESS false:FAIL
     */
    bool setsockopt_ex(SOCKET s, int level, int optname, const void* optval, unsigned int optlen);

    /**
     * 发送消息
     * @param s         socket descriptor
     * @param buf       input buffer
     * @param len       input data length
     * @param flags     send flag (MSG_OOB,MSG_DONTROUTE)
     * @return          length of bytes sent
     */
    unsigned int send_ex(SOCKET s, const void* buf, unsigned int len, unsigned int flags);

    /**
     * 发送消息
     * @param s
     * @param s         socket descriptor
     * @param buf       input buffer
     * @param len       input data length
     * @param flags     send flag (MSG_OOB,MSG_DONTROUTE)
     * @return          length of bytes sent
     * @param to        to addr
     * @param tolen     to addr length
     * @return
     */
    unsigned int sendto_ex(SOCKET s, const void* buf, unsigned int len, unsigned int flags, const struct sockaddr* to, int tolen);

    /**
     * 接收数据
     * @param s         socket descriptor
     * @param buf       input buffer
     * @param len       input data length
     * @param flags     send flag (MSG_OOB,MSG_DONTROUTE)
     * @return          length of bytes received
     */
    unsigned int recv_ex(SOCKET s, void* buf, unsigned int len, unsigned int flags);

    /**
     * 接收数据
     * @param s         socket descriptor
     * @param buf       input buffer
     * @param len       input data length
     * @param flags     send flag (MSG_OOB,MSG_DONTROUTE)
     * @param from      from
     * @param fromlen   from addr length
     * @return
     */
    unsigned int recvfrom_ex(SOCKET s, void* buf, unsigned int len, unsigned int flags, struct sockaddr* from, unsigned int* fromlen);

    /**
     * 关闭socket
     * @param s         socket descriptor
     * @return          true:SUCCESS false:FAIL
     */
    bool closesocket_ex(SOCKET s);

    /**
     * 非阻塞 WIN32 适用
     * @param s         socket descriptor
     * @param cmd
     * @param argp
     * @return
     */
    bool ioctlsocket_ex(SOCKET s, long cmd, unsigned long* argp);

    /**
     * 获得socket是否是阻塞的
     * @param s         socket descriptor
     * @return          true 非阻塞, false 阻塞
     */
    bool getsocketnonblocking_ex(SOCKET s);

    /**
     *  设置socket为阻塞或非阻塞
     * @param s         file descriptor
     * @param on        true 非阻塞, false 阻塞
     * @return          true:SUCCESS false:FAIL
     */
    bool setsocketnonblocking_ex(SOCKET s, bool on);

    /**
     * 查看socket还有多少未处理数据
     * @param s         ocket descriptor
     * @return          amount of data in socket input buffer
     */
    unsigned int availablesocket_ex(SOCKET s);

    /**
     * 指定方案关闭socket
     * @param s         socket descriptor
     * @param how       how to close ( all , send , receive )
     * @return          true:SUCCESS false:FAIL
     */
    bool shutdown_ex(SOCKET s, unsigned int how);

    /**
     * system call for I/O multiplexing
     * @param maxfdp1
     * @param readset
     * @param writeset
     * @param exceptset
     * @param timeout
     * @return          positive count of ready descriptors
     */
    int select_ex(int maxfdp1, fd_set* readset, fd_set* writeset, fd_set* exceptset, struct timeval* timeout);


}; //end of namespace



#endif	/* CN_VICKY__SOCKETAPI_H */