#include "msgproc.h"
#include <QDebug>

MsgProc::MsgProc(QThread *parent) : QThread(parent)
{
    m_isExit = false;
}

void MsgProc::ExitThread()
{
    m_isExit = true;

}

MsgProc::~MsgProc()
{
    ExitThread();
    this->quit();
    this->wait();
}

///线程运行
void MsgProc::run()
{
    while(!m_isExit)
    {
        if(!GlobalVariable::g_msgQueue.isEmpty())
        {
            QByteArray data = GlobalVariable::g_msgQueue.dequeue();
            ParseUserAsk(data);
        }
        msleep(20);
    }
}

///解析用户的请求
void MsgProc::ParseUserAsk(const QByteArray &data)
{
    QJsonParseError jsonError;
    // 转化为 JSON 文档
    QJsonDocument doucment = QJsonDocument::fromJson(data, &jsonError);
    // 解析未发生错误
    if (!doucment.isNull() && (jsonError.error == QJsonParseError::NoError)){
        QJsonObject obj = QJsonDocument::fromJson(data).object();
        //分析请求类型
        if(!obj.value("type").isNull()){
            int cmd_type = obj.value("type").toInt();
            QJsonObject new_obj = obj.value("data").toObject();
            switch(cmd_type)
            {
            case CMD_UserLogin_C :          ParseUserLogin(new_obj); break;
            case CMD_UserExit_X :           ParseUserLogout(new_obj); break;
            case ORD_UserOnLine :           ParseUserOnLine(cmd_type, new_obj); break;
            case ORD_UserOffLine :          ParseUserOffLine(cmd_type, new_obj); break;
            case ORD_SendMsg :              ParseSendMsg(cmd_type, new_obj); break;
            case ORD_NoReadMsg :            ParseNoReadMsg(cmd_type, new_obj); break;
            case ORD_GetMyFriends :         ParseGetMyFriends(cmd_type, new_obj); break;
            case ORD_SearchFriend :         ParseSearchFriendForID(cmd_type, new_obj); break;
            case ORD_InsertFriend :         ParseInsertUser(cmd_type, new_obj); break;
            case ORD_InsertUserResult :     ParseInsertUserResult(cmd_type, new_obj); break;

            case CMD_AllCommodity_D :       ParseAllCommodityInfo(cmd_type, new_obj); break;
            case CMD_AllShopInfo_J :        ParseAllShopInfo(cmd_type, new_obj); break;
            case CMD_CommodityForShop_K :   ParseCommodityForShop(cmd_type, new_obj); break;
            case CMD_GetRepertoryLocal_L :  ParseGetRepertoryLocal(cmd_type, new_obj); break;
            case CMD_GetRepertoryShip_M :   ParseGetRepertoryShip(cmd_type, new_obj); break;
            case CMD_GetCommodityForID_N :  ParseGetCommodityForID(cmd_type, new_obj); break;
            case CMD_UpdateShipment_R :     ParseUpdateShipment(cmd_type, new_obj); break;
            case CMD_InsertShop :           ParseInsertShop(cmd_type, new_obj); break;
            case CMD_UpdatePassword :       ParseUpdatePassword(cmd_type, new_obj); break;
            }
        }else{
            QMessageBox::critical(0, "消息类型", "消息类型判断失败!");
        }
    }else{
        QMessageBox::critical(0, "消息解析", "Json消息解析失败!");
    }
}

///解析登录请求信息
void MsgProc::ParseUserLogin(const QJsonObject &obj)
{
    if(!obj.value("result").isNull()){
        int cmd_result = obj.value("result").toInt();

        emit SignalReturnUserLogin(cmd_result);
    }else{
        QMessageBox::critical(0, "消息返回判断", "[登录]消息返回判断失败!");
    }
}

///解析退出请求信息
void MsgProc::ParseUserLogout(const QJsonObject &obj)
{
    if(!obj.value("result").isNull()){
        int cmd_result = obj.value("result").toInt();

        emit SignalReturnUserLogout(cmd_result);
    }else{
        QMessageBox::critical(0, "消息返回判断", "[退出系统]消息返回判断失败!");
    }
}

///解析好友上线
void MsgProc::ParseUserOnLine(const int cmd_type,const QJsonObject &obj)
{
    if(!obj.value("result").isNull()){
        int cmd_result = obj.value("result").toInt();
        QJsonValue value(obj);
        emit SignalReturnParseResult(cmd_type, cmd_result, value);
    }else{
        QMessageBox::critical(0, "消息返回判断", "[好友上线]消息返回判断失败!");
    }
}

///解析好友下线
void MsgProc::ParseUserOffLine(const int cmd_type, const QJsonObject &obj)
{
    if(!obj.value("result").isNull()){
        int cmd_result = obj.value("result").toInt();
        QJsonValue value(obj);
        emit SignalReturnParseResult(cmd_type, cmd_result, value);
    }else{
        QMessageBox::critical(0, "消息返回判断", "[好友下线]消息返回判断失败!");
    }
}

///解析好友发过来的消息
void MsgProc::ParseSendMsg(const int cmd_type, const QJsonObject &obj)
{
    if(!obj.value("result").isNull()){
        int cmd_result = obj.value("result").toInt();
        QJsonValue value = obj.value("fmsg");
        emit SignalReturnParseResult(cmd_type, cmd_result, value);
    }else{
        QMessageBox::critical(0, "消息返回判断", "[好友发送]消息返回判断失败!");
    }
}

///解析未读的聊天记录
void MsgProc::ParseNoReadMsg(const int cmd_type, const QJsonObject &obj)
{
    if(!obj.value("result").isNull()){
        int cmd_result = obj.value("result").toInt();
        QJsonValue value = obj.value("array");
        emit SignalReturnParseResult(cmd_type, cmd_result, value);
    }else{
        QMessageBox::critical(0, "消息返回判断", "[好友发送]消息返回判断失败!");
    }
}

///获取好友的状态
void MsgProc::ParseGetMyFriends(const int cmd_type, const QJsonObject &obj)
{
    if(!obj.value("result").isNull()){
        int cmd_result = obj.value("result").toInt();
        QJsonValue value = obj.value("array");
        emit SignalReturnParseResult(cmd_type, cmd_result, value);
    }else{
        QMessageBox::critical(0, "消息返回判断", "[获取好友的状态]消息返回判断失败!");
    }
}

///查找人根据ID
void MsgProc::ParseSearchFriendForID(const int cmd_type, const QJsonObject &obj)
{
    if(!obj.value("result").isNull()){
        int cmd_result = obj.value("result").toInt();
        QJsonValue value = obj.value("obj");
        emit SignalReturnParseResult(cmd_type, cmd_result, value);
    }else{
        QMessageBox::critical(0, "消息返回判断", "[查找人根据ID]消息返回判断失败!");
    }
}

///添加好友
void MsgProc::ParseInsertUser(const int cmd_type, const QJsonObject &obj)
{
    if(!obj.value("result").isNull()){
        int cmd_result = obj.value("result").toInt();
        QJsonValue value(obj);
        emit SignalReturnParseResult(cmd_type, cmd_result, value);
    }else{
        QMessageBox::critical(0, "消息返回判断", "[添加好友]消息返回判断失败!");
    }
}

///添加好友结果
void MsgProc::ParseInsertUserResult(const int cmd_type, const QJsonObject &obj)
{
    if(!obj.value("result").isNull()){
        int cmd_result = obj.value("result").toInt();
        QJsonValue value(obj);
        emit SignalReturnParseResult(cmd_type, cmd_result, value);
    }else{
        QMessageBox::critical(0, "消息返回判断", "[添加好友结果]消息返回判断失败!");
    }
}

///解析远程获取所有商品请求信息
void MsgProc::ParseAllCommodityInfo(const int cmd_type, const QJsonObject &obj)
{
    if(!obj.value("result").isNull()){
        int cmd_result = obj.value("result").toInt();
        QJsonValue value = obj.value("array");
        emit SignalReturnParseResult(cmd_type, cmd_result, value);
    }else{
        QMessageBox::critical(0, "消息返回判断", "[远程获取所有商品信息]消息返回判断失败!");
    }
}

///解析远程获取所有店铺请求信息
void MsgProc::ParseAllShopInfo(const int cmd_type, const QJsonObject &obj)
{
    if(!obj.value("result").isNull()){
        int cmd_result = obj.value("result").toInt();
        QJsonValue value = obj.value("array");
        emit SignalReturnParseResult(cmd_type, cmd_result, value);
    }else{
        QMessageBox::critical(0, "消息返回判断", "[远程获取所有店铺信息]消息返回判断失败!");
    }
}

///解析远程获取单个店铺的商品库存信息
void MsgProc::ParseCommodityForShop(const int cmd_type, const QJsonObject &obj)
{
    if(!obj.value("result").isNull()){
        int cmd_result = obj.value("result").toInt();
        QJsonValue value = obj.value("array");
        emit SignalReturnParseResult(cmd_type, cmd_result, value);
    }else{
        QMessageBox::critical(0, "消息返回判断", "[远程获取所有店铺信息]消息返回判断失败!");
    }
}

///解析远程获取商家的库存
void MsgProc::ParseGetRepertoryLocal(const int cmd_type, const QJsonObject &obj)
{
    if(!obj.value("result").isNull()){
        int cmd_result = obj.value("result").toInt();
        QJsonValue value = obj.value("array");
        emit SignalReturnParseResult(cmd_type, cmd_result, value);
    }else{
        QMessageBox::critical(0, "消息返回判断", "[远程获取商家的库存]消息返回判断失败!");
    }
}

///远程获取调货信息
void MsgProc::ParseGetRepertoryShip(const int cmd_type, const QJsonObject &obj)
{
    if(!obj.value("result").isNull()){
        int cmd_result = obj.value("result").toInt();
        QJsonValue value = obj.value("array");
        emit SignalReturnParseResult(cmd_type, cmd_result, value);
    }else{
        QMessageBox::critical(0, "消息返回判断", "[远程获取调货信息]消息返回判断失败!");
    }
}

///远程根据ID和数量获取商品库存
void MsgProc::ParseGetCommodityForID(const int cmd_type, const QJsonObject &obj)
{
    if(!obj.value("result").isNull()){
        int cmd_result = obj.value("result").toInt();
        QJsonValue value = obj.value("array");
        emit SignalReturnParseResult(cmd_type, cmd_result, value);
    }else{
        QMessageBox::critical(0, "消息返回判断", "[远程根据ID和数量获取商品库存]消息返回判断失败!");
    }
}

///远程处理调货
void MsgProc::ParseUpdateShipment(const int cmd_type, const QJsonObject &obj)
{
    if(!obj.value("result").isNull()){
        int cmd_result = obj.value("result").toInt();
        QJsonValue value = obj.value("state");
        emit SignalReturnParseResult(cmd_type, cmd_result, value);
    }else{
        QMessageBox::critical(0, "消息返回判断", "[远程处理调货]消息返回判断失败!");
    }
}

///注册一个店铺
void MsgProc::ParseInsertShop(const int cmd_type, const QJsonObject &obj)
{
    if(!obj.value("result").isNull()){
        int cmd_result = obj.value("result").toInt();
        QJsonValue value(obj);
        emit SignalReturnParseResult(cmd_type, cmd_result, value);
    }else{
        QMessageBox::critical(0, "消息返回判断", "[注册一个店铺]消息返回判断失败!");
    }
}

///修改密码
void MsgProc::ParseUpdatePassword(const int cmd_type, const QJsonObject &obj)
{
    if(!obj.value("result").isNull()){
        int cmd_result = obj.value("result").toInt();
        QJsonValue value(obj);
        emit SignalReturnParseResult(cmd_type, cmd_result, value);
    }else{
        QMessageBox::critical(0, "消息返回判断", "[修改密码]消息返回判断失败!");
    }
}
