#ifndef MSGDEAL_H
#define MSGDEAL_H
#include "control.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDebug>

class MsgDeal
{
public:
    static const QByteArray SetUserLoginMsg(const QString &id, const QString &pswd);                //用户登录
    static const QByteArray SetAllCommodityMsg();                                                   //商品信息获取
    static const QByteArray SetCommodityImageMsg(const QString &com_id, const QString &com_path);   //商品图片封装
   // static const QByteArray SortInfoMsg(const CommodityInfoList *comlist, const int &len);      //商品规格信息获取
   // static const QByteArray FormInfoMsg(const SortInfoList *solist, const int &len);            //订单信息获取

    //static const QByteArray MerchantInfoMsg();                                                  //商家信息获取
   // static const QByteArray ImageMsg(const QString &id, const QString &path);                                                         //图片获取

};

#endif // MSGDEAL_H
