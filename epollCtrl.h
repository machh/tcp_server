//
// Created by machh on 2020/12/23.
//

#ifndef EPOLL_EPOLLCTRL_H
#define EPOLL_EPOLLCTRL_H

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include <sys/epoll.h>

/*
 * events : {EPOLLIN, EPOLLOUT, EPOLLPRI,
            EPOLLHUP, EPOLLET, EPOLLONESHOT }
 */



class epollCtrl {

public:
    epollCtrl();
    ~epollCtrl();

    /**
     * @param size 告诉内核监听的数目
     *
     * @returns 返回一个epoll句柄（即一个文件描述符）
     */
    int epoll_create(int size);

    /**
     *
     * @param epfd 用epoll_create所创建的epoll句柄
     * @param event 从内核得到的事件集合
     * @param maxevents 告知内核这个events有多大,
     *             注意: 值 不能大于创建epoll_create()时的size.
     * @param timeout 超时时间
     *     -1: 永久阻塞
     *     0: 立即返回，非阻塞
     *     >0: 指定微秒
     *
     * @returns 成功: 有多少文件描述符就绪,时间到时返回0
     *          失败: -1, errno 查看错误
     */
    int epoll_wait(int epfd, struct epoll_event *event, int maxevents, int timeout);

    /**
     * @param epfd 用epoll_create所创建的epoll句柄
     * @param op 表示对epoll监控描述符控制的动作
     *
     * EPOLL_CTL_ADD(注册新的fd到epfd)
     * EPOLL_CTL_MOD(修改已经注册的fd的监听事件)
     * EPOLL_CTL_DEL(epfd删除一个fd)
     *
     * @param fd 需要监听的文件描述符
     * @param event 告诉内核需要监听的事件
     *
     * @returns 成功返回0，失败返回-1, errno查看错误信息
     */
    int epoll_ctl(int epfd, int op, int fd, struct epoll_event *event);

    int epfd = 0;

};


#endif //EPOLL_EPOLLCTRL_H
