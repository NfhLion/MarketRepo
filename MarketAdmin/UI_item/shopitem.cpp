#include "shopitem.h"
#include "ui_shopitem.h"

ShopItem::ShopItem(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ShopItem)
{
    ui->setupUi(this);
    ui->pb_enter->setIcon(QPixmap(":/ico/back.png"));
}

ShopItem::~ShopItem()
{
    delete ui;
}

void ShopItem::SetShopID(const QString &id)
{
    m_shid = id;
    ui->lb_shid->setText(id);
}

void ShopItem::SetName(const QString &name)
{
    m_name = name;
    ui->lb_shname->setText(name);
}

void ShopItem::SetPhone(const QString &phone)
{
    m_phone = phone;
    ui->lb_phone->setText(phone);
}

void ShopItem::SetSite(const QString &site)
{
    m_site = site;
    ui->lb_site->setText(site);
}

void ShopItem::SetTime(const QString &time)
{
    m_time = time;
    ui->lb_time->setText(time);
}

void ShopItem::SetHead(const QImage &image)
{
    m_image = image;
    QSize laSize = ui->label->size();//label_carema是要显示图片的label的名称
    QImage image1=image.scaled(laSize,Qt::IgnoreAspectRatio);//重新调整图像大小以适应窗口
    ui->label->setPixmap(QPixmap::fromImage(image1));
}

void ShopItem::SetNumber(const int &number)
{
    m_number = QString("%1").arg(number);
    ui->lb_num->setText(QString("%1").arg(number));
}

const QString ShopItem::GetShopID()
{
    return m_shid;
}

const QString ShopItem::GetName()
{
    return m_name;
}

const QString ShopItem::GetPhone()
{
    return m_phone;
}

const QString ShopItem::GetSite()
{
    return m_site;
}

const QString ShopItem::GetTime()
{
    return m_time;
}

const QString ShopItem::GetNumber()
{
    return m_number;
}

const QImage ShopItem::GetHead()
{
    return m_image;
}

void ShopItem::on_pb_enter_clicked()
{
    emit SignalAllShopInfo(m_number.toInt());
}
