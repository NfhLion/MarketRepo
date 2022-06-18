#ifndef FILESOCKET_H
#define FILESOCKET_H

#include "tcpsocket.h"

#include "control.h"
#include "execsql.h"
#include "connectbase.h"
#include "cJSON.h"

#include <string.h>

//消息队列
typedef struct FileQueue{
    char buf[BUFSIZ];
    struct FileQueue *next;
    struct FileQueue *prev;
}FileQueueN;

typedef struct FileSocket{

    struct FileQueue *m_fileQueue_first;
    struct FileQueue *m_fileQueue_tail;

    //设置并添加msgsocket到MsgServer中的m_msgsocketList。回调MsgServer中的SetMsgSocketConnect
    void *arg;
    void (*FileSocketConnect)(void *arg, struct FileSocket *filesocket);
    //查找需要发送的人的m_ID并发送给他，回调MsgServer中的SearchMsgSocket
    void (*SearchFileSocketSend)(void *arg, char *id, int cmd, char *msg);

    char m_ID[15];                                                  //标示本MsgSocket是哪个用户
    TcpSocket *m_tcpSocket;
}FileSocket;

void FileSocketCreate(FileSocket *filesocket, TcpSocket *socket);
void FileSocketClose(FileSocket *filesocket);       //由TcpSocket来回调
void RecvFileMsg(FileSocket *filesocket, char *msg);                      //读取客户端发的消息
void SendFileMessage(FileSocket *filesocket, int cmd, char *msg);         //组包并发送消息至客户端


void ParseClientFileAsk(FileSocket *filesocket);                   //客户端消息解析接口
void SetUserID(FileSocket *filesocket, char *msg);                 //设置新链接的ID
void UploadFile(FileSocket *filesocket, char *msg);                //下载文件

void SendFileInfo(FileSocket *filesocket, char *msg);              //发送文件


#endif // FILESOCKET_H
