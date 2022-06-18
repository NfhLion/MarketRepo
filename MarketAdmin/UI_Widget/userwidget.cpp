#include "userwidget.h"
#include "ui_userwidget.h"

#include "execsql.h"
#include "control.h"
#include "config.h"
#include "msgsocket.h"
#include "globalvariable.h"
#include <QMessageBox>
#include <QTime>


UserWidget::UserWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::UserWidget)
{
    ui->setupUi(this);
    ui->le_id->setText(Config::m_strUserName);

    ui->pb_ok->setIcon(QPixmap(":/ico/save.png"));
    ui->pb_update->setIcon(QPixmap(":/ico/zhuxiao.jpg"));
}

UserWidget::~UserWidget()
{
    delete ui;
}

void UserWidget::on_pb_update_clicked()
{
    ui->le_np->clear();
    ui->le_onp->clear();
}

void UserWidget::on_pb_ok_clicked()
{
    QString np = ui->le_np->text();
    QString onp = ui->le_onp->text();
    if(np.isEmpty()){
        QMessageBox::information(this, "修改密码", "新密码不能为空！");
        return;
    }
    if(onp.isEmpty()){
        QMessageBox::information(this, "修改密码", "确认密码不能为空！");
        return;
    }
    if(np != onp){
        QMessageBox::information(this, "修改密码", "两次输入不一样，请检查！");
        return ;
    }
    QJsonObject json;
    json.insert("type", CMD_UpdatePassword);
    json.insert("id", Config::m_strUserName);
    json.insert("password", onp);
    QJsonDocument doc(json);
    QByteArray data = doc.toJson();
    MsgSocket::GetMsgSocket()->SendMessage(data);
}
