#ifndef BILLWIDGET_H
#define BILLWIDGET_H

#include <QWidget>

namespace Ui {
class BillWidget;
}

class BillWidget : public QWidget
{
    Q_OBJECT

public:
    explicit BillWidget(QWidget *parent = 0);
    ~BillWidget();

private:
    Ui::BillWidget *ui;
};

#endif // BILLWIDGET_H
