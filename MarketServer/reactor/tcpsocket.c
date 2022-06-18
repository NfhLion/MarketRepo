#include "tcpsocket.h"
#include "eventloop.h"


void SocketCreate(TcpSocket *socket, EventLoop *loop, ThreadPool *pool)
{
    socket->m_sendQueue_first = NULL;
    socket->m_sendQueue_tail = NULL;
    socket->m_handler.arg = socket;
    socket->m_handler.HandlerReadData = RecvData;
    socket->m_handler.HandlerWriteData = SendData;
    socket->loop = loop;
    socket->pool = pool;
}


//读数据
void RecvData(void *arg)
{
    TcpSocket *socket = (TcpSocket *)arg;
    memset(socket->read_buf, 0, MSGSIZE);

    int len;
    len = Read(socket->m_handler.m_fd, socket->read_buf, MSGSIZE);

    printf("client[%d] Recv msg : %s\n", socket->m_handler.m_fd-5, socket->read_buf);
    if (len > 0) {
        socket->RecvFunc(socket->arg, socket->read_buf);

    } else if (len == 0) {
        DelEvent(&socket->loop->myepoller, socket->m_handler.m_fd);
        printf("close [%d]client...r0\n", socket->m_handler.m_fd-5);
        Close(socket->m_handler.m_fd);
        socket->state = 0;

        free(socket->loop->m_handlerList[socket->m_handler.m_fd]);          //释放m_handlerList[i]的空间
        socket->loop->m_handlerList[socket->m_handler.m_fd] = NULL;

        socket->SocketClose(socket->arg);                                   //释放其对应的MsgSocket/FileSocket

        free(socket);                                                       //释放socket_List[i]的空间
        socket = NULL;        
    } else if(len < 0){
        DelEvent(&socket->loop->myepoller, socket->m_handler.m_fd);
        printf("close [%d]client...rd\n", socket->m_handler.m_fd-5);
        Close(socket->m_handler.m_fd);
        socket->state = 0;

        free(socket->loop->m_handlerList[socket->m_handler.m_fd]);          //释放m_handlerList[i]的空间
        socket->loop->m_handlerList[socket->m_handler.m_fd] = NULL;

        socket->SocketClose(socket->arg);                                //释放其对应的MsgSocket/FileSocket

        free(socket);                                                       //释放socket_List[i]的空间
        socket = NULL; 
    }

    return ;
}

//写数据
void SendData(void *arg)
{
    TcpSocket *socket = (TcpSocket *)arg;

    memset(socket->write_buf, 0, MSGSIZE);
    SendQueueN *node = socket->m_sendQueue_first;
    if(node == NULL){
        printf("m_sendQueue is NULL\n");
        exit(1) ;
    }
    strcpy(socket->write_buf, node->buf);
    LL_REMOVE(node, socket->m_sendQueue_first, socket->m_sendQueue_tail);
    free(node);

    int nwrite = 0;
    printf("server write buf: %s\n", socket->write_buf);
    while(nwrite != strlen(socket->write_buf))
    {
        nwrite += Write(socket->m_handler.m_fd, socket->write_buf+nwrite, strlen(socket->write_buf)-nwrite);
        if(nwrite == -1)
        {
            DelEvent(&socket->loop->myepoller, socket->m_handler.m_fd);
            printf("close [%d]client...w\n", socket->m_handler.m_fd-5);
            Close(socket->m_handler.m_fd);
            socket->state = 0;

            free(socket->loop->m_handlerList[socket->m_handler.m_fd]);          //释放m_handlerList[i]的空间
            socket->loop->m_handlerList[socket->m_handler.m_fd] = NULL;

            socket->SocketClose(socket->arg);                                //释放其对应的MsgSocket/FileSocket

            free(socket);                                                       //释放socket_List[i]的空间
            socket = NULL;

            return ;
        }
    }


    MdfEvent(&socket->loop->myepoller, socket->m_handler.m_fd, EPOLLIN | EPOLLET);
    return ;
}

void SendMsg(TcpSocket *socket, char *msg)
{
    SendQueueN *node = (SendQueueN *)malloc(sizeof(SendQueueN));
    if(node == NULL)
        exit(1);
    memset(node, 0, sizeof(SendQueueN));
    strcpy(node->buf, msg);
    LL_ADD(node, socket->m_sendQueue_first, socket->m_sendQueue_tail);

    MdfEvent(&socket->loop->myepoller, socket->m_handler.m_fd, EPOLLOUT | EPOLLET);
}


