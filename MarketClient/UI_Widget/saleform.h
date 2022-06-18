#ifndef SALEFORM_H
#define SALEFORM_H

#include <QWidget>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>

namespace Ui {
class SaleForm;
}

class SaleForm : public QWidget
{
    Q_OBJECT

public:
    explicit SaleForm(QWidget *parent = 0);
    ~SaleForm();

    void SetTabelWidget();

private slots:
    void on_pb_break_clicked();

private:
    Ui::SaleForm *ui;

    QJsonArray m_array;
};

#endif // SALEFORM_H
