#ifndef EXECSQL_H
#define EXECSQL_H

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>

#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>

class ExecSql
{
public:
    ExecSql();
    ~ExecSql();

    ///单例运行数据库
    static ExecSql *GetExecSql();

    ///打开数据库
    bool OpenUserDb(const QString &databaseName);
    bool OpenCommodityDb(const QString &databaseName);

    ///关闭数据库
    void CloseUserDb(const QString &databaseName);
    void CloseCommdityDb(const QString &databaseName);

    ///===================Userdb数据库操作部分====================///
    bool AddUserToMarketInfo(const QJsonObject &obj);                                               //添加一个店铺/企业信息
    QJsonArray SearchMyFriend(const QString &myid);                                                 //查询我的好友
    QJsonArray SearchMyAllFriendID(const QString &myid);                                            //查询我所有的好友ID
    QJsonArray SearchMyAllStateFriendID(const QString &myid, const QString &state);                 //查询我所有在线的好友ID
    bool UpdateMyFriendState(const QString &userid, const QString &id, const QString &state);       //更新好友的状态
    bool InsertChatMsg(const QString &msgid, const QString &id, const QString &time, const QString filesize, const QString msg, const QString &direction);
    bool SearchUserForFriend(const QString &mid, const QString &uid);                               //判断是否为我的好友
    bool InsertFriend(const QString &usid, const QString &id, const QString &name, const QString &type, const QString &state);

    QJsonArray SearchMember(const QString &myid);                                                  //查询本地的会员用户
    bool InsertMember(const QString &id, const QString &username, const QString &phone, const QString &shopid, const QString &time
                      ,const QString &grade, const QString &total, const QString &price, const QString &discount);

    bool UpdateMember(const QString &id, const QString &total, const QString &price);               //更新本地会员用户的积分和消费额

    ///================Commoditydb数据库操作部分==================///
    QJsonArray SearchLocalCommodity(const QString &myid);                                           //查询本地商品
    bool InsertCommodity(const QString &id, const QString &gid, const QString &mid, const QString &all_num
                      ,const QString &sale_num, const QString &residue_num, const QString &shipment_num);
    bool InsertForm(const QString &id, const QString &gid, const QString &cid, const QString &mid
                    ,const QString &num, const QString &price, const QString &state
                    ,const QString &time, const QString &mode, const QString &value);
    QJsonObject SearchCommodity(const QString &gid, const QString &mid);
    bool UpdateCommodity(const QString &gid, const QString &mid, const QString &sale_num, const QString &residue_num);
    QJsonArray SearchForm(const QString &shid); //根据店铺ID查询所有订单

private:
    static ExecSql *m_execsql;

    QSqlDatabase    userdb;
    QSqlDatabase    commoditydb;
};

#endif // EXECSQL_H
