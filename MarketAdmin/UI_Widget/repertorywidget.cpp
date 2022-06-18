#include "repertorywidget.h"
#include "ui_repertorywidget.h"

RepertoryWidget::RepertoryWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RepertoryWidget)
{
    ui->setupUi(this);
    m_repertoryShip = new RepertoryShip(ui->widget);
    m_repertoryLocal = NULL;
    m_repertoryShip->show();

    ui->pb_remove->setIcon(QPixmap(":/ico/shenqing.png"));
    ui->pb_local->setIcon(QPixmap(":/ico/shop.png"));
}

RepertoryWidget::~RepertoryWidget()
{
    delete ui;
}

RepertoryLocal *RepertoryWidget::GetRepertoryLocal()
{
    return m_repertoryLocal;
}

RepertoryShip *RepertoryWidget::GetRepertoryShip()
{
    return m_repertoryShip;
}

void RepertoryWidget::on_pb_remove_clicked()
{
    if(m_repertoryLocal != NULL){
        m_repertoryLocal->hide();
    }
    m_repertoryShip->show();
}

void RepertoryWidget::on_pb_local_clicked()
{
    if(m_repertoryLocal == NULL){
        m_repertoryLocal = new RepertoryLocal(ui->widget);
        m_repertoryLocal->GetRepertoryLocal();
    }
    m_repertoryLocal->show();
    m_repertoryShip->hide();
}


