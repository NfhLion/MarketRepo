#include "insertmember.h"
#include "ui_insertmember.h"

#include "control.h"
#include "msgsocket.h"
#include "config.h"
#include "execsql.h"
#include "globalvariable.h"

#include <QMessageBox>
#include <QTime>

InsertMember::InsertMember(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::InsertMember)
{
    ui->setupUi(this);

    ui->le_total->setText("0");
    ui->le_price->setText("0");

    ui->pb_insert->setIcon(QPixmap(":/ico/zhuce.png"));
    ui->pb_platina->setIcon(QPixmap(":/ico/save.png"));
    ui->pb_diamond->setIcon(QPixmap(":/ico/save.png"));
}

InsertMember::~InsertMember()
{
    delete ui;
}

void InsertMember::SetInsertMember(const int &cmd, const QJsonValue &value)
{
    if(cmd == CMD_Sucess_Y){
        QJsonObject obj = value.toObject();
        QString id = obj.value("id").toString();
        ExecSql::GetExecSql()->InsertMember(id, ui->le_name->text(), ui->le_phone->text(), Config::m_strUserName,
                                            ui->le_time->text(), ui->le_grade->text(), ui->le_total->text(),
                                            ui->le_price->text(), ui->le_discount->text());
        QMessageBox::information(this, "注册会员", "注册成功！");
    }else{
        QMessageBox::information(this, "注册会员", "注册失败！");
    }
}

void InsertMember::on_pb_insert_clicked()
{
    QString name = ui->le_name->text();
    QString phone = ui->le_phone->text();
    if(name.isEmpty()){
        QMessageBox::information(this, "注册会员", "注册人不能为空！");
        return ;
    }else if(phone.isEmpty()){
        QMessageBox::information(this, "注册会员", "注册电话不能为空！");
        return ;
    }
    QJsonObject json;
    json.insert("type", CMD_InsertMember);
    json.insert("name", name);
    json.insert("phone", phone);
    json.insert("shopid", Config::m_strUserName);
    json.insert("time", ui->le_time->text());
    json.insert("grade", ui->le_grade->text());
    json.insert("total", ui->le_total->text());
    json.insert("price", ui->le_price->text());
    json.insert("discount", ui->le_discount->text());

    QJsonDocument doc(json);
    QByteArray data = doc.toJson();
    MsgSocket::GetMsgSocket()->SendMessage(data);
}

///选择白金会员
void InsertMember::on_pb_platina_clicked()
{
    ui->le_grade->setText("白金会员");
    ui->le_discount->setText("0.9");

    QString time;
    QTime timeNow = QTime::currentTime();
    time = (QString)((QDate::currentDate().toString(tr("yyyy-MM-dd")))
                           + tr(" %1").arg(timeNow.toString()));
    ui->le_time->setText(time);
}

///选择钻石会员
void InsertMember::on_pb_diamond_clicked()
{
    ui->le_grade->setText("钻石会员");
    ui->le_discount->setText("0.8");

    QString time;
    QTime timeNow = QTime::currentTime();
    time = (QString)((QDate::currentDate().toString(tr("yyyy-MM-dd")))
                           + tr(" %1").arg(timeNow.toString()));
    ui->le_time->setText(time);
}
