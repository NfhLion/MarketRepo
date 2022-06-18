#include "execsql.h"
#include "connectdatabase.h"
#include "execsql.h"

#include <QDebug>
#include <QMutex>
#include <QTime>
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
    //创建会员用户表
    query.exec("create table MemberUserInfo "
               "(member_id varchar(25) NOT NULL,"
               "member_username varchar(30) NOT NULL,"
               "member_phone varchar(20) NOT NULL,"
               "member_shopid varchar(15) NOT NULL,"
               "member_time varchar(50) default NULL,"
               "member_grade varchar(20) default NULL,"
               "member_total varchar(20) default NULL,"
               "member_price varchar(20) default NULL,"
               "member_discount varchar(10) default NULL,"
               "primary key (member_id)"
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
    //创建本地商品信息表
    query.exec("create table CommodityInfo"
               " (id varchar(30),"
               " gid varchar(20),"
               " mid varchar(20),"
               " all_num varchar(10),"              //总数
               " sale_num varchar(10),"             //售出数
               " residue_num varchar(10),"          //剩余数
               " shipment_num varchar(10),"         //调货数
               " primary key (id)"
               ")");
    //创建商品利润表
    query.exec("create table BillInfo"
               "(id varchar(30),"
               " gid varchar(15),"
               " mid varchar(15)"
               " sale varchar(15),"                 //销售额
               " return varchar(15),"               //利润额
               " primary key (id)"
               ")");

    //创建本地订单表
    query.exec("create table FormInfo"
               "(id varchar(30),"
               " gid varchar(15),"
               " cid varchar(15),"
               " mid varchar(15),"
               " num varchar(20),"
               " price varchar(20),"
               " state varchar(10),"
               " time varchar(50),"
               " mode varchar(10),"
               " value varchar(10),"
               " primary key (id)"
               ")");
    //创建本地进出货记录表
    query.exec("create table RecordInfo"
               "(id varchar(15),"
               " q_id varchar(15),"
               " f_id varchar(15),"
               " com_id varchar(15),"
               " num varchar(10),"
               " time varchar(50),"
               " state varchar(10),"
               " primary key(id)"
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

//添加一个店铺/企业信息
bool ExecSql::AddUserToMarketInfo(const QJsonObject &obj)
{
    QString queryString = QString("insert into MarketInfo values('%1', '%2', '%3', '%4', '%5', '%6')")
                                                .arg(obj.value("id").toString().arg(obj.value("name").toString()
                                                .arg(obj.value("mtype").toString()).arg(obj.value("phone").toString())
                                                .arg(obj.value("site").toString()).arg(obj.value("time").toString())));
    QSqlQuery query(queryString, userdb);
    qDebug() << queryString;
    return query.exec();
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

//查询本地的会员用户
QJsonArray ExecSql::SearchMember(const QString &myid)
{
    QString queryString = QString("select * from MemberUserInfo where member_shopid = '%1'").arg(myid);
    QSqlQuery query(queryString, userdb);
    qDebug() << queryString;
    QJsonArray array;
    while(query.next())
    {
        QJsonObject obj;
        obj.insert("id", query.value(0).toString());
        obj.insert("name", query.value(1).toString());
        obj.insert("phone", query.value(2).toString());
        obj.insert("shopid", query.value(3).toString());
        obj.insert("time", query.value(4).toString());
        obj.insert("grade", query.value(5).toString());
        obj.insert("total", query.value(6).toString());
        obj.insert("price", query.value(7).toString());
        obj.insert("discount", query.value(8).toString());

        array.append(obj);
    }

    return array;
}

//增加一个会员用户
bool ExecSql::InsertMember(const QString &id, const QString &username, const QString &phone, const QString &shopid,
                           const QString &time, const QString &grade, const QString &total, const QString &price, const QString &discount)
{
    QString queryString = QString("insert into MemberUserInfo values('%1', '%2', '%3', '%4', '%5', '%6', '%7', '%8', '%9')")
                                  .arg(id).arg(username).arg(phone).arg(shopid).arg(time).arg(grade).arg(total).arg(price).arg(discount);
    QSqlQuery query(queryString, userdb);
    qDebug() << queryString;
    return query.exec();
}

//更新会员信息
bool ExecSql::UpdateMember(const QString &id, const QString &total, const QString &price)
{
    QString queryString = QString("update MemberUserInfo set member_total = '%1', member_price = '%2' where member_id = '%3'")
                                .arg(total).arg(price).arg(id);
    QSqlQuery query(queryString, userdb);
    qDebug() << queryString;
    return query.exec();
}

//查找本地所有商品
QJsonArray ExecSql::SearchLocalCommodity(const QString &myid)
{
    QString queryString = QString("select * from CommodityInfo where mid = '%1'").arg(myid);
    QSqlQuery query(queryString, commoditydb);
    qDebug() << queryString;
    QJsonArray array;
    while(query.next())
    {
        QJsonObject obj;
        obj.insert("goods_num_id", query.value(0).toString());
        obj.insert("goods_num_gid", query.value(1).toString());
        obj.insert("goods_num_mid", query.value(2).toString());
        obj.insert("goods_num_all", query.value(3).toString());
        obj.insert("goods_num_sale", query.value(4).toString());
        obj.insert("goods_num_residue", query.value(5).toString());
        obj.insert("goods_num_shipment", query.value(6).toString());

        array.append(obj);
    }

    return array;
}

//增加一条商品
bool ExecSql::InsertCommodity(const QString &id, const QString &gid, const QString &mid, const QString &all_num,
                              const QString &sale_num, const QString &residue_num, const QString &shipment_num)
{
    QString queryString = QString("insert into CommodityInfo values('%1', '%2', '%3', '%4', '%5', '%6', '%7')")
                                .arg(id).arg(gid).arg(mid).arg(all_num).arg(sale_num).arg(residue_num).arg(shipment_num);
    QSqlQuery query(queryString, commoditydb);
    qDebug() << queryString;
    return query.exec();
}

//增加一条订单信息
bool ExecSql::InsertForm(const QString &id, const QString &gid, const QString &cid, const QString &mid,
                         const QString &num, const QString &price, const QString &state,
                         const QString &time, const QString &mode, const QString &value)
{
    QString queryString = QString("insert into FormInfo values('%1', '%2', '%3', '%4', '%5', '%6', '%7', '%8', '%9', '%10')")
                                .arg(id).arg(gid).arg(cid).arg(mid).arg(num).arg(price).arg(state)
                                .arg(time).arg(mode).arg(value);
    QSqlQuery query(queryString, commoditydb);
    qDebug() << queryString;
    return query.exec();
}

//查找本地某条商品信息
QJsonObject ExecSql::SearchCommodity(const QString &gid, const QString &mid)
{
    QJsonObject obj;
    QString queryString = QString("select * from CommodityInfo where gid = '%1' and mid = '%2'").arg(gid).arg(mid);
    QSqlQuery query(queryString, commoditydb);
    qDebug() << queryString;
    QJsonArray array;
    if(query.next())
    {
        obj.insert("sale_num", query.value(4).toString());
        obj.insert("residue_num", query.value(5).toString());
        return obj;
    }

    return obj;
}

//更新一条商品库存
bool ExecSql::UpdateCommodity(const QString &gid, const QString &mid, const QString &sale_num, const QString &residue_num)
{
    QString queryString = QString("update CommodityInfo set sale_num = '%1', residue_num = '%2' where gid = '%3' and mid = '%4'")
                                .arg(sale_num).arg(residue_num).arg(gid).arg(mid);
    QSqlQuery query(queryString, commoditydb);
    qDebug() << queryString;
    return query.exec();
}

//根据店铺ID查订单
QJsonArray ExecSql::SearchForm(const QString &shid)
{
    QString queryString = QString("select * from FormInfo where mid = '%1'").arg(shid);
    QSqlQuery query(queryString, commoditydb);
    qDebug() << queryString;
    QJsonArray array;
    while(query.next())
    {
        QJsonObject obj;
        obj.insert("id", query.value(0).toString());
        obj.insert("gid", query.value(1).toString());
        obj.insert("num", query.value(4).toString());
        obj.insert("price", query.value(5).toString());
        obj.insert("state", query.value(6).toString());
        obj.insert("time", query.value(7).toString());
        obj.insert("mode", query.value(8).toString());
        obj.insert("value", query.value(9).toString());

        array.append(obj);
    }

    return array;
}


