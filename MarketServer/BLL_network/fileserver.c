#include "fileserver.h"

void FileServerCreate(FileServer *fileserver, EventLoop *loop, ThreadPool *pool, short port)
{
    fileserver->m_tcpServer = (TcpServer *)malloc(sizeof(TcpServer));
    if(fileserver->m_tcpServer == NULL){
        printf("malloc m_tcpserver failed!\n");
        exit(1);
    }

    int i = 0;
    for(i = 0; i < SOCKETLIST_NUM; i++){
        fileserver->m_fileSocketList[i] = NULL;
    }

    //设置msgserver->m_tcpServer->NewConnection的回调
    fileserver->m_tcpServer->arg = fileserver;
    fileserver->m_tcpServer->NewConnection = NewTcpConnectionFile;

    ServerCreate(fileserver->m_tcpServer, loop, pool, port);
}

void NewTcpConnectionFile(FileServer *fileserver, TcpSocket *socket)
{
    FileSocket *filesocket = (FileSocket *)malloc(sizeof(FileSocket));
    if(filesocket == NULL)
        exit(1);
    FileSocketCreate(filesocket, socket);
    //设置filesocket->FileSocketConnect的回调
    filesocket->arg = fileserver;
    filesocket->FileSocketConnect = NewFileSocketConnect;
}

void NewFileSocketConnect(FileServer *fileserver, FileSocket *filesocket)
{
    if(filesocket != NULL){
        //根据其文件描述符的数值来设置其在数组中的位置
        int i = filesocket->m_tcpSocket->m_handler.m_fd;
        fileserver->m_fileSocketList[i] = filesocket;
        //filesocket->SearchMsgSocketSend = SearchMsgSocketSend;
        //filesocket->arg = (void *)fileserver;
    }
}
