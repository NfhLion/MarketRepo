#ifndef EVENTLOOP_H
#define EVENTLOOP_H

#include <sys/socket.h>
#include <sys/types.h>
#include <fcntl.h>
#include <errno.h>
#include <arpa/inet.h>

#include "tcpserver.h"
#include "epoller.h"
#include "handler.h"

#define SERVER_NUM 100

typedef Handler* HandlerList;

typedef struct EventLoop{

    int server_list[SERVER_NUM];

    Epoller myepoller;
    HandlerList m_handlerList[EPOLLEVENTS_NUM];                         //存放server和socket

}EventLoop;


int IsListenFd(EventLoop *loop, int fd);                                //判断是否是serverFD

void SetNonBlocking(int fd);                                            //设置非阻塞
int RegisterServerEvent(EventLoop *loop, TcpServer *server);           //注册server
int RegisterSocketEvent(EventLoop *loop, TcpSocket *socket);           //注册Socket

void EventLoopCreate(EventLoop *loop);                                  //创建反应堆
void EventLoopRun(EventLoop *loop);                                     //反应堆运行
void EventLoopClose(EventLoop *loop);                                   //反应堆停止

void HandleEvents(EventLoop *loop, int size);                           //事件控制器


#endif // EVENTLOOP_H
