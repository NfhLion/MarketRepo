#include "homewidget.h"
#include "ui_homewidget.h"

HomeWidget::HomeWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::HomeWidget)
{
    ui->setupUi(this);

    m_homeCommodity = new HomeCommodity(ui->widget);
    m_homeNotice    = new HomeNotice(ui->widget);

    m_homeCommodity->show();
    m_homeNotice->hide();

    ui->pb_all->setIcon(QPixmap(":/ico/commodity.png"));
    ui->pb_msg->setIcon(QPixmap(":/ico/msg.png"));
}

HomeWidget::~HomeWidget()
{
    if(m_homeNotice != NULL){
        delete m_homeNotice;
        m_homeNotice = NULL;
    }
    if(m_homeCommodity != NULL){
        delete m_homeCommodity;
        m_homeCommodity = NULL;
    }

    delete ui;
}

HomeCommodity *HomeWidget::GetHomeCommodity()
{
    if(m_homeCommodity == NULL){
        return NULL;
    }
    qDebug() << "GetHomeCommodity";
    return m_homeCommodity;
}

void HomeWidget::on_pb_all_clicked()
{
    m_homeNotice->hide();
    m_homeCommodity->show();
}

void HomeWidget::on_pb_msg_clicked()
{
    m_homeNotice->show();
    m_homeCommodity->hide();
}


