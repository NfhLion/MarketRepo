#ifndef CONTROL_H
#define CONTROL_H

#include <QObject>

enum MsgType{
    MsgText = 0,
    MsgFile = 1,
};

enum Oper_Data{
    Oper_None,
    Oper_Add,
    Oper_Del,
    Oper_Mdy
};

enum COMMAND{
    ///通用请求命令
    CMD_ConnectedHost_A     = 'A',                 //链接服务器
    CMD_DisConnectedHost_B  = 'B',                 //断开服务器
    CMD_UserLogin_C         = 'C',                 //用户登录
    CMD_AllCommodity_D      = 'D',                 //获取商家所有商品
    CMD_AllShopInfo_J       = 'J',                 //获取所有的店铺信息
    CMD_CommodityForShop_K  = 'K',                 //获取单个店铺的商品库存
    CMD_GetRepertoryLocal_L = 'L',                 //获取商家所有库存
    CMD_GetRepertoryShip_M  = 'M',                 //获取所有的调货信息
    CMD_GetCommodityForID_N = 'N',                 //根据商品的ID和数量获取其所在的店铺
    CMD_OKUpdateShipment_Q  = 'Q',                 //处理调货请求
    CMD_UpdateShipment_R    = 'R',                 //更新客户端的调货信息页面请求

    CMD_UserExit_X          = 'X',                 //用户退出

    CMD_InsertShop          = 1010,                //注册店铺
    CMD_UpdatePassword      = 1011,                //修改密码




    ///文件请求命令
    CMD_FileUserID_E       = 'E',                 //用户的ID
    CMD_DownLoad_F         = 'F',                 //下载文件
    CMD_UploadFile_G       = 'G',                 //上传文件
    CMD_TranspondFile_H    = 'H',                 //转发文件
    CMD_GetFileHead_I      = 'I',                 //接收头部成功

    ///请求结果命令
    CMD_Sucess_Y           = 'Y',                 //请求成功
    CMD_Failed_Z           = 'Z',                 //请求失败
};

enum CHAT_MSG_ORDER{
    ORD_UserOnLine          = 130,     // 用户上线通知
    ORD_UserOffLine         = 131,     // 用户下线通知

    ORD_InsertFriend        = 132,     // 添加好友
    ORD_InsertUserResult    = 133,     // 添加好友返回

    ORD_GetMyFriends        = 136,     // 上线获取我的好友的状态
    ORD_GetMyGroups         = 137,     // 获取我的群组信息
    ORD_SendMsg             = 138,     // 发送消息
    ORD_NoReadMsg           = 139,     // 未读的聊天记录
    ORD_SearchFriend        = 140,     // 根据ID查找好友
//    CreateGroup             = 0x25,     // 创建群组
//    RefreshFriends          = 0x35,     // 刷新好友状态
//    RefreshGroups,                  // 刷新群组成员状态


//    SendGroupMsg,                   // 发送群组消息
//    SendFile,                       // 发送文件
//    SendPicture,                    // 发送图片
//    SendFace,                       // 发送表情

//    ChangePasswd        = 0x50,     // 修改密码

//    DeleteFriend        = 0x55,     // 删除好友
//    DeleteGroup,                    // 退出群组

//    SendFileOk          = 0x60,     // 文件发送完成状态

//    GetFile             = 0x65,     // 获取文件（到服务器下载文件）
//    GetPicture,                     // 图片下载

};


class Control
{
public:


};

#endif // CONTROL_H
