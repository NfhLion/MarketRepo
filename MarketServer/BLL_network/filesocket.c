#include "filesocket.h"


void FileSocketCreate(FileSocket *filesocket, TcpSocket *socket)
{
    memset(filesocket->m_ID, 0, 15);
    filesocket->m_fileQueue_first = NULL;
    filesocket->m_fileQueue_tail = NULL;
    filesocket->m_tcpSocket = socket;
    filesocket->m_tcpSocket->RecvFunc = RecvFileMsg;                 //设置TcpSocket的RecvFunc的回调函数为本文件的RecvMsg
    filesocket->m_tcpSocket->SocketClose = FileSocketClose;    //设置TcpSocket的FileSocketClose的回调函数为本文件的FileSocketClose
    filesocket->m_tcpSocket->arg = filesocket;                    //设置TcpSocket的arg参数
}

void FileSocketClose(FileSocket *filesocket)
{
    if(filesocket != NULL){
        free(filesocket);
        filesocket = NULL;
    }
}

void RecvFileMsg(FileSocket *filesocket, char *msg)
{
    //将tcpsocket处读到的数据加到消息队列里
    FileQueueN *node = (FileQueueN *)malloc(sizeof(FileQueueN));
    memset(node, 0, sizeof(FileQueueN));
    strcpy(node->buf, filesocket->m_tcpSocket->read_buf);
    LL_ADD(node, filesocket->m_fileQueue_first, filesocket->m_fileQueue_tail);

    //创建任务并将其加入到线程池
    NJob *job = JobNodeCreate(ParseClientFileAsk, (void *)filesocket);
    ThreadPoolPush(filesocket->m_tcpSocket->pool, job);
}

void SendFileMessage(FileSocket *filesocket, int cmd, char *msg)
{

}

void ParseClientFileAsk(FileSocket *filesocket)
{
    //取出数据
    FileQueueN *qnode = filesocket->m_fileQueue_first;
    if(qnode == NULL)
    {
        printf("m_msgQueue is NULL\n");
        exit(1) ;
    }
    char msg[BUFSIZ] = "";
    strcpy(msg, qnode->buf);
    LL_REMOVE(qnode, filesocket->m_fileQueue_first, filesocket->m_fileQueue_tail);
    free(qnode);

    //解析数据
    int cmd;
    cJSON *json = cJSON_Parse(msg);
    cJSON *node = NULL;
    if(cJSON_HasObjectItem(json, "type")){
        node = cJSON_GetObjectItem(json, "type");
        if(node->type == cJSON_Number){
            cmd = node->valueint;

            switch(cmd)
            {
            case CMD_FileUserID_E:      SetUserID(filesocket, msg); break;
            case CMD_UploadFile_G:      UploadFile(filesocket, msg); break;


            case CMD_GetFileHead_I:     SendFileInfo(filesocket, msg); break;
            }
        }
    }
    cJSON_Delete(json);

    return ;
}

///设置新链接的ID
void SetUserID(FileSocket *filesocket, char *msg)
{
    cJSON *node = NULL;
    cJSON *json = cJSON_Parse(msg);                 //解析json数据

    node = cJSON_GetObjectItem(json, "id");
    if(node->type == cJSON_String){
        //设置其链接到文件服务器的ID和将其加到fileSocketList里
        strcpy(filesocket->m_ID, node->valuestring);
        filesocket->FileSocketConnect(filesocket->arg, filesocket);
    }

    cJSON_Delete(json);
    printf("set m_ID is [%s]\n", node->valuestring);

    return ;
}

///下载文件
void UploadFile(FileSocket *filesocket, char *msg)
{
    cJSON *json = cJSON_Parse(msg);
    cJSON *n_path = NULL;
    cJSON *n_id = NULL;
    n_path = cJSON_GetObjectItem(json, "path");
    n_id = cJSON_GetObjectItem(json, "com_id");


    //客户端传过来的服务器文件路径
    char path[70] = "";     //服务器打开文件使用
    char cli_path[70] = "";
    strcpy(path, n_path->valuestring);
    strcpy(cli_path, n_path->valuestring);

    //获取文件大小
    struct stat statbuf;
    stat(path, &statbuf);
    int size = statbuf.st_size;

    //获取文件名
    int i = 0;
    char *p=strrchr(cli_path, '/');     //倒序查找字符出现的位置，返回地址
    while(*(p + i + 1) != '\0'){
        cli_path[i] = *(p + i + 1);
        i++;
    }
    cli_path[i] = '\0';

    //新的json数据，组装文件头部信息
    cJSON *obj = cJSON_CreateObject();
    cJSON_AddNumberToObject(obj, "type", CMD_UploadFile_G);
    cJSON_AddStringToObject(obj, "com_id", n_id->valuestring);
    cJSON_AddStringToObject(obj, "new_filename", cli_path);
    cJSON_AddStringToObject(obj, "old_filename", path);
    cJSON_AddNumberToObject(obj, "filesize", size);

    char *str = cJSON_Print(obj);
    memset(msg, 0, BUFSIZ);
    strcpy(msg, str);
    msg[strlen(str) + 1] = "\0";

    free(str);
    cJSON_Delete(obj);
    cJSON_Delete(json);
    //发生文件头部
    SendMsg(filesocket->m_tcpSocket, msg);

    return ;
}

void SendFileInfo(FileSocket *filesocket, char *msg)
{
    cJSON *json = cJSON_Parse(msg);
    cJSON *n_path = NULL;
    n_path = cJSON_GetObjectItem(json, "path");

    //文件路径
    char path[70] = "";
    strcpy(path, n_path->valuestring);

    //发送文件具体内容
    int fd = open(path, O_RDONLY);  //打开文件

    int r_size = 0;
    int w_size = 0;
    do{
        char buf[1024] = "";
        r_size = Read(fd, buf, sizeof(buf));
        printf("r_size:%d\n",r_size);
        //SendMsg(filesocket->m_tcpSocket, buf);
        printf("r_size:%d\n",r_size);
        w_size = Write(filesocket->m_tcpSocket->m_handler.m_fd, buf, r_size);
        printf("w_size:%d\n",w_size);
        usleep(10);
    }while(r_size == 1024);
    usleep(10);
    close(fd);
    printf("发送完毕\n");

    return ;

}
