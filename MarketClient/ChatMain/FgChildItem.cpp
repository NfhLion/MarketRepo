#include "FgChildItem.h"
#include "ui_FgChildItem.h"

FgChildItem::FgChildItem(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FgChildItem)
{
    ui->setupUi(this);
}

FgChildItem::~FgChildItem()
{
    delete ui;
}

void FgChildItem::setName(const QString &name)
{
    ui->nameLabel->setText(name);
}

void FgChildItem::setTime(const QString &time)
{
    ui->timeLabel->setText(time);
}

void FgChildItem::setWidth(const QString &width)
{
    ui->widthLabel->setText(width);
}

void FgChildItem::setHeight(const QString &height)
{
    ui->heightLabel->setText(height);
}

void FgChildItem::setRate(const QString &rate)
{
    ui->rateLabel->setText(rate);
}

void FgChildItem::setImage(const QPixmap &pixmap)
{
    ui->imageLabel->setPixmap(pixmap.scaled(ui->imageLabel->size(),Qt::KeepAspectRatio));
}

QString FgChildItem::getName()
{
    return ui->nameLabel->text();
}
