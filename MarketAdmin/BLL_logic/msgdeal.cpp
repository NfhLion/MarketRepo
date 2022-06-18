#include "msgdeal.h"

///用户登录
const QByteArray MsgDeal::SetUserLoginMsg(const QString &id, const QString &pswd)
{    
    QJsonObject json;
    json.insert("msg_type", MsgText);
    json.insert("type", CMD_UserLogin_C);
    json.insert("user_id", id);
    json.insert("user_pswd", pswd);
    QJsonDocument doc(json);
    QByteArray data = doc.toJson();

    return data;
}

const QByteArray MsgDeal::SetAllCommodityMsg()
{
    QJsonObject json;
    //json.insert("msg_type", MsgText);
    json.insert("type", CMD_AllCommodity_D);
    QJsonDocument doc(json);
    QByteArray data = doc.toJson();

    return data;
}

const QByteArray MsgDeal::SetCommodityImageMsg(const QString &com_id, const QString &com_path)
{
    QJsonObject json;
    json.insert("msg_type", MsgFile);
    json.insert("type", CMD_CommodityImage_E);
    json.insert("com_id", com_id);
    json.insert("path", com_path);
    QJsonDocument doc(json);
    QByteArray data = doc.toJson();
    return data;
}



/////商店信息获取
//const QByteArray MsgDeal::ShopInfoMsg(const MerchantInfoList *merlist, const int &len)
//{
//    QJsonObject json;
//    QJsonArray jarr;
//    json.insert("type", CMD_ShopInfo_S);
//    if(len == 0)
//    {
//        json.insert("array", -1);
//        QJsonDocument doc(json);
//        QByteArray data = doc.toJson();

//        return data;
//    }

//    for(int i = 0; i < len; i++)
//    {
//        jarr.append(merlist->at(i).getID());
//    }
//    json.insert("array", jarr);
//    QJsonDocument doc(json);
//    QByteArray data = doc.toJson();

//    return data;
//}

/////商品信息获取
//const QByteArray MsgDeal::CommodityInfoMsg(const ShopInfoList *shlist, const int &len)
//{
//    QJsonObject json;
//    QJsonArray jarr;
//    json.insert("type", CMD_Commodity_C);
//    if(len == 0)
//    {
//        json.insert("array", -1);
//        QJsonDocument doc(json);
//        QByteArray data = doc.toJson();

//        return data;
//    }

//    for(int i = 0; i < len; i++)
//    {
//        jarr.append(shlist->at(i).getID());
//    }
//    json.insert("array", jarr);
//    QJsonDocument doc(json);
//    QByteArray data = doc.toJson();

//    return data;
//}

/////商品规格信息获取
//const QByteArray MsgDeal::SortInfoMsg(const CommodityInfoList *comlist, const int &len)
//{
//    QJsonObject json;
//    QJsonArray jarr;

//    json.insert("type", CMD_SortInfo_O);
//    if(len == 0)
//    {
//        json.insert("array", -1);
//        QJsonDocument doc(json);
//        QByteArray data = doc.toJson();

//        return data;
//    }

//    for(int i = 0; i < len; i++)
//    {
//        jarr.append(comlist->at(i).getID());
//    }
//    json.insert("array", jarr);
//    QJsonDocument doc(json);
//    QByteArray data = doc.toJson();

//    return data;
//}

/////订单信息获取
//const QByteArray MsgDeal::FormInfoMsg(const SortInfoList *solist, const int &len)
//{
//    QJsonObject json;
//    QJsonArray jarr;
//    QByteArray data;

//    json.insert("type", CMD_FormInfo_R);
//    if(len == 0)
//    {
//        json.insert("array", -1);
//        QJsonDocument doc(json);
//        QByteArray data = doc.toJson();

//        return data;
//    }else if(GlobalVars::g_localUser.getType() == "商家")
//    {
//        json.insert("user_type", QString("商家"));
//        for(int i = 0; i < len; i++)
//        {
//            jarr.append(solist->at(i).getID());
//        }
//        json.insert("array", jarr);
//        QJsonDocument doc(json);
//        data = doc.toJson();
//    }else
//    {
//        json.insert("user_type", QString("顾客"));
//        jarr.append(GlobalVars::g_localUser.getID());
//        json.insert("array", jarr);
//        QJsonDocument doc(json);
//        data = doc.toJson();
//    }

//    return data;
//}

/////商家信息获取
//const QByteArray MsgDeal::MerchantInfoMsg()
//{
//    QJsonObject json;
//    json.insert("type", CMD_MerchatInfo);
//    QJsonDocument doc(json);
//    QByteArray data = doc.toJson();

//    return data;
//}

/////图片信息获取
//const QByteArray MsgDeal::ImageMsg(const QString &id, const QString &path)
//{
//    QJsonObject json;
//    json.insert("type", CMD_Image_G);
//    json.insert("id", id);
//    json.insert("path", path);
//    QJsonDocument doc(json);
//    QByteArray data = doc.toJson();

//    return data;
//}
