#include "homenotice.h"
#include "ui_homenotice.h"

HomeNotice::HomeNotice(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::HomeNotice)
{
    ui->setupUi(this);
}

HomeNotice::~HomeNotice()
{
    delete ui;
}
