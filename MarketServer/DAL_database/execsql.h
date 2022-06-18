#ifndef EXECSQL_H
#define EXECSQL_H

#include <mysql/mysql.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

#include "cJSON.h"

typedef struct ExecSql{
    MYSQL *mysql;

    pthread_mutex_t mysql_mtx;
}ExecSql;

void InitExecSql(ExecSql *execsql, MYSQL *mysql);
//void GetTime(char *timestr);//获取系统日期时间

int CheckUserLogin(ExecSql *execsql, const char *id, const char *passward);      //用户校验
cJSON *SelectUserState(ExecSql *execsql, const char *id);                        //查询用户状态
int UpdateUserState(ExecSql *execsql, const char *id,const char *state);        //更新用户状态
int InsertChatMsg(ExecSql *execsql, const char *jid, const char *msg, const char *state);//添加一条聊天记录

cJSON *SelectAllCommodity(ExecSql *execsql);                                     //获取所有商品的信息
cJSON *SelectAllShopInfo(ExecSql *execsql);                                     //获取所有店铺的信息
int SelectUserTimeForID(ExecSql *execsql, char *str);//根据id获取注册时间

cJSON *SelectCommodityForShid(ExecSql *execsql, const char *id);                //根据店铺/商家ID查询对应的商品库存
cJSON *SelectAllShipmentInfo(ExecSql *execsql);                                 //获取所有调货信息
int SelectMarketNameForID(ExecSql *execsql, char *str);//根据id获取店铺的名字
int SelectMarketSiteForID(ExecSql *execsql, char *str);//根据id获取店铺的地址
cJSON *SelectCommodityForComID(ExecSql *execsql, const char *id, const char *num);  //根据商品ID和数量查询对应的库存

cJSON *SelectShipmentForShid(ExecSql *execsql, const char *id);                //根据店铺ID查询对应的货单
int InsertShipment(ExecSql *execsql, char *s_qid, char *s_fid, char *s_comid, char *s_comnum);  //添加一条调货信息

int UpdateShipment(ExecSql *execsql, char *ship_id, char *ship_fid, char *ship_state);            //处理调货信息
int UpdateGoodsNumForFID(ExecSql *execsql, char *ship_fid, char *ship_comid, char *fid_sale, char *fid_residue);  //更新调货方的库存
int UpdateGoodsNumForQID(ExecSql *execsql, char *ship_qid, char *ship_comid, char *ship_comnum);  //更新申请取货方的库存
cJSON *SearchGoodsNUmForGMID(ExecSql *execsql, char *ship_qid, char *ship_comid);//根据mid和gid查找goods_numinfo表看是否存在这个记录

cJSON *SelectMsgInfoForID(ExecSql *execsql, const char *id, const char *state);     //根据ID查询对应的未读聊天记录
int UpdateMsgInfoForID(ExecSql *execsql, const char *id, const char *state);        //更新聊天记录表
cJSON *SelectFriendForID(ExecSql *execsql, const char *id);                         //根据ID查询对应的人


cJSON *SelectMemberInfoForID(ExecSql *execsql, const char *id);                     //根据店铺ID查询本地会员
cJSON *SelectMemberInfoForStr(ExecSql *execsql, const char *str);                   //查找会员用户
int InsertMember(ExecSql *execsql, const char *id, const char *name, const char *phone,
                 const char *shopid, const char *time, const char *grade,
                 const char *total, const char *price, const char *discount);       //注册会员
int InsertForm(ExecSql *execsql, const char *id, const char *gid, const char *cid, const char *mid,
               const char *num, const char *price, const char *state,
               const char *ftime, const char *mode, const char *value);         //增加一条订单
int UpdateGoodsNum(ExecSql *execsql, const char *shid, const char *comid, const char *com_num);  //根据店铺ID和商品ID更新库存表
int UpdateMember(ExecSql *execsql, const char *id, const char *total, const char *price);  //更新会员的积分和消费额

//注册一个店铺
int InsertShopToUser(ExecSql *execsql, const char *id, const char *password, const char *time, const char *type);
//添加注册的店铺到market
int InsertShopToMarket(ExecSql *execsql, const char *id, const char *name, const char *phone, const char *site);

//更新密码
int UpdateUserPassword(ExecSql *execsql, const char *id, const char *password);
#endif // EXECSQL_H
