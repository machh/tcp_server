//
// Created by machh on 2020/12/23.
//

#ifndef EPOLL_EPOLLCTRL_H
#define EPOLL_EPOLLCTRL_H

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string>

#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include <sys/epoll.h>

/*
 * events : {EPOLLIN, EPOLLOUT, EPOLLPRI,
            EPOLLHUP, EPOLLET, EPOLLONESHOT }
 */

class EpollContext {
public:
    EpollContext();
    std::string to_string();

    long long _id;
    void *ptr;
    int fd;
    time_t _last_interact_time; // unit is second
    std::string client_ip;
    int _ctx_status;
};

class EpollSocketWatcher {
public:
    virtual int on_accept(EpollContext &epoll_context) = 0;

    virtual int on_readable(int &epollfd, epoll_event &event) = 0;

    /**
     * return :
     * if return value == 1, we will close the connection
     * if return value == 2, we will continue to write
     */
    virtual int on_writeable(EpollContext &epoll_context) = 0;

    virtual int on_close(EpollContext &epoll_context) = 0;

};

enum EpollSocketStatus {
    S_INIT = 0,
    S_RUN = 1,
    S_REJECT_CONN = 2,
    S_STOP = 3
};

class epollCtrl {

public:
    epollCtrl();
    ~epollCtrl();

    /**
     * @param size 告诉内核监听的数目
     * @returns 返回一个epoll句柄（即一个文件描述符）
     */
    int epoll_create(int size);

    /**
     * @param epfd 用epoll_create所创建的epoll句柄
     * @param event 从内核得到的事件集合
     * @param maxevents 告知内核这个events有多大,注意: 值不能大于创建epoll_create()时的size.
     * @param timeout 超时时间
         *     -1: 永久阻塞
         *     0: 立即返回，非阻塞
         *     >0: 指定微秒
     * @returns 成功: 有多少文件描述符就绪,时间到时返回0
     *          失败: -1, errno 查看错误
     */
    int epoll_wait(int epfd, struct epoll_event *event, int maxevents, int timeout);

    /**
     * @param epfd 用epoll_create所创建的epoll句柄
     * @param op 表示对epoll监控描述符控制的动作
         * EPOLL_CTL_ADD(注册新的fd到epfd)
         * EPOLL_CTL_MOD(修改已经注册的fd的监听事件)
         * EPOLL_CTL_DEL(epfd删除一个fd)
     *
     * @param fd 需要监听的文件描述符
     * @param event 告诉内核需要监听的事件
         *struct epoll_event {
         * __uint32_t events;
         *  epoll_data_t data;
        * };
        * events可以是以下几个宏的集合：
            * EPOLLIN ：表示对应的文件描述符可以读（包括对端SOCKET正常关闭）；
            * EPOLLOUT：表示对应的文件描述符可以写；
            * EPOLLPRI：表示对应的文件描述符有紧急的数据可读（这里应该表示有带外数据到来）；
            * EPOLLERR：表示对应的文件描述符发生错误；
            * EPOLLHUP：表示对应的文件描述符被挂断；
            * EPOLLET： 将EPOLL设为边缘触发(Edge Triggered)模式，这是相对于水平触发(Level Triggered)来说的。
            * EPOLLONESHOT：只监听一次事件，当监听完这次事件之后，如果还需要继续监听这个socket的话，需要再次把这个socket加入到EPOLL队列里
     *
     * @returns 成功返回0，失败返回-1, errno查看错误信息
     */
    int epoll_ctl(int epfd, int op, int fd, struct epoll_event *event);

    int start_epoll();
    int stop_epoll();
    bool is_run();
    int start_event_loop();

public:
    int epfd = 0;

};


#endif //EPOLL_EPOLLCTRL_H
