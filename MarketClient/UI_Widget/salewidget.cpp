#include "salewidget.h"
#include "ui_salewidget.h"

SaleWidget::SaleWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SaleWidget)
{
    ui->setupUi(this);

    m_saleFront = new SaleFront(ui->widget);
    m_saleForm = NULL;
    m_saleFront->show();

    ui->pb_localcom->setIcon(QPixmap(":/ico/form.png"));
    ui->pb_come->setIcon(QPixmap(":/ico/xiaoshou.png"));
}

SaleWidget::~SaleWidget()
{
    delete ui;
}

SaleForm *SaleWidget::GetSaleForm()
{
    return m_saleForm;
}

SaleFront *SaleWidget::GetSaleFront()
{
    return m_saleFront;
}

void SaleWidget::on_pb_come_clicked()
{
    m_saleFront->show();
    m_saleForm->hide();
}

void SaleWidget::on_pb_localcom_clicked()
{
    if(m_saleForm == NULL){
        m_saleForm = new SaleForm(ui->widget);
        m_saleForm->SetTabelWidget();
    }
    m_saleFront->hide();
    m_saleForm->show(); 
}
