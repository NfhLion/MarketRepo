#ifndef REPERTORYWIDGET_H
#define REPERTORYWIDGET_H

#include <QWidget>
#include "repertorylocal.h"
#include "repertoryship.h"

namespace Ui {
class RepertoryWidget;
}

class RepertoryWidget : public QWidget
{
    Q_OBJECT

public:
    explicit RepertoryWidget(QWidget *parent = 0);
    ~RepertoryWidget();

    RepertoryLocal *GetRepertoryLocal();
    RepertoryShip *GetRepertoryShip();

private slots:
    void on_pb_local_clicked();

    void on_pb_remove_clicked();

private:
    Ui::RepertoryWidget *ui;

    RepertoryLocal *m_repertoryLocal;
    RepertoryShip  *m_repertoryShip;
};

#endif // REPERTORYWIDGET_H
