#include "execsql.h"

#include <sys/time.h>
#include <time.h>

void InitExecSql(ExecSql *execsql, MYSQL *mysql)
{
    execsql->mysql = mysql;
    pthread_mutex_init(&execsql->mysql_mtx, NULL);
}


///用户校验
int CheckUserLogin(ExecSql *execsql, const char *id, const char *passward)
{
    unsigned int line = 0;  //记录的行
    MYSQL_RES *res = NULL;

    char queryString[100] = "";
    sprintf(queryString, "select user_id, user_passward from userinfo "
                         "where user_id = '%s' and user_passward = '%s'", id, passward);
    printf("%s\n", queryString);

    //查询数据库中某个表的内容
    pthread_mutex_lock(&execsql->mysql_mtx);     //加锁防止数据库操作紊乱
    if(mysql_real_query(execsql->mysql, queryString, strlen(queryString)) != 0)
    {
        pthread_mutex_unlock(&execsql->mysql_mtx);
        printf("mysql_real_query: %s\n", strerror(mysql_errno(execsql->mysql)));
        exit(1);
    }

    //mysql_store_result()立即检索所有的行并提取了行，并为之分配内存，存储到客户机中
    if((res = mysql_store_result(execsql->mysql)) == NULL)
    {
        pthread_mutex_unlock(&execsql->mysql_mtx);
        printf("mysql_store_result: %s\n", strerror(mysql_errno(execsql->mysql)));
        exit(1);
    }
    pthread_mutex_unlock(&execsql->mysql_mtx);

    if((line = mysql_num_rows(res)) == 0)
    {
        return 0;
    }else
    {
        return 1;
    }
}

///获取所有的商品信息
cJSON *SelectAllCommodity(ExecSql *execsql)
{
    unsigned int line = 0;  //记录的行
    MYSQL_RES *res = NULL;
    MYSQL_ROW row = NULL;
    cJSON *array = NULL;

    char queryString[100] = "";
    sprintf(queryString, "select * from goodsinfo");
    printf("%s\n", queryString);

    //查询数据库中某个表的内容
    pthread_mutex_lock(&execsql->mysql_mtx);     //加锁防止数据库操作紊乱
    if(mysql_real_query(execsql->mysql, queryString, strlen(queryString)) != 0){
        pthread_mutex_unlock(&execsql->mysql_mtx);
        printf("mysql_real_query: %s\n", strerror(mysql_errno(execsql->mysql)));
        exit(1);
    }

    if((res = mysql_store_result(execsql->mysql)) == NULL){
        pthread_mutex_unlock(&execsql->mysql_mtx);
        printf("mysql_store_result: %s\n", strerror(mysql_errno(execsql->mysql)));
        exit(1);
    }
    pthread_mutex_unlock(&execsql->mysql_mtx);

    if((line = mysql_num_rows(res)) == 0){
        return array;
    }else{
        array = cJSON_CreateArray();
    }

    cJSON *snode = NULL;
    while ((row = mysql_fetch_row(res)) != NULL)
    {
        snode = cJSON_CreateObject();
        cJSON_AddStringToObject(snode, "goods_id", row[0]);
        cJSON_AddStringToObject(snode, "goods_name", row[1]);
        cJSON_AddStringToObject(snode, "goods_old_price", row[2]);
        cJSON_AddStringToObject(snode, "goods_new_price", row[3]);
        cJSON_AddStringToObject(snode, "goods_call_num", row[4]);
        cJSON_AddStringToObject(snode, "goods_head", row[5]);

        cJSON_AddItemToArray(array, snode);
        snode = NULL;
    }

    return array;
}

///查询所有店铺的信息
cJSON *SelectAllShopInfo(ExecSql *execsql)
{
    unsigned int line = 0;  //记录的行
    MYSQL_RES *res = NULL;
    MYSQL_ROW row = NULL;
    cJSON *array = NULL;

    char queryString[100] = "";
    sprintf(queryString, "select * from marketinfo");
    printf("%s\n", queryString);

    //查询数据库中某个表的内容
    pthread_mutex_lock(&execsql->mysql_mtx);     //加锁防止数据库操作紊乱
    if(mysql_real_query(execsql->mysql, queryString, strlen(queryString)) != 0){
        pthread_mutex_unlock(&execsql->mysql_mtx);
        printf("mysql_real_query: %s\n", strerror(mysql_errno(execsql->mysql)));
        exit(1);
    }

    if((res = mysql_store_result(execsql->mysql)) == NULL){
        pthread_mutex_unlock(&execsql->mysql_mtx);
        printf("mysql_store_result: %s\n", strerror(mysql_errno(execsql->mysql)));
        exit(1);
    }
    pthread_mutex_unlock(&execsql->mysql_mtx);

    if((line = mysql_num_rows(res)) == 0){
        return array;
    }else{
        array = cJSON_CreateArray();
    }

    cJSON *snode = NULL;
    while ((row = mysql_fetch_row(res)) != NULL)
    {
        snode = cJSON_CreateObject();
        cJSON_AddStringToObject(snode, "id", row[0]);
        cJSON_AddStringToObject(snode, "name", row[1]);
        cJSON_AddStringToObject(snode, "phone", row[2]);
        cJSON_AddStringToObject(snode, "site", row[3]);

        char str[100] = "";
        strcpy(str, row[0]);
        if(SelectUserTimeForID(execsql ,str)){
            cJSON_AddStringToObject(snode, "time", str);
        }

        cJSON_AddItemToArray(array, snode);
        snode = NULL;
    }

    return array;
}

///根据ID获取注册时间
int SelectUserTimeForID(ExecSql *execsql, char *str)
{
    unsigned int line = 0;  //记录的行
    MYSQL_RES *res = NULL;
    MYSQL_ROW row = NULL;

    char queryString[100] = "";
    sprintf(queryString, "select user_time from userinfo where user_id = '%s'", str);
    printf("%s\n", queryString);

    //查询数据库中某个表的内容
    pthread_mutex_lock(&execsql->mysql_mtx);     //加锁防止数据库操作紊乱
    if(mysql_real_query(execsql->mysql, queryString, strlen(queryString)) != 0){
        pthread_mutex_unlock(&execsql->mysql_mtx);
        printf("mysql_real_query: %s\n", strerror(mysql_errno(execsql->mysql)));
        exit(1);
    }

    if((res = mysql_store_result(execsql->mysql)) == NULL){
        pthread_mutex_unlock(&execsql->mysql_mtx);
        printf("mysql_store_result: %s\n", strerror(mysql_errno(execsql->mysql)));
        exit(1);
    }
    pthread_mutex_unlock(&execsql->mysql_mtx);

    if((line = mysql_num_rows(res)) == 0){
        return 0;
    }

    if((row = mysql_fetch_row(res)) != NULL)
    {
        memset(str, 0, 100);
        strcpy(str, row[0]);
        return 1;
    }

    return 0;
}

///根据店铺ID查询对应的商品库存
cJSON *SelectCommodityForShid(ExecSql *execsql, const char *id)
{
    unsigned int line = 0;  //记录的行
    MYSQL_RES *res = NULL;
    MYSQL_ROW row = NULL;
    cJSON *array = NULL;

    char queryString[100] = "";
    sprintf(queryString, "select * from goods_numinfo where goods_num_mid = '%s'", id);
    printf("%s\n", queryString);

    //查询数据库中某个表的内容
    pthread_mutex_lock(&execsql->mysql_mtx);     //加锁防止数据库操作紊乱
    if(mysql_real_query(execsql->mysql, queryString, strlen(queryString)) != 0){
        pthread_mutex_unlock(&execsql->mysql_mtx);
        printf("mysql_real_query: %s\n", strerror(mysql_errno(execsql->mysql)));
        exit(1);
    }

    if((res = mysql_store_result(execsql->mysql)) == NULL){
        pthread_mutex_unlock(&execsql->mysql_mtx);
        printf("mysql_store_result: %s\n", strerror(mysql_errno(execsql->mysql)));
        exit(1);
    }
    pthread_mutex_unlock(&execsql->mysql_mtx);

    if((line = mysql_num_rows(res)) == 0){
        return array;
    }else{
        array = cJSON_CreateArray();
    }

    cJSON *snode = NULL;
    while ((row = mysql_fetch_row(res)) != NULL)
    {
        snode = cJSON_CreateObject();
        cJSON_AddStringToObject(snode, "goods_num_id", row[0]);
        cJSON_AddStringToObject(snode, "goods_num_gid", row[1]);
        cJSON_AddStringToObject(snode, "goods_num_mid", row[2]);
        cJSON_AddStringToObject(snode, "goods_num_all", row[3]);
        cJSON_AddStringToObject(snode, "goods_num_sale", row[4]);
        cJSON_AddStringToObject(snode, "goods_num_residue", row[5]);
        cJSON_AddStringToObject(snode, "goods_num_shipment", row[6]);

        cJSON_AddItemToArray(array, snode);
        snode = NULL;
    }

    return array;
}

///获取所有的调货信息
cJSON *SelectAllShipmentInfo(ExecSql *execsql)
{
    unsigned int line = 0;  //记录的行
    MYSQL_RES *res = NULL;
    MYSQL_ROW row = NULL;
    cJSON *array = NULL;

    char queryString[100] = "";
    sprintf(queryString, "select * from shipmentinfo where shipment_state = '审核中'");
    printf("%s\n", queryString);

    //查询数据库中某个表的内容
    pthread_mutex_lock(&execsql->mysql_mtx);     //加锁防止数据库操作紊乱
    if(mysql_real_query(execsql->mysql, queryString, strlen(queryString)) != 0){
        pthread_mutex_unlock(&execsql->mysql_mtx);
        printf("mysql_real_query: %s\n", strerror(mysql_errno(execsql->mysql)));
        exit(1);
    }

    if((res = mysql_store_result(execsql->mysql)) == NULL){
        pthread_mutex_unlock(&execsql->mysql_mtx);
        printf("mysql_store_result: %s\n", strerror(mysql_errno(execsql->mysql)));
        exit(1);
    }
    pthread_mutex_unlock(&execsql->mysql_mtx);

    if((line = mysql_num_rows(res)) == 0){
        return array;
    }else{
        array = cJSON_CreateArray();
    }

    cJSON *snode = NULL;
    char str[100] = "";
    while ((row = mysql_fetch_row(res)) != NULL)
    {
        snode = cJSON_CreateObject();
        cJSON_AddStringToObject(snode, "shipment_id", row[0]);
        cJSON_AddStringToObject(snode, "shipment_qid", row[1]);

        memset(str, 0, 100);
        strcpy(str, row[1]);
        if(SelectMarketNameForID(execsql ,str)){
            cJSON_AddStringToObject(snode, "shipment_qname", str);
        }

        memset(str, 0, 100);
        strcpy(str, row[2]);
        if(SelectMarketNameForID(execsql ,str)){
            cJSON_AddStringToObject(snode, "shipment_fname", str);
        }

        cJSON_AddStringToObject(snode, "shipment_gid", row[3]);
        cJSON_AddStringToObject(snode, "shipment_num", row[4]);
        cJSON_AddStringToObject(snode, "shipment_time", row[5]);
        cJSON_AddStringToObject(snode, "shipment_state", row[6]);

        cJSON_AddItemToArray(array, snode);
        snode = NULL;
    }

    return array;
}

///根据ID获取店铺/企业的名字
int SelectMarketNameForID(ExecSql *execsql, char *str)
{
    unsigned int line = 0;  //记录的行
    MYSQL_RES *res = NULL;
    MYSQL_ROW row = NULL;

    char queryString[100] = "";
    sprintf(queryString, "select market_name from marketinfo  where market_id = '%s'", str);
    printf("%s\n", queryString);

    //查询数据库中某个表的内容
    pthread_mutex_lock(&execsql->mysql_mtx);     //加锁防止数据库操作紊乱
    if(mysql_real_query(execsql->mysql, queryString, strlen(queryString)) != 0){
        pthread_mutex_unlock(&execsql->mysql_mtx);
        printf("mysql_real_query: %s\n", strerror(mysql_errno(execsql->mysql)));
        exit(1);
    }

    if((res = mysql_store_result(execsql->mysql)) == NULL){
        pthread_mutex_unlock(&execsql->mysql_mtx);
        printf("mysql_store_result: %s\n", strerror(mysql_errno(execsql->mysql)));
        exit(1);
    }
    pthread_mutex_unlock(&execsql->mysql_mtx);

    if((line = mysql_num_rows(res)) == 0){
        return 0;
    }

    if((row = mysql_fetch_row(res)) != NULL)
    {
        strcpy(str, row[0]);
        return 1;
    }

    return 0;
}

///根据商品ID和数量查询对应的库存
cJSON *SelectCommodityForComID(ExecSql *execsql, const char *id, const char *num)
{
    unsigned int line = 0;  //记录的行
    MYSQL_RES *res = NULL;
    MYSQL_ROW row = NULL;
    cJSON *array = NULL;

    char queryString[100] = "";
    sprintf(queryString, "select * from goods_numinfo where goods_num_gid = '%s'", id);
    printf("%s\n", queryString);

    //查询数据库中某个表的内容
    pthread_mutex_lock(&execsql->mysql_mtx);     //加锁防止数据库操作紊乱
    if(mysql_real_query(execsql->mysql, queryString, strlen(queryString)) != 0){
        pthread_mutex_unlock(&execsql->mysql_mtx);
        printf("mysql_real_query: %s\n", strerror(mysql_errno(execsql->mysql)));
        exit(1);
    }

    if((res = mysql_store_result(execsql->mysql)) == NULL){
        pthread_mutex_unlock(&execsql->mysql_mtx);
        printf("mysql_store_result: %s\n", strerror(mysql_errno(execsql->mysql)));
        exit(1);
    }
    pthread_mutex_unlock(&execsql->mysql_mtx);

    if((line = mysql_num_rows(res)) == 0){
        return array;
    }else{
        array = cJSON_CreateArray();
    }

    cJSON *snode = NULL;
    int g_num;
    sscanf(num, "%d", &g_num);
    while ((row = mysql_fetch_row(res)) != NULL)
    {
        //商品的剩余数与请求数的比较
        int r_num;
        sscanf(row[5], "%d", &r_num);
        if(g_num > r_num){
            continue;
        }
        snode = cJSON_CreateObject();
        cJSON_AddStringToObject(snode, "com_id", row[1]);
        cJSON_AddStringToObject(snode, "com_mid", row[2]);
        char str[100] = "";
        strcpy(str, row[2]);
        if(SelectMarketNameForID(execsql ,str)){
            cJSON_AddStringToObject(snode, "com_mname", str);
        }

        memset(str, 0, 100);
        strcpy(str, row[2]);
        if(SelectMarketSiteForID(execsql ,str)){
            cJSON_AddStringToObject(snode, "com_msite", str);
        }

        cJSON_AddStringToObject(snode, "com_all", row[3]);
        cJSON_AddStringToObject(snode, "com_shipment", row[6]);
        cJSON_AddStringToObject(snode, "com_residue", row[5]);

        cJSON_AddItemToArray(array, snode);
        snode = NULL;
    }

    return array;
}

///根据id获取店铺/企业的地址
int SelectMarketSiteForID(ExecSql *execsql, char *str)
{
    unsigned int line = 0;  //记录的行
    MYSQL_RES *res = NULL;
    MYSQL_ROW row = NULL;

    char queryString[100] = "";
    sprintf(queryString, "select market_site from marketinfo  where market_id = '%s'", str);
    printf("%s\n", queryString);

    //查询数据库中某个表的内容
    pthread_mutex_lock(&execsql->mysql_mtx);     //加锁防止数据库操作紊乱
    if(mysql_real_query(execsql->mysql, queryString, strlen(queryString)) != 0){
        pthread_mutex_unlock(&execsql->mysql_mtx);
        printf("mysql_real_query: %s\n", strerror(mysql_errno(execsql->mysql)));
        exit(1);
    }

    if((res = mysql_store_result(execsql->mysql)) == NULL){
        pthread_mutex_unlock(&execsql->mysql_mtx);
        printf("mysql_store_result: %s\n", strerror(mysql_errno(execsql->mysql)));
        exit(1);
    }
    pthread_mutex_unlock(&execsql->mysql_mtx);

    if((line = mysql_num_rows(res)) == 0){
        return 0;
    }

    if((row = mysql_fetch_row(res)) != NULL)
    {
        memset(str, 0, 100);
        strcpy(str, row[0]);
        return 1;
    }

    return 0;
}

///根据店铺ID查询对应的货单
cJSON *SelectShipmentForShid(ExecSql *execsql, const char *id)
{
    unsigned int line = 0;  //记录的行
    MYSQL_RES *res = NULL;
    MYSQL_ROW row = NULL;
    cJSON *array = NULL;

    char queryString[100] = "";
    sprintf(queryString, "select * from shipmentinfo where shipment_qmid = '%s' or shipment_fmid = '%s'", id, id);
    printf("%s\n", queryString);

    //查询数据库中某个表的内容
    pthread_mutex_lock(&execsql->mysql_mtx);     //加锁防止数据库操作紊乱
    if(mysql_real_query(execsql->mysql, queryString, strlen(queryString)) != 0){
        pthread_mutex_unlock(&execsql->mysql_mtx);
        printf("mysql_real_query: %s\n", strerror(mysql_errno(execsql->mysql)));
        exit(1);
    }

    if((res = mysql_store_result(execsql->mysql)) == NULL){
        pthread_mutex_unlock(&execsql->mysql_mtx);
        printf("mysql_store_result: %s\n", strerror(mysql_errno(execsql->mysql)));
        exit(1);
    }
    pthread_mutex_unlock(&execsql->mysql_mtx);

    if((line = mysql_num_rows(res)) == 0){
        return array;
    }else{
        array = cJSON_CreateArray();
    }

    cJSON *snode = NULL;
    while ((row = mysql_fetch_row(res)) != NULL)
    {
        snode = cJSON_CreateObject();
        cJSON_AddStringToObject(snode, "shipment_id", row[0]);
        cJSON_AddStringToObject(snode, "shipment_qid", row[1]);
        cJSON_AddStringToObject(snode, "shipment_fid", row[2]);

        char str[100] = "";
        strcpy(str, row[1]);
        if(SelectMarketNameForID(execsql ,str)){
            cJSON_AddStringToObject(snode, "shipment_qname", str);
        }

        memset(str, 0, 100);
        strcpy(str, row[2]);
        if(SelectMarketNameForID(execsql ,str)){
            cJSON_AddStringToObject(snode, "shipment_fname", str);
        }

        cJSON_AddStringToObject(snode, "shipment_gid", row[3]);
        cJSON_AddStringToObject(snode, "shipment_num", row[4]);
        cJSON_AddStringToObject(snode, "shipment_time", row[5]);
        cJSON_AddStringToObject(snode, "shipment_state", row[6]);

        cJSON_AddItemToArray(array, snode);
        snode = NULL;
    }
    return array;
}

/* 插入数据 */
int InsertShipment(ExecSql *execsql, char *s_qid, char *s_fid, char *s_comid, char *s_comnum)
{
    char queryString[200] = "";
    char timestr[40] = "";
    char shipment_id[30] = "SH";
    char id_time[30] = "";

    time_t t;
    struct tm *nowtime;
    time(&t);
    nowtime = localtime(&t);
    strftime(timestr,sizeof(timestr),"%Y-%m-%d %H:%M",nowtime);
    strftime(id_time,sizeof(id_time),"%Y%m%d%H%M%S",nowtime);
    strcat(shipment_id, id_time);

    //拼接sql语句
    sprintf(queryString, "insert into shipmentinfo values('%s', '%s', '%s', '%s', '%s', '%s', '%s')",
            shipment_id, s_qid, s_fid, s_comid, s_comnum, timestr, "审核中");
    printf("%s\n", queryString);

    pthread_mutex_lock(&execsql->mysql_mtx);     //加锁防止数据库操作紊乱
    if(mysql_real_query(execsql->mysql, queryString, strlen(queryString)) != 0){
        pthread_mutex_unlock(&execsql->mysql_mtx);
        printf("mysql_real_query: %s\n", strerror(mysql_errno(execsql->mysql)));
        return 0;
    }
    pthread_mutex_unlock(&execsql->mysql_mtx);
    return 1;
}

//void GetTime(char *timestr)
//{
//    //例子
//    time_t t;
//    struct tm *nowtime;

//    time(&t);
//    nowtime = localtime(&t);
//    strftime(timestr,sizeof(timestr),"%Y-%m-%d %H:%M",nowtime);

//    return ;
//}

///处理调货信息
int UpdateShipment(ExecSql *execsql, char *ship_id, char *ship_fid, char *ship_state)
{
    char queryString[200] = "";
    char timestr[40] = "";

    time_t t;
    struct tm *nowtime;
    time(&t);
    nowtime = localtime(&t);
    strftime(timestr,sizeof(timestr),"%Y-%m-%d %H:%M",nowtime);


    //拼接sql语句
    sprintf(queryString, "update shipmentinfo set shipment_fmid = '%s', shipment_time = '%s', "
                         "shipment_state = '%s' where shipment_id = '%s'",
            ship_fid, timestr, ship_state, ship_id);
    printf("%s\n", queryString);

    pthread_mutex_lock(&execsql->mysql_mtx);     //加锁防止数据库操作紊乱
    if(mysql_real_query(execsql->mysql, queryString, strlen(queryString)) != 0){
        pthread_mutex_unlock(&execsql->mysql_mtx);
        printf("mysql_real_query: %s\n", strerror(mysql_errno(execsql->mysql)));
        return 0;
    }
    pthread_mutex_unlock(&execsql->mysql_mtx);
    return 1;
}

///更新调货方的库存
int UpdateGoodsNumForFID(ExecSql *execsql, char *ship_fid, char *ship_comid, char *fid_sale, char *fid_residue)
{
    char queryString[200] = "";

    //拼接sql语句
    sprintf(queryString, "update goods_numinfo set goods_num_shipment = '%s', goods_num_residue = '%s'"
                         "where goods_num_gid = '%s' and goods_num_mid = '%s'",
            fid_sale, fid_residue, ship_comid, ship_fid);
    printf("%s\n", queryString);

    pthread_mutex_lock(&execsql->mysql_mtx);     //加锁防止数据库操作紊乱
    if(mysql_real_query(execsql->mysql, queryString, strlen(queryString)) != 0){
        pthread_mutex_unlock(&execsql->mysql_mtx);
        printf("mysql_real_query: %s\n", strerror(mysql_errno(execsql->mysql)));
        return 0;
    }
    pthread_mutex_unlock(&execsql->mysql_mtx);
    return 1;
}

///根据mid和gid查找goods_numinfo表看是否存在这个记录
cJSON *SearchGoodsNUmForGMID(ExecSql *execsql, char *ship_qid, char *ship_comid)
{
    unsigned int line = 0;  //记录的行
    MYSQL_RES *res = NULL;
    MYSQL_ROW row = NULL;

    char querySearch[200] = "";
    sprintf(querySearch, "select * from goods_numinfo where goods_num_mid = '%s' and goods_num_gid = '%s'",
            ship_qid, ship_comid);
    printf("%s\n", querySearch);

    //查询数据库中某个表的内容
    pthread_mutex_lock(&execsql->mysql_mtx);     //加锁防止数据库操作紊乱
    if(mysql_real_query(execsql->mysql, querySearch, strlen(querySearch)) != 0){
        pthread_mutex_unlock(&execsql->mysql_mtx);
        printf("mysql_real_query: %s\n", strerror(mysql_errno(execsql->mysql)));
        exit(1);
    }

    if((res = mysql_store_result(execsql->mysql)) == NULL){
        pthread_mutex_unlock(&execsql->mysql_mtx);
        printf("mysql_store_result: %s\n", strerror(mysql_errno(execsql->mysql)));
        exit(1);
    }
    pthread_mutex_unlock(&execsql->mysql_mtx);

    if((line = mysql_num_rows(res)) == 0){
        return NULL;
    }
    cJSON *snode = NULL;

    if((row = mysql_fetch_row(res)) != NULL)
    {
        snode = cJSON_CreateObject();
        cJSON_AddStringToObject(snode, "goods_num_id", row[0]);
        cJSON_AddStringToObject(snode, "goods_num_all", row[3]);
        cJSON_AddStringToObject(snode, "goods_num_sale", row[4]);
        cJSON_AddStringToObject(snode, "goods_num_residue", row[5]);
        return snode;
    }
    return NULL;
}

///更新申请取货方的库存
int UpdateGoodsNumForQID(ExecSql *execsql, char *ship_qid, char *ship_comid, char *ship_comnum)
{
    char queryString[200] = "";
    cJSON *node = NULL;

    //若goods_numinfo里面没有相应的记录，我们就添加一条，如果有，则直接更新
    if((node = SearchGoodsNUmForGMID(execsql, ship_qid, ship_comid)) == NULL){//添加

        char num_id[25] = "WN";
        char id_time[20] = "";

        time_t t;
        struct tm *nowtime;
        time(&t);
        nowtime = localtime(&t);
        strftime(id_time,sizeof(id_time),"%Y%m%d%H%M%S",nowtime);
        strcat(num_id, id_time);

        //拼接sql语句
        sprintf(queryString, "insert into goods_numinfo values('%s', '%s', '%s', '%s', '%s', '%s', '%s')",
                num_id, ship_comid, ship_qid, ship_comnum, "0", ship_comnum, "0");
        printf("%s\n", queryString);

        pthread_mutex_lock(&execsql->mysql_mtx);     //加锁防止数据库操作紊乱
        if(mysql_real_query(execsql->mysql, queryString, strlen(queryString)) != 0){
            pthread_mutex_unlock(&execsql->mysql_mtx);
            printf("mysql_real_query: %s\n", strerror(mysql_errno(execsql->mysql)));
            return 0;
        }
        pthread_mutex_unlock(&execsql->mysql_mtx);
        return 1;

    }else{//更新
        cJSON *node_id = cJSON_GetObjectItem(node, "goods_num_id");
        cJSON *node_all = cJSON_GetObjectItem(node, "goods_num_all");
        cJSON *node_residue = cJSON_GetObjectItem(node, "goods_num_residue");

        int all, residue, num;
        char allStr[20] = "";
        char residueStr[20] = "";
        sscanf(ship_comnum, "%d", &num);
        sscanf(node_all->valuestring, "%d", &all);
        sscanf(node_residue->valuestring, "%d", &residue);
        all = all + num;
        residue = residue + num;
        sprintf(allStr, "%d", all);
        sprintf(residueStr, "%d", residue);

        //拼接sql语句
        sprintf(queryString, "update goods_numinfo set goods_num_all = '%s', goods_num_residue = '%s'"
                             "where goods_num_id = '%s'",
                allStr, residueStr, node_id->valuestring);
        printf("%s\n", queryString);

        pthread_mutex_lock(&execsql->mysql_mtx);     //加锁防止数据库操作紊乱
        if(mysql_real_query(execsql->mysql, queryString, strlen(queryString)) != 0){
            pthread_mutex_unlock(&execsql->mysql_mtx);
            printf("mysql_real_query: %s\n", strerror(mysql_errno(execsql->mysql)));
            return 0;
        }
        pthread_mutex_unlock(&execsql->mysql_mtx);

        cJSON_Delete(node);
        return 1;
    }
}

///更新用户状态
int UpdateUserState(ExecSql *execsql, const char *id, const char *state)
{
    char queryString[200] = "";
    char timestr[40] = "";

    time_t t;
    struct tm *nowtime;
    time(&t);
    nowtime = localtime(&t);
    strftime(timestr,sizeof(timestr),"%Y-%m-%d %H:%M:%S",nowtime);

    //拼接sql语句
    sprintf(queryString, "update userinfo set user_state = '%s', user_time = '%s' where user_id = '%s'",
            state, timestr, id);
    printf("%s\n", queryString);

    pthread_mutex_lock(&execsql->mysql_mtx);     //加锁防止数据库操作紊乱
    if(mysql_real_query(execsql->mysql, queryString, strlen(queryString)) != 0){
        pthread_mutex_unlock(&execsql->mysql_mtx);
        printf("mysql_real_query: %s\n", strerror(mysql_errno(execsql->mysql)));
        return 0;
    }
    pthread_mutex_unlock(&execsql->mysql_mtx);

    return 1;
}

///查询用户的状态
cJSON *SelectUserState(ExecSql *execsql, const char *id)
{
    unsigned int line = 0;  //记录的行
    MYSQL_RES *res = NULL;
    MYSQL_ROW row = NULL;
    cJSON *node = NULL;

    char queryString[100] = "";
    sprintf(queryString, "select user_state from userinfo where user_id = '%s'", id);
    printf("%s\n", queryString);

    //查询数据库中某个表的内容
    pthread_mutex_lock(&execsql->mysql_mtx);     //加锁防止数据库操作紊乱
    if(mysql_real_query(execsql->mysql, queryString, strlen(queryString)) != 0){
        pthread_mutex_unlock(&execsql->mysql_mtx);
        printf("mysql_real_query: %s\n", strerror(mysql_errno(execsql->mysql)));
        exit(1);
    }

    if((res = mysql_store_result(execsql->mysql)) == NULL){
        pthread_mutex_unlock(&execsql->mysql_mtx);
        printf("mysql_store_result: %s\n", strerror(mysql_errno(execsql->mysql)));
        exit(1);
    }
    pthread_mutex_unlock(&execsql->mysql_mtx);

    if((line = mysql_num_rows(res)) == 0){
        return NULL;
    }

    if((row = mysql_fetch_row(res)) != NULL)
    {
        node = cJSON_CreateObject();
        cJSON_AddStringToObject(node, "id", id);
        cJSON_AddStringToObject(node, "state", row[0]);
    }

    return node;
}

///添加一条聊天记录
int InsertChatMsg(ExecSql *execsql, const char *jid, const char *msg, const char *state)
{
    cJSON *json = cJSON_Parse(msg);                 //解析json数据
    cJSON *node_msgid = cJSON_GetObjectItem(json, "msgid");
    cJSON *node_fid = cJSON_GetObjectItem(json, "fid");
    cJSON *node_msg = cJSON_GetObjectItem(json, "msg");
    cJSON *node_size = cJSON_GetObjectItem(json, "size");
    cJSON *node_time = cJSON_GetObjectItem(json, "time");

    char queryString[1500] = "";
    sprintf(queryString, "insert into msginfo values('%s', '%s', '%s', '%s', '%s', '%s', '%s')",
            node_msgid->valuestring, node_fid->valuestring, jid, node_time->valuestring,
            node_msg->valuestring, node_size->valuestring, state);
    printf("%s\n", queryString);

    pthread_mutex_lock(&execsql->mysql_mtx);     //加锁防止数据库操作紊乱
    if(mysql_real_query(execsql->mysql, queryString, strlen(queryString)) != 0){
        pthread_mutex_unlock(&execsql->mysql_mtx);
        printf("mysql_real_query: %s\n", strerror(mysql_errno(execsql->mysql)));
        return 0;
    }
    pthread_mutex_unlock(&execsql->mysql_mtx);
    return 1;
}

///根据ID查询对应的未读聊天记录
cJSON *SelectMsgInfoForID(ExecSql *execsql, const char *id, const char *state)
{
    unsigned int line = 0;  //记录的行
    MYSQL_RES *res = NULL;
    MYSQL_ROW row = NULL;
    cJSON *array = NULL;

    char queryString[100] = "";
    sprintf(queryString, "select * from msginfo where msg_sid = '%s' and msg_state = '%s'", id, state);
    printf("%s\n", queryString);

    //查询数据库中某个表的内容
    pthread_mutex_lock(&execsql->mysql_mtx);     //加锁防止数据库操作紊乱
    if(mysql_real_query(execsql->mysql, queryString, strlen(queryString)) != 0){
        pthread_mutex_unlock(&execsql->mysql_mtx);
        printf("mysql_real_query: %s\n", strerror(mysql_errno(execsql->mysql)));
        exit(1);
    }

    if((res = mysql_store_result(execsql->mysql)) == NULL){
        pthread_mutex_unlock(&execsql->mysql_mtx);
        printf("mysql_store_result: %s\n", strerror(mysql_errno(execsql->mysql)));
        exit(1);
    }
    pthread_mutex_unlock(&execsql->mysql_mtx);

    if((line = mysql_num_rows(res)) == 0){
        return array;
    }else{
        array = cJSON_CreateArray();
    }

    cJSON *snode = NULL;
    while ((row = mysql_fetch_row(res)) != NULL)
    {
        snode = cJSON_CreateObject();
        cJSON_AddStringToObject(snode, "msg_id", row[0]);
        cJSON_AddStringToObject(snode, "msg_fid", row[1]);
        cJSON_AddStringToObject(snode, "msg_info", row[4]);
        cJSON_AddStringToObject(snode, "msg_size", row[5]);

        cJSON_AddItemToArray(array, snode);
        snode = NULL;
    }
    return array;
}

///更新聊天记录表
int UpdateMsgInfoForID(ExecSql *execsql, const char *id, const char *state)
{
    char queryString[200] = "";
    char timestr[40] = "";

    time_t t;
    struct tm *nowtime;
    time(&t);
    nowtime = localtime(&t);
    strftime(timestr,sizeof(timestr),"%Y-%m-%d %H:%M:%S",nowtime);

    //拼接sql语句
    sprintf(queryString, "update msginfo set msg_state = '%s', msg_time = '%s' where msg_sid = '%s' and msg_state = '%s'",
            "Y", timestr, id, state);
    printf("%s\n", queryString);

    pthread_mutex_lock(&execsql->mysql_mtx);     //加锁防止数据库操作紊乱
    if(mysql_real_query(execsql->mysql, queryString, strlen(queryString)) != 0){
        pthread_mutex_unlock(&execsql->mysql_mtx);
        printf("mysql_real_query: %s\n", strerror(mysql_errno(execsql->mysql)));
        return 0;
    }
    pthread_mutex_unlock(&execsql->mysql_mtx);

    return 1;
}

///根据ID查询对应的人
cJSON *SelectFriendForID(ExecSql *execsql, const char *id)
{
    unsigned int line = 0;  //记录的行
    MYSQL_RES *res = NULL;
    MYSQL_ROW row = NULL;
    cJSON *node = NULL;

    char queryString[100] = "";
    sprintf(queryString, "select user_type from userinfo where user_id = '%s'", id);
    printf("%s\n", queryString);

    //查询数据库中某个表的内容
    pthread_mutex_lock(&execsql->mysql_mtx);     //加锁防止数据库操作紊乱
    if(mysql_real_query(execsql->mysql, queryString, strlen(queryString)) != 0){
        pthread_mutex_unlock(&execsql->mysql_mtx);
        printf("mysql_real_query: %s\n", strerror(mysql_errno(execsql->mysql)));
        exit(1);
    }

    if((res = mysql_store_result(execsql->mysql)) == NULL){
        pthread_mutex_unlock(&execsql->mysql_mtx);
        printf("mysql_store_result: %s\n", strerror(mysql_errno(execsql->mysql)));
        exit(1);
    }
    pthread_mutex_unlock(&execsql->mysql_mtx);

    if((line = mysql_num_rows(res)) == 0){
        return NULL;
    }

    if((row = mysql_fetch_row(res)) != NULL)
    {
        node = cJSON_CreateObject();
        cJSON_AddStringToObject(node, "id", id);
        char str[30] = "";
        strcpy(str, id);
        if( (strcmp(row[0], "client")) != 0){
            SelectMarketNameForID(execsql, str);//根据
        }
        cJSON_AddStringToObject(node, "name", str);

        cJSON *node_state = SelectUserState(execsql, id);
        cJSON *line = cJSON_GetObjectItem(node_state, "state");
        cJSON_AddStringToObject(node, "state", line->valuestring);
        cJSON_Delete(node_state);
    }

    return node;
}

///根据店铺ID查询本地会员
cJSON *SelectMemberInfoForID(ExecSql *execsql, const char *id)
{
    unsigned int line = 0;  //记录的行
    MYSQL_RES *res = NULL;
    MYSQL_ROW row = NULL;
    cJSON *array = NULL;

    char queryString[100] = "";
    sprintf(queryString, "select * from memberuserinfo where member_shopid = '%s'", id);
    printf("%s\n", queryString);

    //查询数据库中某个表的内容
    pthread_mutex_lock(&execsql->mysql_mtx);     //加锁防止数据库操作紊乱
    if(mysql_real_query(execsql->mysql, queryString, strlen(queryString)) != 0){
        pthread_mutex_unlock(&execsql->mysql_mtx);
        printf("mysql_real_query: %s\n", strerror(mysql_errno(execsql->mysql)));
        exit(1);
    }

    if((res = mysql_store_result(execsql->mysql)) == NULL){
        pthread_mutex_unlock(&execsql->mysql_mtx);
        printf("mysql_store_result: %s\n", strerror(mysql_errno(execsql->mysql)));
        exit(1);
    }
    pthread_mutex_unlock(&execsql->mysql_mtx);

    if((line = mysql_num_rows(res)) == 0){
        return array;
    }else{
        array = cJSON_CreateArray();
    }

    cJSON *snode = NULL;
    while ((row = mysql_fetch_row(res)) != NULL)
    {
        snode = cJSON_CreateObject();
        cJSON_AddStringToObject(snode, "id", row[0]);
        cJSON_AddStringToObject(snode, "name", row[1]);
        cJSON_AddStringToObject(snode, "phone", row[2]);
        cJSON_AddStringToObject(snode, "shopid", row[3]);
        cJSON_AddStringToObject(snode, "time", row[4]);
        cJSON_AddStringToObject(snode, "grade", row[5]);
        cJSON_AddStringToObject(snode, "total", row[6]);
        cJSON_AddStringToObject(snode, "price", row[7]);
        cJSON_AddStringToObject(snode, "discount", row[8]);

        cJSON_AddItemToArray(array, snode);
        snode = NULL;
    }

    return array;
}

///查找会员用户
cJSON *SelectMemberInfoForStr(ExecSql *execsql, const char *str)
{
    unsigned int line = 0;  //记录的行
    MYSQL_RES *res = NULL;
    MYSQL_ROW row = NULL;
    cJSON *snode = NULL;

    char queryString[100] = "";
    sprintf(queryString, "select * from memberuserinfo where member_id = '%s' or member_phone = '%s'", str, str);
    printf("%s\n", queryString);

    //查询数据库中某个表的内容
    pthread_mutex_lock(&execsql->mysql_mtx);     //加锁防止数据库操作紊乱
    if(mysql_real_query(execsql->mysql, queryString, strlen(queryString)) != 0){
        pthread_mutex_unlock(&execsql->mysql_mtx);
        printf("mysql_real_query: %s\n", strerror(mysql_errno(execsql->mysql)));
        exit(1);
    }

    if((res = mysql_store_result(execsql->mysql)) == NULL){
        pthread_mutex_unlock(&execsql->mysql_mtx);
        printf("mysql_store_result: %s\n", strerror(mysql_errno(execsql->mysql)));
        exit(1);
    }
    pthread_mutex_unlock(&execsql->mysql_mtx);

    if((line = mysql_num_rows(res)) == 0){
        return NULL;
    }else{
        snode = cJSON_CreateObject();
    }

    if((row = mysql_fetch_row(res)) != NULL)
    {
        cJSON_AddStringToObject(snode, "id", row[0]);
        cJSON_AddStringToObject(snode, "name", row[1]);
        cJSON_AddStringToObject(snode, "phone", row[2]);
        cJSON_AddStringToObject(snode, "shopid", row[3]);
        cJSON_AddStringToObject(snode, "time", row[4]);
        cJSON_AddStringToObject(snode, "grade", row[5]);
        cJSON_AddStringToObject(snode, "total", row[6]);
        cJSON_AddStringToObject(snode, "price", row[7]);
        cJSON_AddStringToObject(snode, "discount", row[8]);
    }

    return snode;
}

///注册会员
int InsertMember(ExecSql *execsql, const char *id, const char *name, const char *phone,
                 const char *shopid, const char *time, const char *grade, const char *total,
                 const char *price, const char *discount)
{
    char queryString[500] = "";

    //拼接sql语句
    sprintf(queryString, "insert into memberuserinfo values('%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s')",
            id, name, phone, shopid, time, grade, total, price, discount);
    printf("%s\n", queryString);

    pthread_mutex_lock(&execsql->mysql_mtx);     //加锁防止数据库操作紊乱
    if(mysql_real_query(execsql->mysql, queryString, strlen(queryString)) != 0){
        pthread_mutex_unlock(&execsql->mysql_mtx);
        printf("mysql_real_query: %s\n", strerror(mysql_errno(execsql->mysql)));
        return 0;
    }
    pthread_mutex_unlock(&execsql->mysql_mtx);
    return 1;
}

///增加一条订单
int InsertForm(ExecSql *execsql, const char *id,const char *gid, const char *cid, const char *mid,
               const char *num, const char *price, const char *state, const char *ftime,
               const char *mode, const char *value)
{
    char queryString[500] = "";

    //拼接sql语句
    sprintf(queryString, "insert into forminfo values('%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s')",
            id, gid, cid, mid, num, price, state, ftime, mode, value);
    printf("%s\n", queryString);

    pthread_mutex_lock(&execsql->mysql_mtx);     //加锁防止数据库操作紊乱
    if(mysql_real_query(execsql->mysql, queryString, strlen(queryString)) != 0){
        pthread_mutex_unlock(&execsql->mysql_mtx);
        printf("mysql_real_query: %s\n", strerror(mysql_errno(execsql->mysql)));
        return 0;
    }
    pthread_mutex_unlock(&execsql->mysql_mtx);
    return 1;
}

//根据店铺ID和商品ID更新库存表
int UpdateGoodsNum(ExecSql *execsql, const char *shid, const char *comid, const char *com_num)
{
    cJSON *node = NULL;
    char queryString[200] = "";
    //若goods_numinfo里面没有相应的记录，我们就添加一条，如果有，则直接更新
    if((node = SearchGoodsNUmForGMID(execsql, shid, comid)) != NULL){
        cJSON *node_id = cJSON_GetObjectItem(node, "goods_num_id");
        cJSON *node_sale = cJSON_GetObjectItem(node, "goods_num_sale");
        cJSON *node_residue = cJSON_GetObjectItem(node, "goods_num_residue");

        int sale, residue, num;
        char saleStr[20] = "";
        char residueStr[20] = "";
        sscanf(com_num, "%d", &num);
        sscanf(node_sale->valuestring, "%d", &sale);
        sscanf(node_residue->valuestring, "%d", &residue);
        sale = sale + num;
        residue = residue - num;
        sprintf(saleStr, "%d", sale);
        sprintf(residueStr, "%d", residue);

        //拼接sql语句
        sprintf(queryString, "update goods_numinfo set goods_num_sale = '%s', goods_num_residue = '%s'"
                             "where goods_num_id = '%s'",
                saleStr, residueStr, node_id->valuestring);
        printf("%s\n", queryString);

        pthread_mutex_lock(&execsql->mysql_mtx);     //加锁防止数据库操作紊乱
        if(mysql_real_query(execsql->mysql, queryString, strlen(queryString)) != 0){
            pthread_mutex_unlock(&execsql->mysql_mtx);
            printf("mysql_real_query: %s\n", strerror(mysql_errno(execsql->mysql)));
            return 0;
        }
        pthread_mutex_unlock(&execsql->mysql_mtx);

        cJSON_Delete(node);
        return 1;
    }
    return 0;
}

int UpdateMember(ExecSql *execsql, const char *id, const char *total, const char *price)
{
    char queryString[200] = "";

    //拼接sql语句
    sprintf(queryString, "update memberuserinfo set member_total = '%s', member_price = '%s' where member_id = '%s'",
            total, price, id);
    printf("%s\n", queryString);

    pthread_mutex_lock(&execsql->mysql_mtx);     //加锁防止数据库操作紊乱
    if(mysql_real_query(execsql->mysql, queryString, strlen(queryString)) != 0){
        pthread_mutex_unlock(&execsql->mysql_mtx);
        printf("mysql_real_query: %s\n", strerror(mysql_errno(execsql->mysql)));
        return 0;
    }
    pthread_mutex_unlock(&execsql->mysql_mtx);

    return 1;
}

///注册一个店铺
int InsertShopToUser(ExecSql *execsql, const char *id, const char *password, const char *time, const char *type)
{
    char queryString[200] = "";

    //拼接sql语句
    sprintf(queryString, "insert into userinfo values('%s', '%s', '%s', '%s', '%s')",
            id, password, type, time, "离线");
    printf("%s\n", queryString);

    pthread_mutex_lock(&execsql->mysql_mtx);     //加锁防止数据库操作紊乱
    if(mysql_real_query(execsql->mysql, queryString, strlen(queryString)) != 0){
        pthread_mutex_unlock(&execsql->mysql_mtx);
        printf("mysql_real_query: %s\n", strerror(mysql_errno(execsql->mysql)));
        return 0;
    }
    pthread_mutex_unlock(&execsql->mysql_mtx);
    return 1;
}

//添加注册的店铺到market
int InsertShopToMarket(ExecSql *execsql, const char *id, const char *name, const char *phone, const char *site)
{
    char queryString[200] = "";

    //拼接sql语句
    sprintf(queryString, "insert into marketinfo values('%s', '%s', '%s', '%s', '')",
            id, name, phone, site);
    printf("%s\n", queryString);

    pthread_mutex_lock(&execsql->mysql_mtx);     //加锁防止数据库操作紊乱
    if(mysql_real_query(execsql->mysql, queryString, strlen(queryString)) != 0){
        pthread_mutex_unlock(&execsql->mysql_mtx);
        printf("mysql_real_query: %s\n", strerror(mysql_errno(execsql->mysql)));
        return 0;
    }
    pthread_mutex_unlock(&execsql->mysql_mtx);
    return 1;
}

///更新密码
int UpdateUserPassword(ExecSql *execsql, const char *id, const char *password)
{
    char queryString[200] = "";

    //拼接sql语句
    sprintf(queryString, "update userinfo set user_passward = '%s' where user_id = '%s'", password, id);
    printf("%s\n", queryString);

    pthread_mutex_lock(&execsql->mysql_mtx);     //加锁防止数据库操作紊乱
    if(mysql_real_query(execsql->mysql, queryString, strlen(queryString)) != 0){
        pthread_mutex_unlock(&execsql->mysql_mtx);
        printf("mysql_real_query: %s\n", strerror(mysql_errno(execsql->mysql)));
        return 0;
    }
    pthread_mutex_unlock(&execsql->mysql_mtx);
    return 1;
}
