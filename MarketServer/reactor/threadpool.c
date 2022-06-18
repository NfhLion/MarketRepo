#include "threadpool.h"

//线程处理函数
void *WorkerCallback(void *arg)
{
    NWorker *worker = (NWorker *)arg;

    while(1)
    {
        pthread_mutex_lock(&worker->pool->jobs_mtx);
        while(worker->pool->jobsList_first == worker->pool->jobsList_tail &&
              worker->pool->jobsList_first == NULL &&
              worker->pool->jobsList_tail == NULL)
        {
            if(worker->shutdown) break;
            pthread_cond_wait(&worker->pool->jobs_cond, &worker->pool->jobs_mtx);
        }

        if(worker->shutdown)
        {
            pthread_mutex_unlock(&worker->pool->jobs_mtx);
            break;
        }

        NJob *job = worker->pool->jobsList_first;
        if(job != NULL)
        {
            LL_REMOVE(job, worker->pool->jobsList_first, worker->pool->jobsList_tail);

        }

        pthread_mutex_unlock(&worker->pool->jobs_mtx);

        if(job == NULL)
        {
            continue;
        }

        printf("pthread PID = [%ld]\n", pthread_self());
        job->job_func(job->arg);
        free(job);
        usleep(1);
    }
    free(worker);
    pthread_exit(NULL);
}

//创建线程池
int ThreadPoolCreate(ThreadPool *pool, int numWorkers)
{
    if(pool == NULL) return 1;
    memset(pool, 0, sizeof(ThreadPool));

    if(numWorkers < 1) numWorkers = 1;

    //这里建议用memcpy，因为...
    pthread_mutex_t blank_mutex = PTHREAD_MUTEX_INITIALIZER;
    memcpy(&pool->jobs_mtx, &blank_mutex, sizeof(pthread_mutex_t));

    pthread_cond_t blank_cond = PTHREAD_COND_INITIALIZER;
    memcpy(&pool->jobs_cond, &blank_cond, sizeof(pthread_cond_t));

    int i = 0;
    for(i = 0; i < numWorkers; i++)
    {
        NWorker *worker = (NWorker *)malloc(sizeof(NWorker));
        if(worker == NULL)
        {
            perror("malloc: ");
            return -1;
        }
        memset(worker, 0, sizeof(NWorker));
        worker->pool = pool;

        int ret = pthread_create(&worker->threadid, NULL, WorkerCallback, worker);
        if(ret > 0)
        {
            perror("pthread_create: ");
            free(worker);
            return -1;
        }

        LL_ADD(worker, pool->workersList_first, pool->workersList_tail);
    }

    return 0;
}

//向线程池中增添任务
void ThreadPoolPush(ThreadPool *pool, NJob *job)
{
    pthread_mutex_lock(&pool->jobs_mtx);
    LL_ADD(job, pool->jobsList_first, pool->jobsList_tail);
    pthread_mutex_unlock(&pool->jobs_mtx);
    pthread_cond_signal(&pool->jobs_cond);
}

//关闭线程池
void ThreadPoolDestory(ThreadPool *pool)
{
    NWorker *worker = NULL;
    NJob *job = NULL;
    for(worker = pool->workersList_first; worker != NULL; worker = worker->next)
    {
        worker->shutdown = 1;      //让线程自杀
    }

    pthread_mutex_lock(&pool->jobs_mtx);

    pthread_cond_broadcast(&pool->jobs_cond);       //唤醒全部线程全部自杀

    pthread_mutex_unlock(&pool->jobs_mtx);

    for(worker = pool->workersList_first; worker != NULL; worker = worker->next)
    {
        LL_REMOVE(worker, pool->workersList_first, worker->pool->workersList_tail);
        free(worker);
        worker = NULL;
    }
    pool->workersList_first = NULL;
    pool->workersList_tail = NULL;

    for(job = pool->jobsList_first; job != NULL; job = job->next){
        LL_REMOVE(job, pool->jobsList_first, worker->pool->jobsList_tail);
        free(job);
        job = NULL;
    }
    pool->jobsList_first = NULL;
    pool->jobsList_tail = NULL;

    return ;
}

NJob *JobNodeCreate(void (*jobfunc)(void *), void *arg)
{
    NJob *job = (NJob *)malloc(sizeof(NJob));
    if(job == NULL){
        printf("malloc job failed!\n");
        return NULL;
    }
    job->job_func = jobfunc;
    job->arg = arg;
    job->next = NULL;

    return job;
}
