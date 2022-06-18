#include "msgsocket.h"

void MsgSocketCreate(MsgSocket *msgsocket, TcpSocket *socket)
{
    memset(msgsocket->m_ID, 0, 20);
    memset(msgsocket->m_Friend, 0, MSGSIZE);
    msgsocket->m_msgQueue_first = NULL;
    msgsocket->m_msgQueue_tail = NULL;
    msgsocket->m_tcpSocket = socket;
    msgsocket->m_tcpSocket->RecvFunc = RecvMsg;                 //设置TcpSocket的RecvFunc的回调函数为本文件的RecvMsg
    msgsocket->m_tcpSocket->SocketClose = MsgSocketClose;    //设置TcpSocket的SocketClose的回调函数为本文件的MsgSocketClose
    msgsocket->m_tcpSocket->arg = msgsocket;                    //设置TcpSocket的arg参数
}

///断开链接以后清除在m_msgSocketList中的MsgSocket
void MsgSocketClose(MsgSocket *msgsocket)
{
    if(msgsocket != NULL){
        ExecSql exec_sql;
        InitExecSql(&exec_sql, GetDatebase());
        if(UpdateUserState(&exec_sql, msgsocket->m_ID, "离线")){


            //通知我的好友我下线了
            char msg[MSGSIZE] = "";
            strcpy(msg, msgsocket->m_Friend);
            memset(msgsocket->m_Friend, 0, MSGSIZE);
            cJSON *json = cJSON_Parse(msg);                 //解析json数据
            cJSON *node = cJSON_GetObjectItem(json, "array");
            if(node != NULL){
                cJSON *obj = NULL;              //新的json数据
                if(node->type == cJSON_Array){
                    //获取数组元素的个数
                    int size = cJSON_GetArraySize(node);

                    int i;
                    cJSON *tnode = NULL;
                    char search_id[20] = "";
                    for(i = 0; i < size; i++){
                        //获取数组里的每一个值
                        obj = cJSON_CreateObject();
                        tnode = cJSON_GetArrayItem(node, i);
                        cJSON *snode = cJSON_GetObjectItem(tnode, "id");
                        if(snode->type == cJSON_String){
                            cJSON_AddStringToObject(obj, "state", "离线");
                            cJSON_AddStringToObject(obj, "id", msgsocket->m_ID);
                            cJSON_AddNumberToObject(obj, "result", CMD_Sucess_Y);
                            memset(search_id, 0, 20);
                            strcpy(search_id, snode->valuestring);
                        }else{
                            cJSON_AddNumberToObject(obj, "result", CMD_Failed_Z);
                        }

                        char *str = cJSON_Print(obj);
                        memset(msg, 0, MSGSIZE);
                        strcpy(msg, str);

                        free(str);
                        cJSON_Delete(obj);
                        obj = NULL;
                        snode = NULL;
                        tnode = NULL;
                        msgsocket->SearchMsgSocketSend(msgsocket->arg, search_id, ORD_UserOffLine, msg);
                    }
                }
            }
            cJSON_Delete(json);
            free(msgsocket);
            msgsocket = NULL;
        }
    }
    return ;
}

void RecvMsg(MsgSocket *msgsocket, char *msg)
{
    //将tcpsocket处读到的数据加到消息队列里
    MsgQueueN *node = (MsgQueueN *)malloc(sizeof(MsgQueueN));
    memset(node, 0, sizeof(MsgQueueN));
    strcpy(node->buf, msgsocket->m_tcpSocket->read_buf);
    LL_ADD(node, msgsocket->m_msgQueue_first, msgsocket->m_msgQueue_tail);

    //创建任务并将其加入到线程池
    NJob *job = JobNodeCreate(ParseClientAsk, (void *)msgsocket);
    ThreadPoolPush(msgsocket->m_tcpSocket->pool, job);
}

///客户端消息解析接口
void ParseClientAsk(MsgSocket *msgsocket)
{
    //取出数据
    MsgQueueN *qnode = msgsocket->m_msgQueue_first;
    if(qnode == NULL)
    {
        printf("m_msgQueue is NULL\n");
        exit(1) ;
    }
    char msg[MSGSIZE];
    memset(msg, 0, MSGSIZE);
    strcpy(msg, qnode->buf);
    LL_REMOVE(qnode, msgsocket->m_msgQueue_first, msgsocket->m_msgQueue_tail);
    free(qnode);

    //解析数据
    int cmd;
    cJSON *json = cJSON_Parse(msg);
    cJSON *node = NULL;
    if(cJSON_HasObjectItem(json, "type")){
        node = cJSON_GetObjectItem(json, "type");
        if(node->type == cJSON_Number){
            cmd = node->valueint;
            cJSON_Delete(json);
            switch(cmd)
            {
            case CMD_UserLogin_C:        ParseUserLogin(msgsocket, msg); break;
            case CMD_UserExit_X:         ParseUserExit(msgsocket, msg); break;
            case ORD_GetMyFriends:       ParseGetMyFriends(msgsocket, msg); break;
            case ORD_UserOnLine:         ParseUserOnLine(msgsocket, msg); break;
            case ORD_SendMsg:            ParseFriendSendMsg(msgsocket, msg); break;
            case ORD_NoReadMsg:          ParseNoReadMsg(msgsocket, msg); break;
            case ORD_SearchFriend:       ParseSearchFriendForID(msgsocket, msg); break;
            case ORD_InsertFriend:       ParseInsertUser(msgsocket, msg); break;
            case ORD_InsertUserResult:   ParseInsertUserResult(msgsocket, msg); break;

            case CMD_AllCommodity_D:     ParseAllCommodity(msgsocket, msg); break;
            case CMD_AllShopInfo_J:      ParseAllShopInfo(msgsocket, msg);  break;
            case CMD_CommodityForShop_K: ParseCommodityForShop(msgsocket, msg); break;
            case CMD_GetRepertoryLocal_L:ParseGetRepertoryLocal(msgsocket, msg); break;
            case CMD_GetRepertoryShip_M: ParseGetRepertoryShip(msgsocket, msg); break;
            case CMD_GetCommodityForID_N:ParseGetCommodityForID(msgsocket, msg); break;
            case CMD_GetShipmentForID_O: ParseShipmentForID(msgsocket, msg); break;
            case CMD_InsertCommodity_P:  ParseInsertCommodity(msgsocket, msg); break;
            case CMD_OKUpdateShipment_Q: ParseUpdateShipmentOK(msgsocket, msg); break;
            case CMD_CanelShipment_S:    ParseCanelShipment(msgsocket, msg); break;
            case CMD_ConfirmShipment_T:  ParseConfirmShipment(msgsocket, msg); break;
            case CMD_GLocalCommodity_U:  ParseGetLocalCommodity(msgsocket, msg); break;
            case CMD_GetMyMemmber:       ParseMemberInfoForID(msgsocket, msg); break;
            case CMD_SearchMember:       ParseSearchMember(msgsocket, msg); break;
            case CMD_InsertMember:       ParseInsertMember(msgsocket, msg); break;
            case CMD_SaleSearchMember:   ParseSaleSearchMember(msgsocket, msg); break;
            case CMD_FinishSale:         ParseFinishSale(msgsocket, msg); break;
            case CMD_InsertShop:         ParseInsertShop(msgsocket, msg); break;
            case CMD_UpdatePassword:     ParseUpdatePassword(msgsocket, msg); break;
            }
        }
    }
    printf("-------------------1-\n");
    return ;
}

///----------------------------------------通用解析------------------------------------------///
///用户登陆信息解析接口
void ParseUserLogin(MsgSocket *msgsocket, char *msg)
{
    cJSON *node1 = NULL;
    cJSON *node2 = NULL;
    cJSON *json = cJSON_Parse(msg);                 //解析json数据

    node1 = cJSON_GetObjectItem(json, "user_id");
    node2 = cJSON_GetObjectItem(json, "user_pswd");

    cJSON *obj = cJSON_CreateObject();  //新的json数据
    if(node1->type == cJSON_String && node2->type == cJSON_String){

        //通过数据库搜索检查账号密码是否正确
        ExecSql exec_sql;
        InitExecSql(&exec_sql, GetDatebase());
        if(CheckUserLogin(&exec_sql, node1->valuestring, node2->valuestring) == 1){
            cJSON *tnode = SelectUserState(&exec_sql, node1->valuestring);
            cJSON *snode = cJSON_GetObjectItem(tnode, "state");
            printf("%s\n", cJSON_Print(snode));
            if(strcmp(snode->valuestring, "离线") == 0){
                cJSON_AddNumberToObject(obj, "result", CMD_Sucess_Y);
                //设置该MsgSocket的ID,并将其加到msgsocketlist里面
                strcpy(msgsocket->m_ID, node1->valuestring);
                if(strlen(msgsocket->m_ID) > 0){
                    msgsocket->MsgSocketConnect(msgsocket->arg, msgsocket);
                }
                //更新其状态
                UpdateUserState(&exec_sql, node1->valuestring, "在线");
            }else{
                cJSON_AddNumberToObject(obj, "result", CMD_Failed_Z);
            }
            cJSON_Delete(tnode);
        }else{
            cJSON_AddNumberToObject(obj, "result", CMD_Failed_Z);
        }
    }

    char *str = cJSON_Print(obj);
    memset(msg, 0, MSGSIZE);
    strcpy(msg, str);
    free(str);

    cJSON_Delete(obj);
    printf("%s\n", msg);
    SendMessage(msgsocket, CMD_UserLogin_C, msg);

    return ;
}

///获取好友的状态
void ParseGetMyFriends(MsgSocket *msgsocket, char *msg)
{
    cJSON *node = NULL;
    cJSON *json = cJSON_Parse(msg);                 //解析json数据

    node = cJSON_GetObjectItem(json, "array");

    cJSON *obj = cJSON_CreateObject();  //新的json数据
    cJSON *arr_json = cJSON_CreateArray();
    cJSON *chr_obj = NULL;
    ExecSql exec_sql;
    InitExecSql(&exec_sql, GetDatebase());
    if(node->type == cJSON_Array){
        //获取数组元素的个数
        int size = cJSON_GetArraySize(node);

        int i;
        cJSON *tnode = NULL;
        for(i = 0; i < size; i++){
            //获取数组里的每一个值
            tnode = cJSON_GetArrayItem(node, i);
            cJSON *snode = cJSON_GetObjectItem(tnode, "id");
            if(snode->type == cJSON_String){
                if((chr_obj = SelectUserState(&exec_sql, snode->valuestring)) != NULL){
                    printf("%s\n",cJSON_Print(chr_obj));
                    cJSON_AddItemToArray(arr_json, chr_obj);
                }
            }
            snode = NULL;
            chr_obj = NULL;
        }

        cJSON_AddNumberToObject(obj, "result", CMD_Sucess_Y);
        cJSON_AddItemToObject(obj, "array", arr_json);
    }else{
        cJSON_AddNumberToObject(obj, "result", CMD_Failed_Z);
    }

    char *str = cJSON_Print(obj);
    memset(msg, 0, MSGSIZE);
    strcpy(msg, str);
    free(str);

    cJSON_Delete(obj);
    printf("%s\n", msg);
    SendMessage(msgsocket, ORD_GetMyFriends, msg);

    return ;
}

///用户退出
void ParseUserExit(MsgSocket *msgsocket, char *msg)
{
    cJSON *node = NULL;
    cJSON *json = cJSON_Parse(msg);                 //解析json数据
    strcpy(msgsocket->m_Friend, msg);

    //发送退出消息
    cJSON *exit_obj = cJSON_CreateObject();  //新的json数据
    cJSON_AddNumberToObject(exit_obj, "result", CMD_Sucess_Y);

    char *str = cJSON_Print(exit_obj);
    memset(msg, 0, MSGSIZE);
    strcpy(msg, str);
    free(str);

    cJSON_Delete(exit_obj);

    printf("%s\n", msg);
    SendMessage(msgsocket, CMD_UserExit_X, msg);

    return ;
}

///用户上线
void ParseUserOnLine(MsgSocket *msgsocket, char *msg)
{
    cJSON *node = NULL;
    cJSON *json = cJSON_Parse(msg);                 //解析json数据

    node = cJSON_GetObjectItem(json, "array");

    cJSON *obj = NULL;  //新的json数据
    if(node->type == cJSON_Array){
        //获取数组元素的个数
        int size = cJSON_GetArraySize(node);

        int i;
        cJSON *tnode = NULL;
        cJSON *snode = NULL;
        char search_id[20] = "";
        for(i = 0; i < size; i++){
            //获取数组里的每一个值
            obj = cJSON_CreateObject();
            tnode = cJSON_GetArrayItem(node, i);
            snode = cJSON_GetObjectItem(tnode, "id");
            if(snode->type == cJSON_String){
                cJSON_AddStringToObject(obj, "state", "在线");
                cJSON_AddStringToObject(obj, "id", msgsocket->m_ID);
                cJSON_AddNumberToObject(obj, "result", CMD_Sucess_Y);
                memset(search_id, 0, 20);
                strcpy(search_id, snode->valuestring);
            }else{
                cJSON_AddNumberToObject(obj, "result", CMD_Failed_Z);
            }
            char *str = cJSON_Print(obj);
            memset(msg, 0, MSGSIZE);
            strcpy(msg, str);
            free(str);
            cJSON_Delete(obj);
            obj = NULL;
            snode = NULL;
            tnode = NULL;
            msgsocket->SearchMsgSocketSend(msgsocket->arg, search_id, ORD_UserOnLine, msg);
        }
    }

    return ;
}

///客户端互发消息
void ParseFriendSendMsg(MsgSocket *msgsocket, char *msg)
{
    cJSON *node_jid = NULL;
    cJSON *node_fmsg = NULL;
    cJSON *json = cJSON_Parse(msg);                 //解析json数据

    node_jid = cJSON_GetObjectItem(json, "jid");
    //组装新的发送包
    char rfmsg[MSGSIZE] = "";
    strcpy(rfmsg, cJSON_Print(cJSON_GetObjectItem(json, "fmsg")));
    cJSON *node_rfmsg = cJSON_Parse(rfmsg);

    //组装msgid
    char msg_id[30] = "MSGR";
    char id_time[20] = "";

    time_t t;
    struct tm *nowtime;
    time(&t);
    nowtime = localtime(&t);
    strftime(id_time,sizeof(id_time),"%Y%m%d%H%M%S",nowtime);
    strcat(msg_id, id_time);

    cJSON_AddStringToObject(node_rfmsg, "msgid", msg_id);//添加到fmsg里面


    ExecSql exec_sql;
    InitExecSql(&exec_sql, GetDatebase());
    cJSON *obj = cJSON_CreateObject();  //新的json数据
    if(node_jid->type == cJSON_String &&
            node_rfmsg->type == cJSON_Object){
        cJSON_AddNumberToObject(obj, "result", CMD_Sucess_Y);
        cJSON_AddStringToObject(obj, "jid", node_jid->valuestring);

        cJSON_AddItemToObject(obj, "fmsg", node_rfmsg);

        char *str = cJSON_Print(obj);
        memset(msg, 0, MSGSIZE);
        strcpy(msg, str);
        free(str);

        if(msgsocket->SearchMsgSocketSend(msgsocket->arg, node_jid->valuestring, ORD_SendMsg, msg)){
            InsertChatMsg(&exec_sql, node_jid->valuestring, cJSON_Print(node_rfmsg), "Y");
        }else{
            InsertChatMsg(&exec_sql, node_jid->valuestring, cJSON_Print(node_rfmsg), "N");
        }
     }else{
        cJSON_AddNumberToObject(obj, "result", CMD_Failed_Z);
     }

    cJSON_Delete(obj);
    printf("msg obj %s\n", msg);

    return;
}

///获取未读的聊天记录
void ParseNoReadMsg(MsgSocket *msgsocket, char *msg)
{
    cJSON *node = NULL;
    cJSON *json = cJSON_Parse(msg);                 //解析json数据

    node = cJSON_GetObjectItem(json, "userid");

    cJSON *obj = cJSON_CreateObject();  //新的json数据
    cJSON *arr_json = NULL;
    if(node->type == cJSON_String){

        ExecSql exec_sql;
        InitExecSql(&exec_sql, GetDatebase());
        if((arr_json = SelectMsgInfoForID(&exec_sql, node->valuestring, "N")) != NULL){
             cJSON_AddNumberToObject(obj, "result", CMD_Sucess_Y);
             cJSON_AddItemToObject(obj, "array", arr_json);
             UpdateMsgInfoForID(&exec_sql, node->valuestring, "N");//更新聊天记录表
        }else{
             cJSON_AddNumberToObject(obj, "result", CMD_Sucess_Y);
             cJSON_AddStringToObject(obj, "array", "null");
        }
    }else{
        cJSON_AddNumberToObject(obj, "result", CMD_Failed_Z);
    }

    char *str = cJSON_Print(obj);
    memset(msg, 0, MSGSIZE);
    strcpy(msg, str);
    free(str);

    cJSON_Delete(obj);
    printf("%s\n", msg);
    SendMessage(msgsocket, ORD_NoReadMsg, msg);

    return ;
}

///根据ID查询好友
void ParseSearchFriendForID(MsgSocket *msgsocket, char *msg)
{
    cJSON *node = NULL;
    cJSON *json = cJSON_Parse(msg);                 //解析json数据

    node = cJSON_GetObjectItem(json, "id");

    cJSON *obj = cJSON_CreateObject();  //新的json数据
    cJSON *obj_json = NULL;
    if(node->type == cJSON_String){

        ExecSql exec_sql;
        InitExecSql(&exec_sql, GetDatebase());
        if((obj_json = SelectFriendForID(&exec_sql, node->valuestring)) != NULL){
             cJSON_AddNumberToObject(obj, "result", CMD_Sucess_Y);
             cJSON_AddItemToObject(obj, "obj", obj_json);
        }else{
             cJSON_AddNumberToObject(obj, "result", CMD_Sucess_Y);
             cJSON_AddStringToObject(obj, "obj", "null");
        }
    }else{
        cJSON_AddNumberToObject(obj, "result", CMD_Failed_Z);
    }

    char *str = cJSON_Print(obj);
    memset(msg, 0, MSGSIZE);
    strcpy(msg, str);
    free(str);

    cJSON_Delete(obj);
    printf("%s\n", msg);
    SendMessage(msgsocket, ORD_SearchFriend, msg);

    return ;
}

///添加好友
void ParseInsertUser(MsgSocket *msgsocket, char *msg)
{
    cJSON *node_userid = NULL;
    cJSON *node_qtype = NULL;
    cJSON *json = cJSON_Parse(msg);                 //解析json数据

    node_userid = cJSON_GetObjectItem(json, "userid");
    node_qtype = cJSON_GetObjectItem(json, "qtype");

    cJSON *obj = cJSON_CreateObject();  //新的json数据
    if(node_userid->type == cJSON_String &&
            node_qtype->type == cJSON_String){

        ExecSql exec_sql;
        InitExecSql(&exec_sql, GetDatebase());
        char strname[100] = "";
        strcpy(strname, msgsocket->m_ID);

        if(strcmp(node_qtype->valuestring, "client") != 0){
            SelectMarketNameForID(&exec_sql, strname);
        }else{

        }

        cJSON_AddNumberToObject(obj, "result", CMD_Sucess_Y);
        cJSON_AddStringToObject(obj, "qid", msgsocket->m_ID);
        cJSON_AddStringToObject(obj, "qname", strname);
        cJSON_AddStringToObject(obj, "qtype", node_qtype->valuestring);

        char *str = cJSON_Print(obj);
        memset(msg, 0, MSGSIZE);
        strcpy(msg, str);
        free(str);
        printf("%s\n", msg);

        msgsocket->SearchMsgSocketSend(msgsocket->arg, node_userid->valuestring, ORD_InsertFriend, msg);
        printf("%s\n", msg);
    }else{
        cJSON_AddNumberToObject(obj, "result", CMD_Failed_Z);
    }
    cJSON_Delete(obj);
    cJSON_Delete(json);

    return ;
}

///添加好友的结果
void ParseInsertUserResult(MsgSocket *msgsocket, char *msg)
{
    cJSON *node_state = NULL;
    cJSON *node_mtype = NULL;
    cJSON *node_qid = NULL;
    cJSON *json = cJSON_Parse(msg);                 //解析json数据

    node_state = cJSON_GetObjectItem(json, "state");
    node_mtype = cJSON_GetObjectItem(json, "mtype");
    node_qid = cJSON_GetObjectItem(json, "qid");

    cJSON *obj = cJSON_CreateObject();  //新的json数据
    if(node_state->type == cJSON_String){
        if(strcmp(node_state->valuestring, "Yes") == 0){
            ExecSql exec_sql;
            InitExecSql(&exec_sql, GetDatebase());
            cJSON *obj_json = NULL;
            if((obj_json = SelectFriendForID(&exec_sql, msgsocket->m_ID)) != NULL){
                cJSON_AddNumberToObject(obj, "result", CMD_Sucess_Y);
                cJSON_AddStringToObject(obj, "state", "Yes");
                cJSON_AddStringToObject(obj, "qtype", node_mtype->valuestring);
                cJSON_AddItemToObject(obj, "obj", obj_json);
            }

            char *str = cJSON_Print(obj);
            memset(msg, 0, MSGSIZE);
            strcpy(msg, str);
            free(str);
            printf("%s\n", msg);

            msgsocket->SearchMsgSocketSend(msgsocket->arg, node_qid->valuestring, ORD_InsertUserResult, msg);
        }else{
            cJSON_AddNumberToObject(obj, "result", CMD_Sucess_Y);
            cJSON_AddStringToObject(obj, "state", "No");
            cJSON_AddStringToObject(obj, "id", msgsocket->m_ID);

            char *str = cJSON_Print(obj);
            memset(msg, 0, MSGSIZE);
            strcpy(msg, str);
            free(str);
            printf("%s\n", msg);

            msgsocket->SearchMsgSocketSend(msgsocket->arg, node_qid->valuestring, ORD_InsertUserResult, msg);
        }
    }else{
        cJSON_AddNumberToObject(obj, "result", CMD_Failed_Z);
    }

    cJSON_Delete(obj);

    return ;
}

///获取单个店铺库存
void ParseCommodityForShop(MsgSocket *msgsocket, char *msg)
{
    cJSON *node = NULL;
    cJSON *json = cJSON_Parse(msg);                 //解析json数据

    node = cJSON_GetObjectItem(json, "sh_id");

    cJSON *obj = cJSON_CreateObject();  //新的json数据
    cJSON *arr_json = NULL;
    if(node->type == cJSON_String){

        ExecSql exec_sql;
        InitExecSql(&exec_sql, GetDatebase());
        arr_json = SelectCommodityForShid(&exec_sql, node->valuestring);
        cJSON_AddNumberToObject(obj, "result", CMD_Sucess_Y);
        cJSON_AddItemToObject(obj, "array", arr_json);
    }else{
        cJSON_AddNumberToObject(obj, "result", CMD_Failed_Z);
    }

    char *str = cJSON_Print(obj);
    memset(msg, 0, MSGSIZE);
    strcpy(msg, str);
    free(str);

    cJSON_Delete(obj);
    printf("%s\n", msg);
    SendMessage(msgsocket, CMD_CommodityForShop_K, msg);

    return ;
}

///修改密码
void ParseUpdatePassword(MsgSocket *msgsocket, char *msg)
{
    cJSON *json = cJSON_Parse(msg);                 //解析json数据

    cJSON *node_id = cJSON_GetObjectItem(json, "id");
    cJSON *node_pass = cJSON_GetObjectItem(json, "password");

    cJSON *obj = cJSON_CreateObject();  //新的json数据

    if(node_id->type == cJSON_String &&
            node_pass->type == cJSON_String){

        ExecSql exec_sql;
        InitExecSql(&exec_sql, GetDatebase());
        if(UpdateUserPassword(&exec_sql, node_id->valuestring, node_pass->valuestring)){
            cJSON_AddNumberToObject(obj, "result", CMD_Sucess_Y);
            cJSON_AddStringToObject(obj, "update", "finish");
        }
    }else{
        cJSON_AddNumberToObject(obj, "result", CMD_Failed_Z);
    }

    char *str = cJSON_Print(obj);
    memset(msg, 0, MSGSIZE);
    strcpy(msg, str);
    free(str);

    cJSON_Delete(obj);
    printf("%s\n", msg);
    SendMessage(msgsocket, CMD_UpdatePassword, msg);

    return ;
}


///-------------------------------------------企业解析-------------------------------------------///

///连锁店获取所有商品请求信息解析
void ParseAllCommodity(MsgSocket *msgsocket, char *msg)
{
    ExecSql exec_sql;
    InitExecSql(&exec_sql, GetDatebase());
    cJSON *obj = cJSON_CreateObject();  //新的json数据
    cJSON *arr_json = NULL;

    if((arr_json = SelectAllCommodity(&exec_sql)) != NULL){
         cJSON_AddNumberToObject(obj, "result", CMD_Sucess_Y);
         cJSON_AddItemToObject(obj, "array", arr_json);
    }else{
         cJSON_AddNumberToObject(obj, "result", CMD_Failed_Z);
    }

    char *str = cJSON_Print(obj);
    memset(msg, 0, MSGSIZE);
    strcpy(msg, str);
    free(str);

    cJSON_Delete(obj);

    printf("%s\n", msg);
    SendMessage(msgsocket, CMD_AllCommodity_D, msg);

    return ;
}

///获取所有店铺的信息
void ParseAllShopInfo(MsgSocket *msgsocket, char *msg)
{
    ExecSql exec_sql;
    InitExecSql(&exec_sql, GetDatebase());
    cJSON *obj = cJSON_CreateObject();  //新的json数据
    cJSON *arr_json = NULL;

    if((arr_json = SelectAllShopInfo(&exec_sql)) != NULL){
         cJSON_AddNumberToObject(obj, "result", CMD_Sucess_Y);
         cJSON_AddItemToObject(obj, "array", arr_json);
    }else{
        cJSON_AddNumberToObject(obj, "result", CMD_Failed_Z);
    }

    char *str = cJSON_Print(obj);
    memset(msg, 0, MSGSIZE);
    strcpy(msg, str);
    free(str);

    cJSON_Delete(obj);

    printf("%s\n", msg);
    SendMessage(msgsocket, CMD_AllShopInfo_J, msg);

    return ;
}

///获取商家的所有商品库存
void ParseGetRepertoryLocal(MsgSocket *msgsocket, char *msg)
{
    cJSON *node = NULL;
    cJSON *json = cJSON_Parse(msg);                 //解析json数据

    node = cJSON_GetObjectItem(json, "mer_id");

    cJSON *obj = cJSON_CreateObject();  //新的json数据
    cJSON *arr_json = NULL;
    if(node->type == cJSON_String){

        ExecSql exec_sql;
        InitExecSql(&exec_sql, GetDatebase());
        if((arr_json = SelectCommodityForShid(&exec_sql, node->valuestring)) != NULL){
             cJSON_AddNumberToObject(obj, "result", CMD_Sucess_Y);
             cJSON_AddItemToObject(obj, "array", arr_json);
        }else{
            cJSON_AddNumberToObject(obj, "result", CMD_Failed_Z);
        }
    }

    char *str = cJSON_Print(obj);
    memset(msg, 0, MSGSIZE);
    strcpy(msg, str);
    free(str);

    cJSON_Delete(obj);
    printf("%s\n", msg);
    SendMessage(msgsocket, CMD_GetRepertoryLocal_L, msg);

    return ;
}

///获取所有调货信息解析接口
void ParseGetRepertoryShip(MsgSocket *msgsocket, char *msg)
{
    ExecSql exec_sql;
    InitExecSql(&exec_sql, GetDatebase());
    cJSON *obj = cJSON_CreateObject();  //新的json数据
    cJSON *arr_json = NULL;

    if((arr_json = SelectAllShipmentInfo(&exec_sql)) != NULL){
         cJSON_AddNumberToObject(obj, "result", CMD_Sucess_Y);
         cJSON_AddItemToObject(obj, "array", arr_json);
    }else{
        cJSON_AddNumberToObject(obj, "result", CMD_Failed_Z);
    }

    char *str = cJSON_Print(obj);
    memset(msg, 0, MSGSIZE);
    strcpy(msg, str);
    free(str);
    cJSON_Delete(obj);

    printf("%s\n", msg);
    SendMessage(msgsocket, CMD_GetRepertoryShip_M, msg);
    printf("%s\n", msg);

    return ;
}

///根据商品ID和num获取店铺解析接口
void ParseGetCommodityForID(MsgSocket *msgsocket, char *msg)
{
    cJSON *node_id = NULL;
    cJSON *node_num = NULL;
    cJSON *json = cJSON_Parse(msg);                 //解析json数据

    node_id = cJSON_GetObjectItem(json, "com_id");
    node_num = cJSON_GetObjectItem(json, "com_num");

    cJSON *obj = cJSON_CreateObject();  //新的json数据
    cJSON *arr_json = NULL;
    if(node_id->type == cJSON_String &&
            node_num->type == cJSON_String){

        ExecSql exec_sql;
        InitExecSql(&exec_sql, GetDatebase());
        if((arr_json = SelectCommodityForComID(&exec_sql, node_id->valuestring, node_num->valuestring)) != NULL){
             cJSON_AddNumberToObject(obj, "result", CMD_Sucess_Y);
             cJSON_AddItemToObject(obj, "array", arr_json);
        }else{
            cJSON_AddNumberToObject(obj, "result", CMD_Failed_Z);
        }
    }

    char *str = cJSON_Print(obj);
    memset(msg, 0, MSGSIZE);
    strcpy(msg, str);
    free(str);

    cJSON_Delete(obj);
    printf("%s\n", msg);
    SendMessage(msgsocket, CMD_GetCommodityForID_N, msg);

    return ;
}

///处理调货请求成功解析接口
void ParseUpdateShipmentOK(MsgSocket *msgsocket, char *msg)
{
    cJSON *node_id = NULL;
    cJSON *node_qid = NULL;
    cJSON *node_fid = NULL;
    cJSON *node_fid_shipment = NULL;
    cJSON *node_fid_residue = NULL;
    cJSON *node_comid = NULL;
    cJSON *node_comnum = NULL;
    cJSON *json = cJSON_Parse(msg);                 //解析json数据

    node_id = cJSON_GetObjectItem(json, "ship_id");
    node_qid = cJSON_GetObjectItem(json, "ship_qid");
    node_fid = cJSON_GetObjectItem(json, "ship_fid");
    node_fid_shipment = cJSON_GetObjectItem(json, "fid_shipment");
    node_fid_residue = cJSON_GetObjectItem(json, "fid_residue");
    node_comid = cJSON_GetObjectItem(json, "ship_comid");
    node_comnum = cJSON_GetObjectItem(json, "ship_comnum");
    char search_id[20] = "";

    cJSON *obj = cJSON_CreateObject();  //新的json数据
    if(node_id->type == cJSON_String &&
            node_qid->type == cJSON_String &&
            node_fid->type == cJSON_String &&
            node_fid_shipment->type == cJSON_String &&
            node_fid_residue->type == cJSON_String &&
            node_comid->type == cJSON_String &&
            node_comnum->type == cJSON_String){

        ExecSql exec_sql;
        InitExecSql(&exec_sql, GetDatebase());
        if(UpdateShipment(&exec_sql, node_id->valuestring, node_fid->valuestring, "调货中") &&
                UpdateGoodsNumForFID(&exec_sql, node_fid->valuestring, node_comid->valuestring
                                     ,node_fid_shipment->valuestring, node_fid_residue->valuestring)){
            cJSON_AddNumberToObject(obj, "result", CMD_Sucess_Y);
            cJSON_AddStringToObject(obj, "state", "调货中");
            strcpy(search_id, node_qid->valuestring);
        }else{
            cJSON_AddNumberToObject(obj, "result", CMD_Failed_Z);
        }
    }
    char *str = cJSON_Print(obj);
    memset(msg, 0, MSGSIZE);
    strcpy(msg, str);

    //供转发用
    char msgs[MSGSIZE] = "";
    memset(msgs, 0, MSGSIZE);
    strcpy(msgs, str);

    free(str);

    cJSON_Delete(obj);
    printf("%s\n", msg);

    SendMessage(msgsocket, CMD_UpdateShipment_R, msg);

    //寻找发送
    msgsocket->SearchMsgSocketSend(msgsocket->arg, search_id, CMD_UpdateShipment_R, msgs);

    return ;
}

///注册店铺
void ParseInsertShop(MsgSocket *msgsocket, char *msg)
{
    cJSON *json = cJSON_Parse(msg);                 //解析json数据

    cJSON *node_id = cJSON_GetObjectItem(json, "id");
    cJSON *node_name = cJSON_GetObjectItem(json, "name");
    cJSON *node_password = cJSON_GetObjectItem(json, "password");
    cJSON *node_phone = cJSON_GetObjectItem(json, "phone");
    cJSON *node_site = cJSON_GetObjectItem(json, "site");
    cJSON *node_time = cJSON_GetObjectItem(json, "time");
    cJSON *node_shtype = cJSON_GetObjectItem(json, "shtype");

    cJSON *obj = cJSON_CreateObject();  //新的json数据
    ExecSql exec_sql;
    InitExecSql(&exec_sql, GetDatebase());
    if(InsertShopToUser(&exec_sql, node_id->valuestring, node_password->valuestring,
                      node_time->valuestring, node_shtype->valuestring) &&
            InsertShopToMarket(&exec_sql, node_id->valuestring, node_name->valuestring,
                                  node_phone->valuestring, node_site->valuestring)){
            cJSON_AddNumberToObject(obj, "result", CMD_Sucess_Y);
            cJSON_AddStringToObject(obj, "id", node_id->valuestring);
            cJSON_AddStringToObject(obj, "name", node_name->valuestring);
            cJSON_AddStringToObject(obj, "type", node_shtype->valuestring);
            cJSON_AddStringToObject(obj, "phone", node_phone->valuestring);
            cJSON_AddStringToObject(obj, "site", node_site->valuestring);
            cJSON_AddStringToObject(obj, "time", node_time->valuestring);
    }else{
        cJSON_AddNumberToObject(obj, "result", CMD_Failed_Z);
    }

    char *str = cJSON_Print(obj);
    memset(msg, 0, MSGSIZE);
    strcpy(msg, str);
    free(str);

    cJSON_Delete(obj);
    printf("%s\n", msg);
    SendMessage(msgsocket, CMD_InsertShop, msg);

    return ;
}

///-----------------------------------------店铺解析----------------------------------------------///
///根据店铺ID获取它的货单
void ParseShipmentForID(MsgSocket *msgsocket, char *msg)
{
    cJSON *node = NULL;
    cJSON *json = cJSON_Parse(msg);                 //解析json数据

    node = cJSON_GetObjectItem(json, "id");

    cJSON *obj = cJSON_CreateObject();  //新的json数据
    cJSON *arr_json = NULL;
    if(node->type == cJSON_String){

        ExecSql exec_sql;
        InitExecSql(&exec_sql, GetDatebase());
        arr_json = SelectShipmentForShid(&exec_sql, node->valuestring);
        cJSON_AddNumberToObject(obj, "result", CMD_Sucess_Y);
        cJSON_AddItemToObject(obj, "array", arr_json);

    }else{
        cJSON_AddNumberToObject(obj, "result", CMD_Failed_Z);
    }

    char *str = cJSON_Print(obj);
    memset(msg, 0, MSGSIZE);
    strcpy(msg, str);
    free(str);

    cJSON_Delete(obj);
    printf("%s\n", msg);
    SendMessage(msgsocket, CMD_GetShipmentForID_O, msg);

    return ;
}

///店铺申请调货解析接口
void ParseInsertCommodity(MsgSocket *msgsocket, char *msg)
{
    cJSON *node_qid = NULL;
    cJSON *node_fid = NULL;
    cJSON *node_comid = NULL;
    cJSON *node_comnum = NULL;
    cJSON *json = cJSON_Parse(msg);                 //解析json数据

    node_qid = cJSON_GetObjectItem(json, "s_qid");
    node_fid = cJSON_GetObjectItem(json, "s_fid");
    node_comid = cJSON_GetObjectItem(json, "s_comid");
    node_comnum = cJSON_GetObjectItem(json, "s_comnum");
    char search_id[20] = "";

    cJSON *obj = cJSON_CreateObject();  //新的json数据
    if(node_qid->type == cJSON_String &&
            node_fid->type == cJSON_String &&
            node_comid->type == cJSON_String &&
            node_comnum->type == cJSON_String){

        ExecSql exec_sql;
        InitExecSql(&exec_sql, GetDatebase());
        if(InsertShipment(&exec_sql, node_qid->valuestring, node_fid->valuestring,
                          node_comid->valuestring, node_comnum->valuestring)){
            cJSON_AddNumberToObject(obj, "result", CMD_Sucess_Y);
            cJSON_AddStringToObject(obj, "state", "审核中");
            strcpy(search_id, node_fid->valuestring);
        }else{
            cJSON_AddNumberToObject(obj, "result", CMD_Failed_Z);
        }
    }

    char *str = cJSON_Print(obj);
    memset(msg, 0, MSGSIZE);
    strcpy(msg, str);

    //供转发用
    char msgs[MSGSIZE] = "";
    memset(msgs, 0, MSGSIZE);
    strcpy(msgs, str);

    free(str);

    cJSON_Delete(obj);
    printf("%s\n", msg);

    SendMessage(msgsocket, CMD_UpdateShipment_R, msg);
    //寻找发送
    msgsocket->SearchMsgSocketSend(msgsocket->arg, search_id, CMD_UpdateShipment_R, msgs);

    return ;
}

///店铺取消调货
void ParseCanelShipment(MsgSocket *msgsocket, char *msg)
{
    cJSON *node_id = NULL;
    cJSON *node_fid = NULL;
    cJSON *json = cJSON_Parse(msg);                 //解析json数据

    node_id = cJSON_GetObjectItem(json, "ship_id");
    node_fid = cJSON_GetObjectItem(json, "ship_fid");
    char search_id[20] = "";

    cJSON *obj = cJSON_CreateObject();  //新的json数据
    if(node_id->type == cJSON_String &&
            node_fid->type == cJSON_String){

        ExecSql exec_sql;
        InitExecSql(&exec_sql, GetDatebase());
        if(UpdateShipment(&exec_sql, node_id->valuestring, node_fid->valuestring, "已取消")){
            cJSON_AddNumberToObject(obj, "result", CMD_Sucess_Y);
            cJSON_AddStringToObject(obj, "state", "已取消");
            strcpy(search_id, node_fid->valuestring);
        }else{
            cJSON_AddNumberToObject(obj, "result", CMD_Failed_Z);
        }
    }

    char *str = cJSON_Print(obj);
    memset(msg, 0, MSGSIZE);
    strcpy(msg, str);

    //供转发用
    char msgs[MSGSIZE] = "";
    memset(msgs, 0, MSGSIZE);
    strcpy(msgs, str);

    free(str);

    cJSON_Delete(obj);
    printf("%s\n", msg);


    SendMessage(msgsocket, CMD_UpdateShipment_R, msg);

    //寻找发送
    msgsocket->SearchMsgSocketSend(msgsocket->arg, search_id, CMD_UpdateShipment_R, msgs);

    return ;
}

///店铺确认收到货
void ParseConfirmShipment(MsgSocket *msgsocket, char *msg)
{
    cJSON *node_id = NULL;
    cJSON *node_qid = NULL;
    cJSON *node_fid = NULL;
    cJSON *node_comid = NULL;
    cJSON *node_comnum = NULL;
    cJSON *json = cJSON_Parse(msg);                 //解析json数据

    node_id = cJSON_GetObjectItem(json, "ship_id");
    node_qid = cJSON_GetObjectItem(json, "ship_qid");
    node_fid = cJSON_GetObjectItem(json, "ship_fid");
    node_comid = cJSON_GetObjectItem(json, "ship_comid");
    node_comnum = cJSON_GetObjectItem(json, "ship_comnum");
    char search_id[20] = "";

    cJSON *obj = cJSON_CreateObject();  //新的json数据
    if(node_id->type == cJSON_String &&
            node_qid->type == cJSON_String &&
            node_fid->type == cJSON_String &&
            node_comid->type == cJSON_String &&
            node_comnum->type == cJSON_String){

        ExecSql exec_sql;
        InitExecSql(&exec_sql, GetDatebase());
        if(UpdateShipment(&exec_sql, node_id->valuestring, node_fid->valuestring, "已收到") &&
                UpdateGoodsNumForQID(&exec_sql, node_qid->valuestring, node_comid->valuestring
                                     ,node_comnum->valuestring)){
            cJSON_AddNumberToObject(obj, "result", CMD_Sucess_Y);
            cJSON_AddStringToObject(obj, "state", "已收到");
            strcpy(search_id, node_fid->valuestring);
        }else{
            cJSON_AddNumberToObject(obj, "result", CMD_Failed_Z);
        }
    }

    char *str = cJSON_Print(obj);
    memset(msg, 0, MSGSIZE);
    strcpy(msg, str);

    //供转发用
    char msgs[MSGSIZE] = "";
    memset(msgs, 0, MSGSIZE);
    strcpy(msgs, str);

    free(str);

    cJSON_Delete(obj);
    printf("%s\n", msg);

    SendMessage(msgsocket, CMD_UpdateShipment_R, msg);

    //寻找发送
    msgsocket->SearchMsgSocketSend(msgsocket->arg, search_id, CMD_UpdateShipment_R, msgs);

    return ;
}

///获取本地商品解析接口
void ParseGetLocalCommodity(MsgSocket *msgsocket, char *msg)
{
    cJSON *node = NULL;
    cJSON *json = cJSON_Parse(msg);                 //解析json数据

    node = cJSON_GetObjectItem(json, "shop_id");

    cJSON *obj = cJSON_CreateObject();  //新的json数据
    cJSON *arr_json = NULL;
    if(node->type == cJSON_String){

        ExecSql exec_sql;
        InitExecSql(&exec_sql, GetDatebase());
        arr_json = SelectCommodityForShid(&exec_sql, node->valuestring);
        cJSON_AddNumberToObject(obj, "result", CMD_Sucess_Y);
        cJSON_AddItemToObject(obj, "array", arr_json);
    }else{
        cJSON_AddNumberToObject(obj, "result", CMD_Failed_Z);
    }


    char *str = cJSON_Print(obj);
    memset(msg, 0, MSGSIZE);
    strcpy(msg, str);
    free(str);

    cJSON_Delete(obj);
    printf("%s\n", msg);
    SendMessage(msgsocket, CMD_GLocalCommodity_U, msg);

    return ;
}

///根据店铺ID获取本地会员信息解析接口
void ParseMemberInfoForID(MsgSocket *msgsocket, char *msg)
{
    cJSON *node = NULL;
    cJSON *json = cJSON_Parse(msg);                 //解析json数据

    node = cJSON_GetObjectItem(json, "shop_id");

    ExecSql exec_sql;
    InitExecSql(&exec_sql, GetDatebase());
    cJSON *obj = cJSON_CreateObject();  //新的json数据
    cJSON *arr_json = NULL;

    if(node->type == cJSON_String){
        arr_json = SelectMemberInfoForID(&exec_sql, node->valuestring);
        cJSON_AddNumberToObject(obj, "result", CMD_Sucess_Y);
        cJSON_AddItemToObject(obj, "array", arr_json);

    }else{
        cJSON_AddNumberToObject(obj, "result", CMD_Failed_Z);
    }



    char *str = cJSON_Print(obj);
    memset(msg, 0, MSGSIZE);
    strcpy(msg, str);
    free(str);

    cJSON_Delete(obj);

    printf("%s\n", msg);
    SendMessage(msgsocket, CMD_GetMyMemmber, msg);

    return ;
}

///查找会员用户信息解析接口
void ParseSearchMember(MsgSocket *msgsocket, char *msg)
{
    cJSON *node = NULL;
    cJSON *json = cJSON_Parse(msg);                 //解析json数据

    node = cJSON_GetObjectItem(json, "str");

    ExecSql exec_sql;
    InitExecSql(&exec_sql, GetDatebase());
    cJSON *obj = cJSON_CreateObject();  //新的json数据
    cJSON *new_obj = NULL;

    if(node->type == cJSON_String){
        new_obj = SelectMemberInfoForStr(&exec_sql, node->valuestring);
        cJSON_AddNumberToObject(obj, "result", CMD_Sucess_Y);
        cJSON_AddItemToObject(obj, "obj", new_obj);

    }else{
        cJSON_AddNumberToObject(obj, "result", CMD_Failed_Z);
    }



    char *str = cJSON_Print(obj);
    memset(msg, 0, MSGSIZE);
    strcpy(msg, str);
    free(str);

    cJSON_Delete(obj);

    printf("%s\n", msg);
    SendMessage(msgsocket, CMD_SearchMember, msg);

    return ;
}

///注册会员
void ParseInsertMember(MsgSocket *msgsocket, char *msg)
{
    cJSON *json = cJSON_Parse(msg);                 //解析json数据

    cJSON *node_name = cJSON_GetObjectItem(json, "name");
    cJSON *node_phone = cJSON_GetObjectItem(json, "phone");
    cJSON *node_shopid = cJSON_GetObjectItem(json, "shopid");
    cJSON *node_time = cJSON_GetObjectItem(json, "time");
    cJSON *node_grade = cJSON_GetObjectItem(json, "grade");
    cJSON *node_total = cJSON_GetObjectItem(json, "total");
    cJSON *node_price = cJSON_GetObjectItem(json, "price");
    cJSON *node_discount = cJSON_GetObjectItem(json, "discount");

    //生成会员ID
    char id[40] = "MEM";
    char id_time[20] = "";

    time_t t;
    struct tm *nowtime;
    time(&t);
    nowtime = localtime(&t);
    strftime(id_time,sizeof(id_time),"%Y%m%d%H%M%S",nowtime);
    strcat(id, id_time);

    ExecSql exec_sql;
    InitExecSql(&exec_sql, GetDatebase());
    cJSON *obj = cJSON_CreateObject();  //新的json数据

    if(InsertMember(&exec_sql, id, node_name->valuestring, node_phone->valuestring,
                    node_shopid->valuestring, node_time->valuestring, node_grade->valuestring,
                    node_total->valuestring, node_price->valuestring, node_discount->valuestring)){

        cJSON_AddNumberToObject(obj, "result", CMD_Sucess_Y);
        cJSON_AddStringToObject(obj, "id", id);
    }else{
        cJSON_AddNumberToObject(obj, "result", CMD_Failed_Z);
    }



    char *str = cJSON_Print(obj);
    memset(msg, 0, MSGSIZE);
    strcpy(msg, str);
    free(str);

    cJSON_Delete(obj);

    printf("%s\n", msg);
    SendMessage(msgsocket, CMD_InsertMember, msg);

    return ;
}

///销售查找会员用户信息解析接口
void ParseSaleSearchMember(MsgSocket *msgsocket, char *msg)
{
    cJSON *node = NULL;
    cJSON *json = cJSON_Parse(msg);                 //解析json数据

    node = cJSON_GetObjectItem(json, "str");

    ExecSql exec_sql;
    InitExecSql(&exec_sql, GetDatebase());
    cJSON *obj = cJSON_CreateObject();  //新的json数据
    cJSON *new_obj = NULL;

    if(node->type == cJSON_String){
        new_obj = SelectMemberInfoForStr(&exec_sql, node->valuestring);
        cJSON_AddNumberToObject(obj, "result", CMD_Sucess_Y);
        cJSON_AddItemToObject(obj, "obj", new_obj);

    }else{
        cJSON_AddNumberToObject(obj, "result", CMD_Failed_Z);
    }



    char *str = cJSON_Print(obj);
    memset(msg, 0, MSGSIZE);
    strcpy(msg, str);
    free(str);

    cJSON_Delete(obj);

    printf("%s\n", msg);
    SendMessage(msgsocket, CMD_SaleSearchMember, msg);

    return ;
}

///销售结算解析接口
void ParseFinishSale(MsgSocket *msgsocket, char *msg)
{
    cJSON *json = cJSON_Parse(msg);                 //解析json数据
    cJSON *node_gid = cJSON_GetObjectItem(json, "gid");
    cJSON *node_cid = cJSON_GetObjectItem(json, "cid");
    cJSON *node_mid = cJSON_GetObjectItem(json, "mid");
    cJSON *node_num = cJSON_GetObjectItem(json, "num");
    cJSON *node_price = cJSON_GetObjectItem(json, "price");
    cJSON *node_state = cJSON_GetObjectItem(json, "state");
    cJSON *node_time = cJSON_GetObjectItem(json, "time");
    cJSON *node_mode = cJSON_GetObjectItem(json, "mode");
    cJSON *node_value = cJSON_GetObjectItem(json, "value");
    cJSON *node_memid = cJSON_GetObjectItem(json, "mem_id");
    cJSON *node_total = cJSON_GetObjectItem(json, "total");
    cJSON *node_memprice = cJSON_GetObjectItem(json, "mem_price");

    ExecSql exec_sql;
    InitExecSql(&exec_sql, GetDatebase());
    cJSON *obj = cJSON_CreateObject();  //新的json数据

    char id[40] = "FM";
    char id_time[20] = "";

    time_t t;
    struct tm *nowtime;
    time(&t);
    nowtime = localtime(&t);
    strftime(id_time,sizeof(id_time),"%Y%m%d%H%M%S",nowtime);
    strcat(id, id_time);

    if(InsertForm(&exec_sql, id, node_gid->valuestring, node_cid->valuestring, node_mid->valuestring,
                  node_num->valuestring, node_price->valuestring, node_state->valuestring,
                  node_time->valuestring, node_mode->valuestring, node_value->valuestring)&&
            UpdateGoodsNum(&exec_sql, node_mid->valuestring, node_gid->valuestring, node_num->valuestring)){
        cJSON_AddNumberToObject(obj, "result", CMD_Sucess_Y);
        cJSON_AddStringToObject(obj, "id", id);
        if(strcmp(node_memid->valuestring, "null") == 0){
            cJSON_AddStringToObject(obj, "mem_id", "null");
        }else
        {
            UpdateMember(&exec_sql, node_memid->valuestring, node_total->valuestring, node_memprice->valuestring);
            cJSON_AddStringToObject(obj, "mem_id", node_memid->valuestring);
        }
    }else{
        cJSON_AddNumberToObject(obj, "result", CMD_Failed_Z);
    }

    char *str = cJSON_Print(obj);
    memset(msg, 0, MSGSIZE);
    strcpy(msg, str);
    free(str);

    cJSON_Delete(obj);

    printf("%s\n", msg);
    SendMessage(msgsocket, CMD_FinishSale, msg);
    return ;
}

///组包并发送消息至客户端
void SendMessage(MsgSocket *msgsocket, int cmd, char *msg)
{
    cJSON *json = cJSON_Parse(msg);                 //解析json数据
    cJSON *obj = cJSON_CreateObject();              //新的json数据
    cJSON_AddNumberToObject(obj, "type", cmd);
    cJSON_AddItemToObject(obj, "data", json);
    char *str = cJSON_Print(obj);
    memset(msg, 0, MSGSIZE);
    strcpy(msg, str);
    free(str);
    cJSON_Delete(obj);

    SendMsg(msgsocket->m_tcpSocket, msg);
}










