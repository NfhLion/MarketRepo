#include <stdio.h>

#include "connectbase.h"
#include "msgserver.h"
#include "fileserver.h"
#include "eventloop.h"
#include "threadpool.h"

int main()
{
    MYSQL *mysql = NULL;
    if((mysql = InitDatabase()) != NULL){
        ConnectDatabase(mysql, "localhost", "root", "123456", "market");
    }else{
        return 0;
    }

    //创建反应堆
    EventLoop loop;
    EventLoopCreate(&loop);
    //创建线程池
    ThreadPool pool;
    ThreadPoolCreate(&pool, 5);
    //创建消息服务器
    MsgServer server;
    MsgServerCreate(&server, &loop, &pool, MSG_PORT);
    //创建文件服务器
    FileServer fserver;
    FileServerCreate(&fserver, &loop, &pool, FILE_PORT);

    //反应堆运行
    EventLoopRun(&loop);

    //反应堆停止
    EventLoopClose(&loop);
    CloseDatabase(mysql);

    return 0;
}
