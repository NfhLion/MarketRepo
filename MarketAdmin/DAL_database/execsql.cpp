#include "execsql.h"

#include "config.h"
#include <QDebug>
#include <QMutex>
#include <QMessageBox>

ExecSql *ExecSql::m_execsql = NULL;

ExecSql::ExecSql()
{

}

ExecSql::~ExecSql()
{
    if(m_execsql != NULL){
        delete m_execsql;
        m_execsql = NULL;
    }
}

///单例运行
ExecSql *ExecSql::GetExecSql()
{
    static QMutex mutex;
    if(m_execsql == NULL){
        QMutexLocker locker(&mutex);

        if (!m_execsql)
        {
            m_execsql = new ExecSql();
        }
    }

    return m_execsql;
}

///打开数据库
bool ExecSql::OpenUserDb(const QString &databaseName)
{
    userdb = QSqlDatabase::addDatabase("QSQLITE", "connectionUserdb");
    userdb.setDatabaseName(databaseName);

    if(!userdb.open()){
        QMessageBox::warning(0, QObject::tr("User Database Error"),
                                     userdb.lastError().text());        //返回数据库上发生的最后一个错误信息

        return false;
    }

    // 添加数据表
    QSqlQuery query(userdb);
    //创建店铺/商家表
    query.exec("create table MarketInfo "
               "(id varchar(15),"
               " name varchar(30),"
               " type varchar(20),"
               " phone varchar(15), "
               " site varchar(100),"
               " time varchar(50),"
               " primary key (id)"
               ")");

    // 创建我的好友表 id为好友id，userid为当前用户id
    query.exec("create table FriendInfo "
               "(userid varchar(15), "
               "id varchar(15), "
               "name varchar(50),"
               "type varchar(15),"
               "state varchar(10),"
               "primary key (userid, id))");

    // 创建历史聊天表
    query.exec("CREATE TABLE MsgInfo "
               "(msgid varchar(20) PRIMARY KEY, "
               "otherid varchar(15),"
               "time varchar(50), "
               "filesize varchar(30),"
               "msg varchar(1000),"
               "direction varchar(10)"
               ")");

    return true;
}

bool ExecSql::OpenCommodityDb(const QString &databaseName)
{
    commoditydb = QSqlDatabase::addDatabase("QSQLITE", "connectionCommoditydb");
    commoditydb.setDatabaseName(databaseName);
    if(!commoditydb.open()){
        QMessageBox::warning(0, QObject::tr("Commodity Database Error"),
                                     commoditydb.lastError().text());        //返回数据库上发生的最后一个错误信息

        return false;
    }

    // 添加数据表
    QSqlQuery query(commoditydb);
    //创建商品信息表
    query.exec("create table CommodityInfo"
               " (id varchar(15),"
               " name varchar(30),"
               " old_price varchar(10),"            //成本价
               " new_price varchar(10),"            //售价
               " page_num varchar(10),"             //浏览量
               " all_num varchar(10),"              //总数
               " sell_num varchar(10),"             //售出数
               " residue_num varchar(10),"          //剩余数
               " sale varchar(15),"                 //销售额
               " profit varchar(15),"               //利润额
               " primary key (id)"
               ")");
    //创建本地订单表
    query.exec("create table FormInfo"
               "(id varchar(15),"
               " com_id varchar(15),"
               " cli_id varchar(15),"
               " num varchar(10),"
               " all_price varchar(10),"
               " state varchar(10),"
               " time varchar(50),"
               " comment varchar(1000),"
               " primary key (id)"
               ")");
    //创建本地进出货记录表
    query.exec("create table RecordInfo"
               "(q_id varchar(15),"
               " f_id varchar(15),"
               " com_id varchar(15),"
               " num varchar(10),"
               " time varchar(50)"
               ")");

    return true;
}

///关闭数据库
void ExecSql::CloseUserDb(const QString &databaseName)
{
    QSqlDatabase::database(databaseName).close();
}

void ExecSql::CloseCommdityDb(const QString &databaseName)
{
    QSqlDatabase::database(databaseName).close();
}

///===================Userdb数据库操作部分====================///

//添加店铺信息
bool ExecSql::AddAllShopToMarketInfo(const QJsonArray &array)
{
    for(int i = 0; i < array.size(); i++){
        QJsonObject obj = array.at(i).toObject();
        QString qString = QString("select * from MarketInfo where id = '%1'").arg(obj.value("id").toString());
        QSqlQuery querys(qString, userdb);
        qDebug() << qString;
        if(querys.next()){
            QString uString = QString("update MarketInfo set name = '%1', phone = '%2', site = '%3', time = '%4' where id = '%5'")
                                        .arg(obj.value("name").toString()).arg(obj.value("phone").toString())
                                        .arg(obj.value("site").toString()).arg(obj.value("time").toString())
                                        .arg(obj.value("id").toString());
            QSqlQuery queryu(uString, userdb);
            qDebug() << uString;
            queryu.exec(uString);
        }else{
            QString type;
            if(obj.value("id").toString() == Config::m_strUserName){
                type = "merchant";
            }else{
                type = "shop";
            }
            QString queryString = QString("insert into MarketInfo values('%1', '%2', '%3', '%4', '%5', '%6')")
                                                        .arg(obj.value("id").toString()).arg(obj.value("name").toString())
                                                        .arg(type).arg(obj.value("phone").toString())
                                                        .arg(obj.value("site").toString()).arg(obj.value("time").toString());
            QSqlQuery query(queryString, userdb);
            qDebug() << queryString;
            query.exec(queryString);
        }
    }
    return true;
}

//查询所有的店铺信息
QJsonArray ExecSql::SelectAllShopFromMarketInfo()
{
    QJsonArray array;

    QString queryString = QString("select * from MarketInfo where type = 'shop'");
    QSqlQuery query(queryString, userdb);
    qDebug() << queryString;

    while(query.next())
    {
        QJsonObject obj;
        obj.insert("id", query.value(0).toString());
        obj.insert("name", query.value(1).toString());
        obj.insert("phone", query.value(3).toString());
        obj.insert("site", query.value(4).toString());
        obj.insert("time", query.value(5).toString());
        array.append(obj);
    }

    return array;
}

//查询我所有的好友ID
QJsonArray ExecSql::SearchMyAllFriendID(const QString &myid)
{
    QString queryString = QString("select id from FriendInfo where userid = '%1'").arg(myid);
    QSqlQuery query(queryString, userdb);
    qDebug() << queryString;
    QJsonArray array;
    while(query.next())
    {
        QJsonObject obj;
        obj.insert("id", query.value(0).toString());
        array.append(obj);
    }
    return array;
}

//查询所有的在线好友ID
QJsonArray ExecSql::SearchMyAllStateFriendID(const QString &myid, const QString &state)
{
    QString queryString = QString("select id from FriendInfo where userid = '%1' and state = '%2'").arg(myid).arg(state);
    QSqlQuery query(queryString, userdb);
    qDebug() << queryString;
    QJsonArray array;
    while(query.next())
    {
        QJsonObject obj;
        obj.insert("id", query.value(0).toString());
        array.append(obj);
    }
    return array;
}

//查询我的好友
QJsonArray ExecSql::SearchMyFriend(const QString &myid)
{
    QString queryString = QString("select * from FriendInfo where userid = '%1'").arg(myid);
    QSqlQuery query(queryString, userdb);
    qDebug() << queryString;
    QJsonArray array;
    while(query.next())
    {
        QJsonObject obj;
        obj.insert("id", query.value(1).toString());
        obj.insert("name", query.value(2).toString());
        obj.insert("type", query.value(3).toString());
        obj.insert("state", query.value(4).toString());
        array.append(obj);
    }

    return array;
}

//更新好友的状态
bool ExecSql::UpdateMyFriendState(const QString &userid, const QString &id, const QString &state)
{
    QString queryString = QString("update FriendInfo set state = '%1' where userid = '%2' and id = '%3'").arg(state).arg(userid).arg(id);
    QSqlQuery query(queryString, userdb);
    qDebug() << queryString;
    return query.exec();
}

//增加一条聊天记录
bool ExecSql::InsertChatMsg(const QString &msgid, const QString &id, const QString &time,
                            const QString filesize, const QString msg, const QString &direction)
{
    QString queryString = QString("insert into MsgInfo values('%1', '%2', '%3', '%4', '%5', '%6')")
                                  .arg(msgid).arg(id).arg(time).arg(filesize).arg(msg).arg(direction);
    QSqlQuery query(queryString, userdb);
    qDebug() << queryString;
    return query.exec();
}

bool ExecSql::SearchUserForFriend(const QString &mid, const QString &uid)
{
    QString queryString = QString("select * from FriendInfo where userid = '%1' and id = '%2'").arg(mid).arg(uid);
    QSqlQuery query(queryString, userdb);
    qDebug() << queryString;
    int i = 0;
    while(query.next())
    {
        i++;
    }

    if(i > 0){
        return true;
    }
    return false;
}

//添加一个好友
bool ExecSql::InsertFriend(const QString &usid, const QString &id, const QString &name, const QString &type, const QString &state)
{
    QString queryString = QString("insert into FriendInfo values('%1', '%2', '%3', '%4', '%5')")
                                  .arg(usid).arg(id).arg(name).arg(type).arg(state);
    QSqlQuery query(queryString, userdb);
    qDebug() << queryString;
    return query.exec();
}

//添加一个店铺
bool ExecSql::InsertShopToMarket(const QString &id, const QString &name, const QString &type, const QString &phone, const QString &site, const QString &time)
{
    QString queryString = QString("insert into MarketInfo values('%1', '%2', '%3', '%4', '%5', '%6')")
                                  .arg(id).arg(name).arg(type).arg(phone).arg(site).arg(time);
    QSqlQuery query(queryString, userdb);
    qDebug() << queryString;
    return query.exec();
}
