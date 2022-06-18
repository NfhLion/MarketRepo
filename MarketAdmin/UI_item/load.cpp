#include "load.h"
#include "ui_load.h"
#include <QMovie>
#include <QSize>
Load::Load(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Load)
{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground);

    QMovie *movie = new QMovie(":/image/Main/load.gif");
    movie->setScaledSize(QSize(210, 210));
    ui->lb_load->setMovie(movie);
    movie->start();
}

Load::~Load()
{
    delete ui;
}
