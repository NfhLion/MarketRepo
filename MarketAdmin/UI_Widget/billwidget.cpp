#include "billwidget.h"
#include "ui_billwidget.h"

BillWidget::BillWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::BillWidget)
{
    ui->setupUi(this);

    ui->pb_select->setIcon(QPixmap(":/ico/select.png"));
    ui->pb_break->setIcon(QPixmap(":/ico/shuaxin.png"));
}

BillWidget::~BillWidget()
{
    delete ui;
}
