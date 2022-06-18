#include "shopwidget.h"
#include "ui_shopwidget.h"

ShopWidget::ShopWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ShopWidget)
{
    ui->setupUi(this);

    m_shopInfo = new ShopInfo(ui->widget);
    m_insertShop = new InsertShop(ui->widget);
    m_shopInfo->show();
    m_insertShop->hide();

    ui->pb_all->setIcon(QPixmap(":/ico/commodity.png"));
    ui->pb_msg->setIcon(QPixmap(":/ico/zhuce.png"));
}

ShopWidget::~ShopWidget()
{
    delete ui;
}

ShopInfo *ShopWidget::GetShopInfo()
{
    return m_shopInfo;
}

InsertShop *ShopWidget::GetInsertShop()
{
    return m_insertShop;
}

void ShopWidget::on_pb_all_clicked()
{
    m_shopInfo->show();
    m_insertShop->hide();
}

void ShopWidget::on_pb_msg_clicked()
{
    m_shopInfo->hide();
    m_insertShop->show();
}
