#ifndef MSGSOCKET_H
#define MSGSOCKET_H

#include "tcpsocket.h"

#include "control.h"
#include "execsql.h"
#include "connectbase.h"
#include "cJSON.h"

#include <string.h>
#include <time.h>

//消息队列
typedef struct MsgQueue{
    char buf[MSGSIZE];
    struct MsgQueue *next;
    struct MsgQueue *prev;
}MsgQueueN;

typedef struct MsgSocket{

    struct MsgQueue *m_msgQueue_first;
    struct MsgQueue *m_msgQueue_tail;

    //设置并添加msgsocket到MsgServer中的m_msgsocketList。回调MsgServer中的SetMsgSocketConnect
    void *arg;
    void (*MsgSocketConnect)(void *arg, struct MsgSocket *msgsocket);

    //查找需要发送的人的m_ID并发送给他，回调MsgServer中的SearchMsgSocket
    int (*SearchMsgSocketSend)(void *arg, char *id, int cmd, char *msg);

    char m_ID[20];                                                  //标示本MsgSocket是哪个用户
    char m_Friend[MSGSIZE];                                          //用来保存本用户退出时需要通知的好友
    TcpSocket *m_tcpSocket;
}MsgSocket;

void MsgSocketCreate(MsgSocket *msgsocket, TcpSocket *socket);
void MsgSocketClose(MsgSocket *msgsocket);                          //由TcpSocket来回调
void RecvMsg(MsgSocket *msgsocket, char *msg);                      //读取客户端发的消息
void SendMessage(MsgSocket *msgsocket, int cmd, char *msg);         //组包并发送消息至客户端

///---------------------通用解析----------------------///
void ParseClientAsk(MsgSocket *msgsocket);                      //客户端消息解析接口
void ParseUserLogin(MsgSocket *msgsocket, char *msg);           //用户登陆解析接口
void ParseUserExit(MsgSocket *msgsocket, char *msg);            //用户退出
void ParseUserOnLine(MsgSocket *msgsocket, char *msg);          //用户上线
void ParseGetMyFriends(MsgSocket *msgsocket, char *msg);        //获取好友的状态
void ParseFriendSendMsg(MsgSocket *msgsocket, char *msg);       //客户端互发送消息
void ParseNoReadMsg(MsgSocket *msgsocket, char *msg);           //获取未读的聊天记录
void ParseSearchFriendForID(MsgSocket *msgsocket, char *msg);   //根据ID查询好友
void ParseInsertUser(MsgSocket *msgsocket, char *msg);          //添加好友
void ParseInsertUserResult(MsgSocket *msgsocket, char *msg);    //添加好友的结果
void ParseCommodityForShop(MsgSocket *msgsocket, char *msg);    //获取单个店铺库存
void ParseUpdatePassword(MsgSocket *msgsocket, char *msg);      //修改密码

///---------------------企业解析----------------------///
void ParseAllCommodity(MsgSocket *msgsocket, char *msg);        //商品信息解析接口
void ParseAllShopInfo(MsgSocket *msgsocket, char *msg);        //店铺信息解析接口
void ParseGetRepertoryLocal(MsgSocket *msgsocket, char *msg);   //获取商家的所有商品库存解析接口
void ParseGetRepertoryShip(MsgSocket *msgsocket, char *msg);   //获取所有调货信息解析接口
void ParseGetCommodityForID(MsgSocket *msgsocket, char *msg);   //根据商品ID和shul获取店铺解析接口
void ParseUpdateShipmentOK(MsgSocket *msgsocket, char *msg);    //处理调货请求成功解析接口
void ParseInsertShop(MsgSocket *msgsocket, char *msg);          //注册店铺

///---------------------店铺解析----------------------///
void ParseShipmentForID(MsgSocket *msgsocket, char *msg);       //根据店铺ID获取它的货单
void ParseInsertCommodity(MsgSocket *msgsocket, char *msg);     //店铺申请调货解析接口
void ParseCanelShipment(MsgSocket *msgsocket, char *msg);       //店铺取消调货解析接口
void ParseConfirmShipment(MsgSocket *msgsocket, char *msg);     //店铺确认收到货解析接口
void ParseGetLocalCommodity(MsgSocket *msgsocket, char *msg);   //获取本地商品解析接口
void ParseMemberInfoForID(MsgSocket *msgsocket, char *msg);     //根据店铺ID获取本地会员信息解析接口
void ParseSearchMember(MsgSocket *msgsocket, char *msg);        //查找会员用户信息解析接口
void ParseInsertMember(MsgSocket *msgsocket, char *msg);        //注册会员
void ParseSaleSearchMember(MsgSocket *msgsocket, char *msg);    //销售查找会员用户信息解析接口
void ParseFinishSale(MsgSocket *msgsocket, char *msg);          //销售结算解析接口

#endif // MSGSOCKET_H
