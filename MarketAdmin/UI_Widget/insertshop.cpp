#include "insertshop.h"
#include "ui_insertshop.h"

#include "execsql.h"
#include "control.h"
#include "config.h"
#include "msgsocket.h"
#include "globalvariable.h"
#include <QMessageBox>
#include <QTime>

InsertShop::InsertShop(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::InsertShop)
{
    ui->setupUi(this);

    ui->pb_ok->setIcon(QPixmap(":/ico/save.png"));
    ui->pb_update->setIcon(QPixmap(":/ico/zhuxiao.jpg"));
}

InsertShop::~InsertShop()
{
    delete ui;
}

void InsertShop::SetInsertShop(const int &cmd, const QJsonValue &value)
{
    if(cmd == CMD_Sucess_Y){
        QJsonObject obj = value.toObject();
        ExecSql::GetExecSql()->InsertShopToMarket(obj.value("id").toString(), obj.value("name").toString(), obj.value("type").toString(),
                                                  obj.value("phone").toString(), obj.value("site").toString(), obj.value("time").toString());
        QMessageBox::information(this, "注册店铺", "注册店铺成功！");
    }
}

void InsertShop::on_pb_update_clicked()
{
    ui->le_id->clear();
    ui->le_name->clear();
    ui->le_password->clear();
    ui->le_phone->clear();
    ui->le_site->clear();
}

void InsertShop::on_pb_ok_clicked()
{
    QString id = ui->le_id->text();
    if(id.isEmpty()){
        QMessageBox::information(this, "注册店铺", "ID不能为空！");
        return;
    }
    QString name = ui->le_name->text();
    if(name.isEmpty()){
        QMessageBox::information(this, "注册店铺", "名字不能为空！");
        return;
    }
    QString password = ui->le_password->text();
    if(password.isEmpty()){
        QMessageBox::information(this, "注册店铺", "密码不能为空！");
        return;
    }
    QString phone = ui->le_phone->text();
    if(phone.isEmpty()){
        QMessageBox::information(this, "注册店铺", "电话不能为空！");
        return;
    }
    QString site = ui->le_site->text();
    if(site.isEmpty()){
        QMessageBox::information(this, "注册店铺", "地址不能为空！");
        return;
    }
    QString time;
    QTime timeNow = QTime::currentTime();
    time = (QString)((QDate::currentDate().toString(tr("yyyy-MM-dd")))
                           + tr(" %1").arg(timeNow.toString()));
    QString shtype = "shop";

    QJsonObject json;
    json.insert("type", CMD_InsertShop);
    json.insert("id", id);
    json.insert("name", name);
    json.insert("password", password);
    json.insert("phone", phone);
    json.insert("site", site);
    json.insert("time", time);
    json.insert("shtype", shtype);
    QJsonDocument doc(json);
    QByteArray data = doc.toJson();
    MsgSocket::GetMsgSocket()->SendMessage(data);
}
