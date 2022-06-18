#include "homewidget.h"
#include "ui_homewidget.h"

HomeWidget::HomeWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::HomeWidget)
{
    ui->setupUi(this);

    m_homeCommodity = new HomeCommodity(ui->widget);


    m_homeCommodity->show();

    ui->pb_all->setIcon(QPixmap(":/ico/commodity.png"));
}

HomeWidget::~HomeWidget()
{
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
    m_homeCommodity->show();
}



