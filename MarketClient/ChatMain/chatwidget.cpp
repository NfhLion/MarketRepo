#include "chatwidget.h"
#include "ui_chatwidget.h"

#include "config.h"
#include "control.h"
#include "globalvariable.h"
#include "msgsocket.h"
#include "execsql.h"

#include <QTime>
#include <QStringBuilder>

ChatWidget::ChatWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ChatWidget)
{
    ui->setupUi(this);
    m_line = 0;
    m_isShow = false;
    ui->listWidget->setFocusPolicy(Qt::NoFocus);        //取消点击一个item时出现的虚线框

    ui->pb_close->setIcon(QPixmap(":/ico/go.jpg"));
    ui->pb_send->setIcon(QPixmap(":/ico/back.png"));
}

ChatWidget::~ChatWidget()
{
    delete ui;
}

void ChatWidget::setID(const QString &id)
{
    m_id = id;
}

void ChatWidget::setName(const QString &name)
{
    ui->groupBox->setTitle(QString("正在与%1聊天。。。").arg(name));
    m_name = name;
}

void ChatWidget::setImage(const QImage &image)
{
    m_image = image;
}

void ChatWidget::setShow(bool isShow)
{
    m_isShow = isShow;
}

const QString ChatWidget::GetID() const
{
    return m_id;
}

const QString ChatWidget::GetName() const
{

    return m_name;
}

const QImage ChatWidget::GetImage() const
{
    return m_image;
}

const bool ChatWidget::GetShow() const
{
    return m_isShow;
}

//设置自己发送的消息框
void ChatWidget::SetListWidgetLeft(const QString &id, const QString time,
                                   const QString &msg, const QImage &image)
{
    LeftChatItem *item = new LeftChatItem(this);
    item->setID(id);
    item->setTime(time);
    item->setImage(image);
    item->setMsg(msg);

    ui->listWidget->addItem("");
    ui->listWidget->item(m_line)->setSizeHint(QSize(715,100));
    ui->listWidget->setItemWidget(ui->listWidget->item(m_line), item);
    m_line++;
    ui->listWidget->setCurrentRow(ui->listWidget->count() - 1); //定位到最下面
}

//设置对方发送的消息框
void ChatWidget::SetListWidgetRight(const QString &id, const QString time,
                                    const QString &msg, const QImage &image)
{
    RightChatItem *item = new RightChatItem(this);
    item->setID(m_id);
    item->setTime(time);
    item->setImage(m_image);
    item->setMsg(msg);

    ui->listWidget->addItem("");
    ui->listWidget->item(m_line)->setSizeHint(QSize(715,100));
    ui->listWidget->setItemWidget(ui->listWidget->item(m_line), item);
    m_line++;
    ui->listWidget->setCurrentRow(ui->listWidget->count() - 1); //定位到最下面
}

//发送聊天记录
void ChatWidget::on_pb_send_clicked()
{
    QTime timeone = QTime::currentTime();
    QString tid;
    tid = tr("%1").arg(timeone.toString());
    QStringList ttid = tid.split(':');
    QString msgid = QString("MSGS%1").arg(QDate::currentDate().toString(tr("yyyyMMdd"))
                                      % ttid.at(0) % ttid.at(1) % ttid.at(2));

    QString time;
    QTime timeNow = QTime::currentTime();
    time = (QString)((QDate::currentDate().toString(tr("yyyy-MM-dd")))
                           + tr(" %1").arg(timeNow.toString()));
    QString msg = ui->textEdit->toPlainText();
    ui->textEdit->clear();

    SetListWidgetLeft(m_id, time, msg, m_image);
    ExecSql::GetExecSql()->InsertChatMsg(msgid, m_id, time, QString("%1").arg(msg.size()), msg, QString("send"));

    //组装消息
    QJsonObject json;
    QJsonObject obj;
    obj.insert("fid", Config::m_strUserName);
    obj.insert("time", time);
    obj.insert("msg", msg);
    obj.insert("size", QString("%1").arg(msg.size()));

    json.insert("type", ORD_SendMsg);
    json.insert("jid", m_id);
    json.insert("fmsg", obj);
    QJsonDocument doc(json);
    QByteArray data = doc.toJson();
    MsgSocket::GetMsgSocket()->SendMessage(data);

}
