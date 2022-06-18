//#include "parse.h"
//#include "cJSON.h"
//#include <string.h>

/////连锁店获取所有商品请求信息解析
//void ParseAllCommodity(char *msg, int len)
//{
//    ExecSql exec_sql;
//    InitExecSql(&exec_sql, GetDatebase());
//    cJSON *obj = cJSON_CreateObject();  //新的json数据
//    cJSON *arr_json = NULL;
//    cJSON_AddNumberToObject(obj, "type", CMD_AllCommodity_D);

//    if((arr_json = SelectAllCommodity(&exec_sql)) != NULL){
//         cJSON_AddNumberToObject(obj, "result", CMD_Sucess_Y);
//         cJSON_AddItemToObject(obj, "array", arr_json);
//    }else{
//        cJSON_AddNumberToObject(obj, "result", CMD_Failed_Z);
//    }

//    char *str = cJSON_Print(obj);
//    memset(msg, 0, len);
//    strcpy(msg, str);

//    free(str);

//    cJSON_Delete(obj);

//    return ;
//}

void ParseCommodityImage(char *msg, int len)
{

}

/////商品信息解析接口
//void *parseCommodityInfo(void *arg)
//{
//    struct myevent_s *ev = (struct myevent_s *)arg;

//    char *string = ev->buf;
//    cJSON *json = cJSON_Parse(string);
//    cJSON *node = cJSON_GetObjectItem(json, "array");
//    cJSON *tnode = NULL;
//    CommodityInfo *info = NULL;
//    int com_len;
//    int i, j;

//    cJSON *new_json = cJSON_CreateObject();
//    cJSON_AddNumberToObject(new_json, "type", CMD_Commodity_C);

//    if(node->type == cJSON_Array)
//    {
//        cJSON_AddNumberToObject(new_json, "result", CMD_Sucess_S);
//        int len = cJSON_GetArraySize(node);

//        cJSON *snode = NULL;
//        cJSON *array = cJSON_CreateArray();
//        cJSON_AddItemToObject(new_json, "commodity_array", array);

//        for(i = 0; i < len; i++)
//        {
//            tnode = cJSON_GetArrayItem(node, i);
//            info = selectCommodityInfoFromShopID(tnode->valuestring, &com_len);
//            if(info == NULL)
//                continue;

//            for(j = 0; j < com_len; j++)
//            {
//                snode = cJSON_CreateObject();
//                cJSON_AddStringToObject(snode, "com_id", info[j].com_id);
//                cJSON_AddStringToObject(snode, "com_shopid", info[j].com_shopid);
//                cJSON_AddStringToObject(snode, "com_name", info[j].com_name);
//                cJSON_AddStringToObject(snode, "com_sprice", info[j].com_sprice);
//                cJSON_AddStringToObject(snode, "com_sex", info[j].com_sex);
//                cJSON_AddStringToObject(snode, "com_image", info[j].com_image);

//                cJSON_AddItemToArray(array, snode);
//                snode = NULL;
//            }
//            if(info != NULL)
//            {
//                free(info);
//                info = NULL;
//            }
//        }
//    }else if(node->valueint == -1)
//    {
//        cJSON_AddNumberToObject(new_json, "result", CMD_Sucess_S);
//        cJSON_AddNumberToObject(new_json, "commodity_array", -1);
//    }else
//    {
//        cJSON_AddNumberToObject(new_json, "result", CMD_File_F);
//    }

//    char *str = cJSON_Print(new_json);
//    printf("server send: %s\n", str);
//    strcpy(ev->buf, str);
//    ev->len = strlen(str);

//    free(str);
//    cJSON_Delete(new_json);
//    cJSON_Delete(json);

//    return (void *)ev;
//}

/////商品规格信息解析接口
//void *parseSortInfo(void *arg)
//{
//    struct myevent_s *ev = (struct myevent_s *)arg;

//    char *string = ev->buf;
//    cJSON *json = cJSON_Parse(string);
//    cJSON *node = cJSON_GetObjectItem(json, "array");
//    cJSON *tnode = NULL;
//    SortInfo *info = NULL;
//    int so_len;
//    int i, j;

//    cJSON *new_json = cJSON_CreateObject();
//    cJSON_AddNumberToObject(new_json, "type", CMD_SortInfo_O);

//    if(node->type == cJSON_Array)
//    {
//        cJSON_AddNumberToObject(new_json, "result", CMD_Sucess_S);
//        int len = cJSON_GetArraySize(node);
//        cJSON *snode = NULL;
//        cJSON *array = cJSON_CreateArray();

//        cJSON_AddItemToObject(new_json, "sort_array", array);
//        for(i = 0; i < len; i++)
//        {
//            tnode = cJSON_GetArrayItem(node, i);
//            info = selectSortInfoFromComID(tnode->valuestring, &so_len);
//            if(info == NULL)
//                continue;

//            for(j = 0; j < so_len; j++)
//            {
//                snode = cJSON_CreateObject();
//                cJSON_AddStringToObject(snode, "so_id", info[j].so_id);
//                cJSON_AddStringToObject(snode, "so_comid", info[j].so_comid);
//                cJSON_AddStringToObject(snode, "so_bar", info[j].so_bar);
//                cJSON_AddStringToObject(snode, "so_colour", info[j].so_colour);
//                cJSON_AddStringToObject(snode, "so_num", info[j].so_num);

//                cJSON_AddItemToArray(array, snode);
//                snode = NULL;
//            }
//            if(info != NULL)
//            {
//                free(info);
//                info = NULL;
//            }
//        }
//    }else if(node->valueint == -1)
//    {
//        cJSON_AddNumberToObject(new_json, "result", CMD_Sucess_S);
//        cJSON_AddNumberToObject(new_json, "sort_array", -1);
//    }else
//    {
//        cJSON_AddNumberToObject(new_json, "result", CMD_File_F);
//    }

//    char *str = cJSON_Print(new_json);
//    printf("server send: %s\n", str);
//    strcpy(ev->buf, str);
//    ev->len = strlen(str);

//    free(str);
//    cJSON_Delete(new_json);
//    cJSON_Delete(json);

//    return (void *)ev;
//}

/////订单信息解析接口
//void *parseFormInfo(void *arg)
//{
//    struct myevent_s *ev = (struct myevent_s *)arg;

//    char *string = ev->buf;
//    cJSON *json = cJSON_Parse(string);
//    cJSON *node = cJSON_GetObjectItem(json, "array");

//    cJSON *user_node = cJSON_GetObjectItem(json, "user_type");
//    FormInfo *(*selectFormInfoFromID)(char *, int *) = NULL;

//    cJSON *tnode = NULL;
//    FormInfo *info = NULL;
//    int fo_len = 0;
//    int i, j;

//    cJSON *new_json = cJSON_CreateObject();
//    cJSON_AddNumberToObject(new_json, "type", CMD_FormInfo_R);

//    if(node->type == cJSON_Array)
//    {
//        cJSON_AddNumberToObject(new_json, "result", CMD_Sucess_S);
//        int len = cJSON_GetArraySize(node);
//        cJSON *snode = NULL;
//        cJSON *array = cJSON_CreateArray();

//        if(strcmp(user_node->valuestring, "商家") == 0)
//        {
//            selectFormInfoFromID = selectFormInfoFromSoID;
//        }else
//        {
//            selectFormInfoFromID = selectFormInfoFromCliID;
//        }

//        cJSON_AddItemToObject(new_json, "form_array", array);
//        for(i = 0; i < len; i++)
//        {
//            tnode = cJSON_GetArrayItem(node, i);

//            info = selectFormInfoFromID(tnode->valuestring, &fo_len);
//            if(info == NULL)
//                continue;
//            for(j = 0; j < fo_len; j++)
//            {
//                snode = cJSON_CreateObject();
//                cJSON_AddStringToObject(snode, "fo_id", info[j].fo_id);
//                cJSON_AddStringToObject(snode, "fo_comid", info[j].fo_comid);
//                cJSON_AddStringToObject(snode, "fo_cliid", info[j].fo_cliid);
//                cJSON_AddStringToObject(snode, "fo_soid", info[j].fo_soid);
//                cJSON_AddStringToObject(snode, "fo_time", info[j].fo_time);
//                cJSON_AddStringToObject(snode, "fo_num", info[j].fo_num);

//                cJSON_AddItemToArray(array, snode);
//                snode = NULL;
//            }
//            if(info != NULL)
//            {
//                free(info);
//                info = NULL;
//            }
//        }
//    }else if(node->valueint == -1)
//    {
//        cJSON_AddNumberToObject(new_json, "result", CMD_Sucess_S);
//        cJSON_AddNumberToObject(new_json, "form_array", -1);
//    }else
//    {
//        cJSON_AddNumberToObject(new_json, "result", CMD_File_F);
//    }


//    char *str = cJSON_Print(new_json);
//    printf("server send: %s\n", str);
//    memset(ev->buf, 0, ev->len);
//    memcpy(ev->buf, str, strlen(str));
//    ev->len = strlen(str);
//    ev->buf[ev->len] = '\0';

//    free(str);
//    cJSON_Delete(new_json);
//    cJSON_Delete(json);

//    return (void *)ev;
//}

/////商家信息解析接口
//void *parseMerchantInfo(void *arg)
//{
//    struct myevent_s *ev = (struct myevent_s *)arg;

//    char *string = ev->buf;
//    cJSON *json = cJSON_Parse(string);
//    MerchantInfo *info = NULL;
//    int mer_len;
//    int j;

//    cJSON *new_json = cJSON_CreateObject();
//    cJSON_AddNumberToObject(new_json, "type", CMD_MerchatInfo);
//    cJSON_AddNumberToObject(new_json, "result", CMD_Sucess_S);

//    cJSON *snode = NULL;
//    cJSON *array = cJSON_CreateArray();
//    info = selectMerchantAll(&mer_len);

//    if(info == NULL)
//    {
//        cJSON_AddNumberToObject(new_json, "mer_array", -1);
//    }else
//    {
//        for(j = 0; j < mer_len; j++)
//        {
//            snode = cJSON_CreateObject();
//            cJSON_AddStringToObject(snode, "mer_id", info[j].mer_id);
//            cJSON_AddStringToObject(snode, "mer_name", info[j].mer_name);
//            cJSON_AddStringToObject(snode, "mer_phone", info[j].mer_phone);
//            cJSON_AddStringToObject(snode, "mer_time", info[j].mer_time);
//            cJSON_AddStringToObject(snode, "mer_site", info[j].mer_site);
//            cJSON_AddStringToObject(snode, "mer_image", info[j].mer_image);

//            cJSON_AddItemToArray(array, snode);
//            snode = NULL;
//        }
//        cJSON_AddItemToObject(new_json, "mer_array", array);
//        free(info);
//        info = NULL;
//    }

//    char *str = cJSON_Print(new_json);
//    printf("server send: %s\n", str);
//    memset(ev->buf, 0, ev->len);
//    memcpy(ev->buf, str, strlen(str));
//    ev->len = strlen(str);
//    ev->buf[ev->len] = '\0';

//    free(str);
//    cJSON_Delete(new_json);
//    cJSON_Delete(json);

//    return (void *)ev;
//}

/////图片信息解析接口
//void *parseImageInfo(void *arg)
//{
//    struct myevent_s *ev = (struct myevent_s *)arg;

//    char *string = ev->buf;
//    cJSON *json = cJSON_Parse(string);
//    cJSON *id_node = cJSON_GetObjectItem(json, "id");
//    cJSON *image_node = cJSON_GetObjectItem(json, "path");
//    cJSON *new_json = cJSON_CreateObject();

//    char *image_str = selectImageForPath(image_node->valuestring);
//    if(image_str != NULL)
//    {
//        cJSON_AddNumberToObject(new_json, "type", CMD_Image_G);
//        cJSON_AddNumberToObject(new_json, "result", CMD_Sucess_S);
//        cJSON_AddStringToObject(new_json, "id", id_node->valuestring);
//        cJSON_AddStringToObject(new_json, "image", image_str);
//        free(image_str);
//    }

//    char *str = cJSON_Print(new_json);
//    printf("server send: %s\n", str);
//    memset(ev->buf, 0, ev->len);
//    memcpy(ev->buf, str, strlen(str));
//    ev->len = strlen(str);
//    ev->buf[ev->len] = '\0';

//    free(str);
//    cJSON_Delete(new_json);
//    cJSON_Delete(json);

//    return (void *)ev;
//}






