#ifndef TCPSOCKET_H
#define TCPSOCKET_H

#include <sys/socket.h>
#include <sys/types.h>
#include <fcntl.h>
#include <errno.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>

#include "wrap.h"
#include "threadpool.h"
#include "handler.h"

#define MSGSIZE  1024 * 512

typedef struct EventLoop EventLoop;

typedef struct SendQueue{
    char buf[MSGSIZE];
    struct SendQueue *next;
    struct SendQueue *prev;
}SendQueueN;

typedef struct TcpSocket{
    Handler m_handler;

    char read_buf[MSGSIZE];
    char write_buf[MSGSIZE];
    struct EventLoop *loop;
    ThreadPool *pool;
    int state;
    //long last_active;

    struct SendQueue *m_sendQueue_first;
    struct SendQueue *m_sendQueue_tail;

    void *arg;
    void (*RecvFunc)(void *arg, char *msg);
    void (*SocketClose)(void *arg);
}TcpSocket;

void SocketCreate(TcpSocket *socket, EventLoop *loop, ThreadPool *pool);

//发送消息
void SendMsg(TcpSocket *socket, char *msg);

void RecvData(void *arg);                                               //读取数据
void SendData(void *arg);                                               //发送数据

#endif // TCPSOCKET_H
