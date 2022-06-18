#include "eventloop.h"


void EventLoopCreate(EventLoop *loop)
{
    CreateEpoller(&loop->myepoller);
    int i = 0;
    for(i = 0; i < EPOLLEVENTS_NUM; i++){
        loop->m_handlerList[i] = NULL;
    }

    memset(loop->server_list, -1, sizeof(int)*SERVER_NUM);
    return ;
}

void EventLoopClose(EventLoop *loop)
{

//    close(loop->myepoller.efd);
//    memset(loop->myepoller.myevents, 0, sizeof(struct epoll_event)*EPOLLEVENTS_NUM);
//    free(loop->myepoller.myevents);

//    int i = 0;
//    for(i = 0; i < EPOLLEVENTS_NUM; i++){
//        if(loop->m_handlerList[i] != NULL){
//            free(loop->m_handlerList[i]);
//        }
//    }

    printf("EventLoopClose....\n");
}


//反应堆运行
void EventLoopRun(EventLoop *loop)
{
    while(1){
        int nfd = EpollWait(&loop->myepoller);
        if (nfd < 0) {
            printf("epoll_wait error, exit\n");
            EventLoopClose(loop);
            break;
        }
        HandleEvents(loop, nfd);
    }
}

//事件控制器
void HandleEvents(EventLoop *loop, int size)
{
    int i = 0;
    for (i = 0; i < size; i++){
        int fd = loop->myepoller.myevents[i].data.fd;
        if(IsListenFd(loop, fd) && (loop->myepoller.myevents[i].events & EPOLLIN)){
            loop->m_handlerList[fd]->HandlerSetListen(loop->m_handlerList[fd]->arg);

        }else if(loop->myepoller.myevents[i].events & EPOLLIN){
            loop->m_handlerList[fd]->HandlerReadData(loop->m_handlerList[fd]->arg);

        }else if(loop->myepoller.myevents[i].events & EPOLLOUT){
            loop->m_handlerList[fd]->HandlerWriteData(loop->m_handlerList[fd]->arg);
        }

    }
}

//设置非阻塞
void SetNonBlocking(int fd)
{
    int oldOption = fcntl(fd, F_GETFL);
    int newOption = oldOption | O_NONBLOCK;
    fcntl(fd, F_SETFL, newOption);
    return ;
}

//注册监听事件
int RegisterServerEvent(EventLoop *loop, TcpServer *server)
{
    if(AddEvent(&loop->myepoller, server->m_handler.m_fd, EPOLLIN | EPOLLET)){
        int i = server->m_handler.m_fd;
        if(loop->m_handlerList[i] == NULL && i < EPOLLEVENTS_NUM){

            Handler *handler = (Handler *)malloc(sizeof(Handler));
            if(handler == NULL){
                printf("malloc handler failed\n");
                exit(1);
            }
            server->state = 1;
            handler->m_fd = server->m_handler.m_fd;
            handler->HandlerSetListen = server->m_handler.HandlerSetListen;
            handler->arg = server;
            loop->m_handlerList[server->m_handler.m_fd] = handler;

            loop->server_list[server->m_handler.m_fd] = server->m_handler.m_fd;

            SetNonBlocking(server->m_handler.m_fd);

            return 1;
        }else if(i >= EPOLLEVENTS_NUM){
            printf("Register Server falied: exceed EPOLLEVENTS_NUM!\n");
            exit(1);
        }else{
            printf("Register Server falied: It not is NULL\n");
            exit(1);
        }

    }

    return 0;
}

//注册链接事件
int RegisterSocketEvent(EventLoop *loop, TcpSocket *socket)
{
    if(AddEvent(&loop->myepoller, socket->m_handler.m_fd, EPOLLIN| EPOLLET)){

        int i = socket->m_handler.m_fd;
        if(loop->m_handlerList[i] == NULL && i < EPOLLEVENTS_NUM){

            Handler *handler = (Handler *)malloc(sizeof(Handler));
            if(handler == NULL){
                printf("malloc handler failed\n");
                exit(1);
            }
            socket->state = 1;
            handler->m_fd = socket->m_handler.m_fd;
            handler->arg = socket;
            handler->HandlerReadData = socket->m_handler.HandlerReadData;
            handler->HandlerWriteData = socket->m_handler.HandlerWriteData;
            loop->m_handlerList[socket->m_handler.m_fd] = handler;

            SetNonBlocking(socket->m_handler.m_fd);

            return 1;
        }else if(i >= EPOLLEVENTS_NUM){
            printf("Register socket falied: exceed EPOLLEVENTS_NUM!\n");
            exit(1);
        }else{
            printf("Register socket falied: It not is NULL\n");
            exit(1);
        }
    }

    return 0;
}

int IsListenFd(EventLoop *loop, int fd)
{
    if(fd == loop->server_list[fd]){
        return 1;
    }

    return 0;
}





