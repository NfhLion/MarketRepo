#include "homeitem.h"
#include "ui_homeitem.h"

HomeItem::HomeItem(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::HomeItem)
{
    ui->setupUi(this);
    ui->pb_all->setIcon(QPixmap(":/ico/all.png"));
}

HomeItem::~HomeItem()
{
    delete ui;
}

void HomeItem::SetId(const QString &id)
{
    m_id = id;
    ui->lb_id->setText(id);
}

void HomeItem::SetName(const QString &name)
{
    m_name= name;
    ui->lb_name->setText(name);
}

void HomeItem::SetOldPrice(const QString &price)
{
    m_oldPrice = price;
}

void HomeItem::SetNewPrice(const QString &price)
{
    m_newPrice = price;
}

void HomeItem::SetCallNum(const QString &num)
{
    m_callNum = num;
}

void HomeItem::SetHead(const QImage &image)
{
    m_image = image;
    QSize laSize = ui->label->size();//label_carema是要显示图片的label的名称
    QImage image1=image.scaled(laSize,Qt::IgnoreAspectRatio);//重新调整图像大小以适应窗口
    ui->label->setPixmap(QPixmap::fromImage(image1));
}

void HomeItem::SetNumber(const int &number)
{
    m_number = number;
    ui->lb_number->setText(QString("%1").arg(number));
}

void HomeItem::SetHeadPath(const QString &path)
{
    m_path = path;
}

const QString HomeItem::GetId() const
{
    return m_id;
}

const QString HomeItem::GetName() const
{
    return m_name;
}

const QString HomeItem::GetOldPrice() const
{
    return m_oldPrice;
}

const QString HomeItem::GetNewPrice() const
{
    return m_newPrice;
}

const QString HomeItem::GetCallNum() const
{
    return m_callNum;
}

const QImage HomeItem::GetHead() const
{
    return m_image;
}

const QString HomeItem::GetHeadPath() const
{
    return m_path;
}

void HomeItem::on_pb_all_clicked()
{
    emit SignalAllCommodityInfo(m_number);
}
