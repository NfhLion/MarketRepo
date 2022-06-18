#include "epoller.h"

void CreateEpoller(Epoller *epoller)
{
    epoller->efd = epoll_create(EPOLLEVENTS_NUM);
    if(epoller->efd < 0){
        exit(1);
    }

    epoller->myevents = (struct epoll_event *)malloc(sizeof(struct epoll_event)*EPOLLEVENTS_NUM);
    if(epoller->myevents == NULL){
        printf("malloc epoller->myevents failed!\n");
        exit(1);
    }
    memset(epoller->myevents, 0, sizeof(struct epoll_event)*EPOLLEVENTS_NUM);

    return ;
}

int EpollWait(Epoller *epoller)
{
    return epoll_wait(epoller->efd, epoller->myevents, EPOLLEVENTS_NUM, -1);
}

//添加事件
int AddEvent(Epoller *epoller, int fd, int event)
{
    struct epoll_event epv;
    epv.data.fd = fd;
    epv.events = event;
    return !epoll_ctl(epoller->efd, EPOLL_CTL_ADD, fd, &epv);
}

//修改事件
int MdfEvent(Epoller *epoller, int fd, int event)
{
    struct epoll_event epv;
    epv.data.fd = fd;
    epv.events = event;

    return !epoll_ctl(epoller->efd, EPOLL_CTL_MOD, fd, &epv);
}

//删除事件
int DelEvent(Epoller *epoller, int fd)
{
    return !epoll_ctl(epoller->efd, EPOLL_CTL_DEL, fd, NULL);             //从红黑树 efd 上将 ev->fd 摘除
}






