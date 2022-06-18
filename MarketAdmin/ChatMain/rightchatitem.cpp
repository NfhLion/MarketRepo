#include "rightchatitem.h"
#include "ui_rightchatitem.h"

RightChatItem::RightChatItem(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RightChatItem)
{
    ui->setupUi(this);
}

RightChatItem::~RightChatItem()
{
    delete ui;
}

void RightChatItem::setID(const QString &id)
{
    m_id = id;
}

void RightChatItem::setTime(const QString &time)
{
    m_time = time;
    ui->lb_time->setText(time);
}

void RightChatItem::setImage(const QImage &image)
{
    m_image = image;
    QSize laSize = ui->lb_head->size();//label_carema是要显示图片的label的名称
    QImage image1=image.scaled(laSize,Qt::IgnoreAspectRatio);//重新调整图像大小以适应窗口
    ui->lb_head->setPixmap(QPixmap::fromImage(image1));
}

void RightChatItem::setMsg(const QString &msg)
{
    m_msg = msg;
    ui->textBrowser->setText(msg);
}
