#ifndef LOCALCOMMODITY_H
#define LOCALCOMMODITY_H

#include <QWidget>

#include "control.h"
#include "msgsocket.h"
#include "config.h"
#include "execsql.h"
#include "globalvariable.h"

#include "histogramset.h"

#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>

struct DistroItem
{
    const char *distro;
    const int hits;
    QColor color;
} ;


namespace Ui {
class LocalCommodity;
}

class LocalCommodity : public QWidget
{
    Q_OBJECT

public:
    explicit LocalCommodity(QWidget *parent = 0);
    ~LocalCommodity();

    void GetLocalCommodity();
    void SetLocalCommodity(const int &cmd, const QJsonValue &value);
    void SetLocalCommodityTableWidget();

    //绘制柱状图的主要函数
    void SetBarchart(QString name, int all_num, int sale_num, int shipment_num, int residu_num);
    void setOrientation(QStringList d_distros);

private slots:
    void on_tableWidget_clicked(const QModelIndex &index);

    void on_pb_shipment_clicked();

    void on_pb_break_clicked();

private:
    Ui::LocalCommodity *ui;

    QJsonArray m_array;

    //绘制直方图所需
    DistroChartItem *d_barChartItem;
};

#endif // LOCALCOMMODITY_H
