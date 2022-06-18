#include "tcpserver.h"
#include "eventloop.h"

//初始化监听socket
void ServerCreate(TcpServer *server, EventLoop *loop, ThreadPool *pool, short port)
{
    //初始化socketlist
    int i = 0;
    for(i = 0; i < SOCKETLIST_NUM; i++){
        server->socket_list[i] = NULL;
    }

    struct sockaddr_in servaddr;
    int lfd = Socket(AF_INET, SOCK_STREAM, 0);
    //一个端口释放后会等待两分钟之后才能再被使用，SO_REUSEADDR是让端口释放后立即就可以被再次使用。
    int reuse_addr = 1;
    if(setsockopt(lfd, SOL_SOCKET, SO_REUSEADDR, &reuse_addr, sizeof(reuse_addr)) == -1)
    {
        exit(1);
    }
    bzero(&servaddr,sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    //inet_pton(AF_INET,ip,&servaddr.sin_addr);
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);//绑定所有网卡所有IP
    servaddr.sin_port = htons(port);
    Bind(lfd,(struct sockaddr*)&servaddr,sizeof(servaddr));

    server->m_loop = loop;
    server->m_pool = pool;
    server->m_port = port;
    server->m_handler.arg = (void *)server;
    server->m_handler.HandlerSetListen = SetAccept;
    server->m_handler.m_fd = lfd;

    RegisterServerEvent(loop, server);

    Listen(lfd, LISTEN_NUM);

    printf("server running:port[%d]\n", port);
    printf("Accept client connect ...\n");
}

//设置监听
void SetAccept(void *arg)
{
    TcpServer *server = (TcpServer *)arg;
    struct sockaddr_in cin;
    socklen_t len = sizeof(cin);
    int cfd;

    if ((cfd = Accept(server->m_handler.m_fd, (struct sockaddr *)&cin, &len)) == -1) {

        return ;
    }

    TcpSocket *socket = (TcpSocket *)malloc(sizeof(TcpSocket));
    if(socket == NULL){
        printf("malloc tcpsocket failed!\n");
        exit(1);
    }
    socket->m_handler.m_fd = cfd;
    server->socket_list[cfd] = socket;
    SocketCreate(socket, server->m_loop, server->m_pool);

    if(RegisterSocketEvent(server->m_loop, socket)){
        server->NewConnection(server->arg, socket);
    }


    printf("new connect [ip: %s, port: %d, cfd: %d]\n",
            inet_ntoa(cin.sin_addr), ntohs(cin.sin_port), cfd-5);
    return ;
}

void ServerStop(TcpServer *server)
{
    int i = 0;
    for(i = 0; i < SOCKETLIST_NUM; i++){
        if(server->socket_list[i]->state = 1){
            close(server->m_handler.m_fd);
        }
        if(server->socket_list[i] != NULL){
            free(server->socket_list[i]);
        }
    }

    close(server->m_handler.m_fd);
}
