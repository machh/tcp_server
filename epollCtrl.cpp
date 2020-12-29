//
// Created by machenghai on 2020/12/23.
//

#include "epollCtrl.h"
#include <stdio.h>
#include <errno.h>

// https://blog.csdn.net/libaineu2004/article/details/85526325
// https://blog.csdn.net/haolipengzhanshen/article/details/53761261
// https://blog.csdn.net/haolipengzhanshen/article/details/53745106
// https://blog.csdn.net/idwtwt/article/details/79460217
// https://gitee.com/mirrors/concurrent-tcp-client/blob/master/common.c
// 媒体转发服务器-TCP 在 EPOLL模型中的注意细节 https://blog.csdn.net/liuhongxiangm/article/details/17299025
// Epoll示例 https://blog.csdn.net/yuanchunsi/article/details/72914576


#define SERVER_PORT         (7778)
#define EPOLL_MAX_NUM       (2048)
#define BUFFER_MAX_LEN      (4096)

char buffer[BUFFER_MAX_LEN];

epollCtrl::epollCtrl(){

}
epollCtrl::~epollCtrl(){

}
int epollCtrl::epoll_create(int size) {
    // epoll create
    epfd = epoll_create(EPOLL_MAX_NUM);
    if (epfd < 0) {
        perror("epoll create");
        return -1;
    }

    return epfd;
}

int epollCtrl::epoll_wait(int epfd, struct epoll_event *event, int maxevents, int timeout) {

    int client_fd = 0;
    socklen_t           client_len;
    struct sockaddr_in server_addr;
    struct sockaddr_in client_addr;
    // socket
    int listen_fd = socket(AF_INET, SOCK_STREAM, 0);

    struct epoll_event  *my_events;
    while (1) {
        int active_fds_cnt = epoll_wait(epfd, my_events, EPOLL_MAX_NUM, -1);
        int i = 0;
        for (i = 0; i < active_fds_cnt; i++) {
            // if fd == listen_fd
            if (my_events[i].data.fd == listen_fd) {
                //accept
                client_fd = accept(listen_fd, (struct sockaddr*)&client_addr, &client_len);
                if (client_fd < 0) {
                    perror("accept");
                    continue;
                }

                char ip[20];
                printf("new connection[%s:%d]\n", inet_ntop(AF_INET, &client_addr.sin_addr, ip, sizeof(ip)), ntohs(client_addr.sin_port));

                event->events = EPOLLIN | EPOLLET;
                event->data.fd = client_fd;
                epoll_ctl(epfd, EPOLL_CTL_ADD, client_fd, event);
            }
        }
    }

    return 0;
}

int epollCtrl::epoll_ctl(int epfd, int op, int fd, struct epoll_event *event) {
    //struct epoll_event event, *my_events;
    op = EPOLL_CTL_ADD;
    event->events = EPOLLIN;
    event->data.fd = fd;
    if (epoll_ctl(epfd, op, fd, event) < 0) {
        perror("epoll ctl add listen_fd ");
        return -1;
    }

    return 0;
}