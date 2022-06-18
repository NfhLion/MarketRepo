#ifndef HOMEWIDGET_H
#define HOMEWIDGET_H

#include <QWidget>

#include "homenotice.h"
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
    void on_pb_msg_clicked();

    void on_pb_all_clicked();

private:
    Ui::HomeWidget *ui;

    HomeNotice    *m_homeNotice;
    HomeCommodity *m_homeCommodity;
};

#endif // HOMEWIDGET_H
