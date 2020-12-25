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

epollCtrl::epollCtrl(){

}
epollCtrl::~epollCtrl(){

}
int epollCtrl::epoll_create(int size) {

    return 0;
}

int epollCtrl::epoll_wait(int epfd, struct epoll_event *event, int maxevents, int timeout) {

    return 0;
}

int epollCtrl::epoll_ctl(int epfd, int op, int fd, struct epoll_event *event) {

    return 0;
}