#ifndef HANDLER_H
#define HANDLER_H

typedef struct Handler{
    int m_fd; //文件描述符

    void *arg;
    void (*HandlerSetListen)(void *arg);
    void (*HandlerReadData)(void *arg);
    void (*HandlerWriteData)(void *arg);
}Handler;

#endif // HANDLER_H
