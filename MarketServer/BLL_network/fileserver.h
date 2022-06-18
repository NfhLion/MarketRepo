#ifndef FILESERVER_H
#define FILESERVER_H

#include "filesocket.h"
#include "tcpserver.h"

#define SOCKETLIST_NUM 15000
#define FILE_PORT      10241

typedef FileSocket* FileSocketList;

typedef struct FileServer{

    TcpServer *m_tcpServer;

    FileSocketList m_fileSocketList[SOCKETLIST_NUM];                      //用来存储filesocket

}FileServer;

void FileServerCreate(FileServer *fileserver, EventLoop *loop, ThreadPool *pool, short port);
void FileServerStop(FileServer *fileserver);

void NewTcpConnectionFile(FileServer *fileserver, TcpSocket *socket);                         //由TcpSocket来回调

void NewFileSocketConnect(FileServer *fileserver, FileSocket *filesocket);                   //由MsgSocket来回调
//void SearchFileSocketSend(MsgServer *fileserver, char *id, int cmd, char *msg);           //由MsgScoket来回调

#endif // FILESERVER_H
