#include "leftchatitem.h"
#include "ui_leftchatitem.h"
#include <QDebug>

LeftChatItem::LeftChatItem(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LeftChatItem)
{
    ui->setupUi(this);
}

LeftChatItem::~LeftChatItem()
{
    delete ui;
}

void LeftChatItem::setID(const QString &id)
{
    m_id = id;
}

void LeftChatItem::setTime(const QString &time)
{
    m_time = time;
    ui->lb_time->setText(time);
}

void LeftChatItem::setImage(const QImage &image)
{
    m_image = image;
    QSize laSize = ui->lb_head->size();//label_carema是要显示图片的label的名称
    QImage image1=image.scaled(laSize,Qt::IgnoreAspectRatio);//重新调整图像大小以适应窗口
    ui->lb_head->setPixmap(QPixmap::fromImage(image1));
}

void LeftChatItem::setMsg(const QString &msg)
{
    m_msg = msg;
    ui->textBrowser->setText(msg);
}


