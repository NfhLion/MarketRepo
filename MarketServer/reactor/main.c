#include "eventloop.h"
#include "threadpool.h"
#include "msgserver.h"

int main()
{
    EventLoop loop;
    ThreadPool pool;
    MsgServer server;

    EventLoopCreate(&loop);
    ThreadPoolCreate(&pool, 10);
    MsgServerCreate(&server, &loop, &pool, MSG_PORT);
    EventLoopRun(&loop);

    //ServerStop(&server);
    EventLoopClose(&loop);
    ThreadPoolDestory(&pool);

    return 0;
}
