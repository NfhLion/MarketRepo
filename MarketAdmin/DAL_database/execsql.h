#ifndef EXECSQL_H
#define EXECSQL_H

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>
#include <QSqlTableModel>

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
    bool AddAllShopToMarketInfo(const QJsonArray &array);                   //添加一个店铺/企业信息
    QJsonArray SelectAllShopFromMarketInfo();

    QJsonArray SearchMyFriend(const QString &myid);                                                 //查询我的好友
    QJsonArray SearchMyAllFriendID(const QString &myid);                                            //查询我所有的好友ID
    QJsonArray SearchMyAllStateFriendID(const QString &myid, const QString &state);                 //查询我所有在线的好友ID
    bool UpdateMyFriendState(const QString &userid, const QString &id, const QString &state);       //更新好友的状态
    bool InsertChatMsg(const QString &msgid, const QString &id, const QString &time, const QString filesize, const QString msg, const QString &direction);
    bool SearchUserForFriend(const QString &mid, const QString &uid);                               //判断是否为我的好友
    bool InsertFriend(const QString &usid, const QString &id, const QString &name, const QString &type, const QString &state);
    bool InsertShopToMarket(const QString &id, const QString &name, const QString &type,            //增加一个店铺
                            const QString &phone, const QString &site, const QString &time);


    ///================Commoditydb数据库操作部分==================///
private:
    static ExecSql *m_execsql;

    QSqlDatabase    userdb;
    QSqlDatabase    commoditydb;
};

#endif // EXECSQL_H
