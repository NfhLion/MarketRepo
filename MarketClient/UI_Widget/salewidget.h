#ifndef SALEWIDGET_H
#define SALEWIDGET_H

#include <QWidget>
#include "saleform.h"
#include "salefront.h"

namespace Ui {
class SaleWidget;
}

class SaleWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SaleWidget(QWidget *parent = 0);
    ~SaleWidget();

    SaleForm *GetSaleForm();
    SaleFront *GetSaleFront();

private slots:
    void on_pb_come_clicked();

    void on_pb_localcom_clicked();

private:
    Ui::SaleWidget *ui;

    SaleFront *m_saleFront;
    SaleForm  *m_saleForm;
};

#endif // SALEWIDGET_H
