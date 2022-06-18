#ifndef THREADPOOL_H
#define THREADPOOL_H

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

#define LL_ADD(item, list_first, list_tail) do{          \
    if(list_first == list_tail &&                        \
        list_tail == NULL && list_first== NULL) {        \
        list_first = item;                               \
        list_tail = item;                                \
    }else{                                               \
        list_tail->next = item;                          \
        item->prev = list_tail;                          \
        list_tail = item;                                \
    }                                                    \
}while(0)

#define LL_REMOVE(item, list_first, list_tail) do{          \
    if (list_tail == list_first){                           \
        list_first = NULL;                                  \
        list_tail = NULL;                                   \
    }else{                                                  \
        list_first = item->next;                            \
        list_first->prev = NULL;                            \
    }                                                       \
    item->prev = item->next = NULL;                         \
}while(0)

struct ThreadPool;

//线程执行队列节点
typedef struct Worker{
    pthread_t threadid;
    struct ThreadPool *pool;
    int shutdown;

    struct Worker *prev;
    struct Worker *next;
}NWorker;


//任务队列节点
typedef struct Job{
    void (*job_func)(void *arg);        //任务回调函数,线程处理函数执行的
    void *arg;

    struct Job *prev;
    struct Job *next;
} NJob;

//线程池，用来管理多个线程和任务的的组件
typedef struct ThreadPool{
    struct Worker *workersList_first;
    struct Worker *workersList_tail;

    struct Job *jobsList_first;
    struct Job *jobsList_tail;

    pthread_mutex_t jobs_mtx;
    pthread_cond_t jobs_cond;

}ThreadPool;

///线程池是一个SDK(软件开发包)
///API
void *WorkerCallback(void *arg);
int ThreadPoolCreate(ThreadPool *pool, int numWorkers);
void ThreadPoolPush(ThreadPool *pool, NJob *job);
void ThreadPoolDestory(ThreadPool *pool);
NJob *JobNodeCreate(void (*jobfunc)(void *arg), void *arg);

#endif // THREADPOOL_H
