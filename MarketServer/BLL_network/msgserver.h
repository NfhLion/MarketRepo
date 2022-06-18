#ifndef MSGSERVER_H
#define MSGSERVER_H

#include "tcpserver.h"
#include "msgsocket.h"

#define MSG_PORT        10240
#define SOCKETLIST_NUM  15000

typedef MsgSocket* MsgSocketList;

typedef struct MsgServer{

    TcpServer *m_tcpServer;

    MsgSocketList m_msgSocketList[SOCKETLIST_NUM];                      //用来存储msgsocket

}MsgServer;

void MsgServerCreate(MsgServer *msgserver, EventLoop *loop, ThreadPool *pool, short port);
void MsgServerStop(MsgServer *msgserver);

void NewTcpConnection(MsgServer *msgserver, TcpSocket *socket);                         //由TcpSocket来回调

void NewMsgSocketConnect(void *arg, MsgSocket *msgsocket);                   //由MsgSocket来回调
int SearchMsgSocketSend(void *arg, char *id, int cmd, char *msg);           //由MsgScoket来回调

#endif // MSGSERVER_H
