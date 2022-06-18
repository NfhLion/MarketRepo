#ifndef SHOPWIDGET_H
#define SHOPWIDGET_H

#include <QWidget>
#include "shopinfo.h"
#include "insertshop.h"

namespace Ui {
class ShopWidget;
}

class ShopWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ShopWidget(QWidget *parent = 0);
    ~ShopWidget();

    ShopInfo *GetShopInfo();
    InsertShop * GetInsertShop();

private slots:
    void on_pb_all_clicked();

    void on_pb_msg_clicked();

private:
    Ui::ShopWidget *ui;

    ShopInfo   *m_shopInfo;
    InsertShop *m_insertShop;
};

#endif // SHOPWIDGET_H
