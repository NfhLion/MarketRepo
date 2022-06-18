#ifndef EPOLLER_H
#define EPOLLER_H

#include <sys/types.h>
#include <sys/epoll.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#define EPOLLEVENTS_NUM 15000

typedef struct Epoller{
    int efd;
    struct epoll_event *myevents;
}Epoller;


void CreateEpoller(Epoller *epoller);
int EpollWait(Epoller *epoller);

int AddEvent(Epoller *epoller, int fd, int event);                         //epoll增加事件
int MdfEvent(Epoller *epoller, int fd, int event);                         //epoll修改事件
int DelEvent(Epoller *epoller, int fd);                                    //epoll删除事件

#endif // EPOLLER_H
