#include "childitem.h"
#include "ui_childitem.h"

ChildItem::ChildItem(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ChildItem)
{
    ui->setupUi(this);
    m_num = 0;

    ui->pb_msg->setIcon(QPixmap(":/ico/back.png"));
    ui->pb_info->setIcon(QPixmap(":/ico/all.png"));
}

ChildItem::~ChildItem()
{
    delete ui;
}

void ChildItem::setID(const QString &id)
{
    m_id = id;
}

void ChildItem::setName(const QString &name)
{
    m_name = name;
    ui->lb_name->setText(name);
}

void ChildItem::setLine(const QString &line)
{
    m_line = line;
    ui->lb_line->setText(line);
    if(line == "离线"){
        ui->lb_line->setStyleSheet("color: rgb(255, 0, 0);");
    }else{
        ui->lb_line->setStyleSheet("color: rgb(0, 255, 0);");
    }
}


void ChildItem::setImage(const QImage &image)
{
    m_image = image;
    QSize laSize = ui->lb_head->size();//label_carema是要显示图片的label的名称
    QImage image1=image.scaled(laSize,Qt::IgnoreAspectRatio);//重新调整图像大小以适应窗口
    ui->lb_head->setPixmap(QPixmap::fromImage(image1));
}

void ChildItem::setState(const int num)
{
    if(num >= 0 && (num % 2 == 0)){
        ui->lb_state->setStyleSheet("background-color: rgb(255, 0, 0);");
    }else if(num >= 0 && (num % 2 != 0)){
        ui->lb_state->setStyleSheet("background-color: rgb(0, 255, 0);");
    }else if(num < 0){
        ui->lb_state->setStyleSheet("background-color: rgb(180, 180, 180);");
    }
}

const QString ChildItem::GetID() const
{
    return m_id;
}

const QString ChildItem::GetName() const
{
    return m_name;
}

const QString ChildItem::GetLine() const
{
    return m_line;
}

const QImage ChildItem::GetImage() const
{
    return m_image;
}

void ChildItem::on_pb_msg_clicked()
{
    emit SignalChatMsg(m_id);
}

void ChildItem::on_pb_info_clicked()
{
    emit SignalFriendInfo(m_id);
}
