#include "localwidget.h"
#include "ui_localwidget.h"

LocalWidget::LocalWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LocalWidget)
{
    ui->setupUi(this);
    m_localRecord = new LocalRecord(ui->widget);
    m_localCommodity = NULL;
    m_localRecord->show();

    ui->pb_come->setIcon(QPixmap(":/ico/shenqing.png"));
    ui->pb_localcom->setIcon(QPixmap(":/ico/shop.png"));
}

LocalWidget::~LocalWidget()
{
    delete ui;
}

LocalRecord *LocalWidget::GetLocalRecord()
{
    return m_localRecord;
}

LocalCommodity *LocalWidget::GetLocalCommodity()
{
    return m_localCommodity;
}

void LocalWidget::on_pb_localcom_clicked()
{
    if(m_localCommodity == NULL){
        m_localCommodity = new LocalCommodity(ui->widget);
    }
    m_localCommodity->show();
    m_localRecord->hide();
}


void LocalWidget::on_pb_come_clicked()
{
    m_localRecord->show();
    if(m_localCommodity != NULL){
        m_localCommodity->hide();
    }
}
