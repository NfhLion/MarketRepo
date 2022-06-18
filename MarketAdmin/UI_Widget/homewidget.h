#ifndef HOMEWIDGET_H
#define HOMEWIDGET_H

#include <QWidget>

#include "homecommodity.h"

namespace Ui {
class HomeWidget;
}

class HomeWidget : public QWidget
{
    Q_OBJECT

public:
    explicit HomeWidget(QWidget *parent = 0);
    ~HomeWidget();

    HomeCommodity *GetHomeCommodity();

private slots:

    void on_pb_all_clicked();

private:
    Ui::HomeWidget *ui;

    HomeCommodity *m_homeCommodity;
};

#endif // HOMEWIDGET_H
