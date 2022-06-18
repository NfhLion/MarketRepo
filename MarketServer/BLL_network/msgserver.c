#include "msgserver.h"

void MsgServerCreate(MsgServer *msgserver, EventLoop *loop, ThreadPool *pool, short port)
{
    msgserver->m_tcpServer = (TcpServer *)malloc(sizeof(TcpServer));
    if(msgserver->m_tcpServer == NULL){
        printf("malloc m_tcpserver failed!\n");
        exit(1);
    }

    int i = 0;
    for(i = 0; i < SOCKETLIST_NUM; i++){
        msgserver->m_msgSocketList[i] = NULL;
    }

    //设置msgserver->m_tcpServer->NewConnection的回调
    msgserver->m_tcpServer->arg = msgserver;
    msgserver->m_tcpServer->NewConnection = NewTcpConnection;

    ServerCreate(msgserver->m_tcpServer, loop, pool, port);
}


void NewTcpConnection(MsgServer *msgserver, TcpSocket *socket)
{
    MsgSocket *msgsocket = (MsgSocket *)malloc(sizeof(MsgSocket));
    if(msgsocket == NULL)
        exit(1);
    MsgSocketCreate(msgsocket, socket);
    //设置msgsocket->MsgSocketConnect的回调
    msgsocket->arg = (void *)msgserver;
    msgsocket->MsgSocketConnect = NewMsgSocketConnect;
}


void MsgServerStop(MsgServer *msgserver)
{
    ServerStop(msgserver->m_tcpServer);
}

void NewMsgSocketConnect(void *arg, MsgSocket *msgsocket)
{
    MsgServer *msgserver = (MsgServer *)arg;
    if(msgsocket != NULL){
        //根据其文件描述符的数值来设置其在数组中的位置
        int i = msgsocket->m_tcpSocket->m_handler.m_fd;
        msgserver->m_msgSocketList[i] = msgsocket;
        msgsocket->SearchMsgSocketSend = SearchMsgSocketSend;
    }
}

int SearchMsgSocketSend(void *arg, char *id, int cmd, char *msg)
{
    MsgServer *msgserver = (MsgServer *)arg;
    int i = 0;
    for(i = 0; i < SOCKETLIST_NUM; i++){
        if(msgserver->m_msgSocketList[i] != NULL && strcmp(id, msgserver->m_msgSocketList[i]->m_ID) == 0){
            printf("%s\n", msgserver->m_msgSocketList[i]->m_ID);
            SendMessage(msgserver->m_msgSocketList[i], cmd, msg);
            return 1;
        }
    }
    return 0;
}


