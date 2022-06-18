#ifndef TCPSERVER_H
#define TCPSERVER_H

#include <sys/socket.h>
#include <sys/types.h>
#include <fcntl.h>
#include <errno.h>
#include <arpa/inet.h>

#include "wrap.h"
#include "threadpool.h"
#include "tcpsocket.h"
#include "handler.h"

#define LISTEN_NUM      125
#define SOCKETLIST_NUM  15000

typedef struct EventLoop EventLoop;

typedef TcpSocket* TcpSocketList;

typedef struct TcpServer{

    Handler m_handler;

    int m_port;
    struct EventLoop *m_loop;
    ThreadPool *m_pool;
    int state;

    TcpSocketList socket_list[SOCKETLIST_NUM];

    void *arg;
    void (*NewConnection)(void *arg, TcpSocket *socket);

}TcpServer;


void ServerCreate(TcpServer *server, EventLoop *loop, ThreadPool *pool, short port);     //初始化监听socket
void SetAccept(void *arg);                                                               //设置监听
void ServerStop(TcpServer *server);                                                      //停止监听

#endif // TCPSERVER_H
