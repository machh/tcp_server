//
// Created by machenghai on 2020/12/23.
//

#ifndef EPOLL_EPOLLCTRL_H
#define EPOLL_EPOLLCTRL_H


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

};


#endif //EPOLL_EPOLLCTRL_H
