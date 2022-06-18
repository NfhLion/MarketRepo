#include "searchfriend.h"
#include "ui_searchfriend.h"

#include "config.h"
#include "control.h"
#include "globalvariable.h"
#include "msgsocket.h"
#include "execsql.h"

#include <QMessageBox>

SearchFriend::SearchFriend(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SearchFriend)
{
    ui->setupUi(this);
    ui->le_id->clear();
    ui->le_name->clear();
    ui->pb_add->setEnabled(false);
}

SearchFriend::~SearchFriend()
{
    delete ui;
}

void SearchFriend::SetId(const QString &id)
{
    ui->le_id->clear();
    m_id = id;
    ui->le_id->setText(id);
    if(m_state == "在线"){
        ui->pb_add->setEnabled(true);
    }else{
        ui->pb_add->setEnabled(false);
    }
}

void SearchFriend::SetName(const QString &name)
{
    ui->le_name->clear();
    m_name = name;
    ui->le_name->setText(name);
}

void SearchFriend::SetState(const QString &state)
{
    ui->le_state->clear();
    m_state = state;
    ui->le_state->setText(state);
}

const QString SearchFriend::GetId()
{
    return m_id;
}

const QString SearchFriend::GetName()
{
    return m_name;
}

void SearchFriend::on_pb_search_clicked()
{
    QJsonObject json;
    json.insert("type", ORD_SearchFriend);
    json.insert("id", ui->le_searchid->text());
    QJsonDocument doc(json);
    QByteArray data = doc.toJson();
    MsgSocket::GetMsgSocket()->SendMessage(data);
}

void SearchFriend::on_pb_add_clicked()
{
    if(ExecSql::GetExecSql()->SearchUserForFriend(Config::m_strUserName, m_id) == false){
        QJsonObject json;
        json.insert("type", ORD_InsertFriend);
        json.insert("qid", Config::m_strUserName);
        json.insert("qtype", QString("shop"));
        json.insert("userid", m_id);
        QJsonDocument doc(json);
        QByteArray data = doc.toJson();
        MsgSocket::GetMsgSocket()->SendMessage(data);
        ui->pb_add->setEnabled(false);
        QMessageBox::information(this, "添加好友", "已发送请求，等待对方同意!");
    }else{
        QMessageBox::information(this, "添加好友", "该用户已经是你的好友!");
    }
    this->close();
    return ;
}

void SearchFriend::on_pb_close_clicked()
{
    this->close();
}
