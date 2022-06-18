#ifndef SHOPINFO_H
#define SHOPINFO_H

#include <QWidget>
#include "shopitem.h"

#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>

#include "histogramset.h"

struct DistroItem
{
    const char *distro;
    const int hits;
    QColor color;
} ;

namespace Ui {
class ShopInfo;
}

class ShopInfo : public QWidget
{
    Q_OBJECT

public:
    explicit ShopInfo(QWidget *parent = 0);
    ~ShopInfo();

    void SetShopInfo();
    void SetShopArray(const int &result_cmd, const QJsonValue &value);
    void SetShopItem();
    void SetShopCommodityAll(const int &result_cmd, const QJsonValue &value);

    //绘制柱状图的主要函数
    void SetBarchart(QString name, int all_num, int sale_num, int shipment_num, int residu_num);
    void setOrientation(QStringList d_distros);

private slots:
    void on_pb_update_clicked();

    void on_pb_cancel_clicked();

    void on_pb_break_clicked();

    void on_pb_ack_clicked();

    void on_tableWidget_clicked(const QModelIndex &index);

public slots:
    void SlotAllShopInfo(int number);

private:
    Ui::ShopInfo *ui;

    ShopItemList *m_shopItemList;
    QJsonArray m_array;

    //绘制直方图所需
    DistroChartItem *d_barChartItem;
};

#endif // SHOPINFO_H
