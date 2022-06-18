#ifndef REPERTORYLOCAL_H
#define REPERTORYLOCAL_H

#include <QWidget>

#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include "histogramset.h"

namespace Ui {
class RepertoryLocal;
}

class RepertoryLocal : public QWidget
{
    Q_OBJECT

public:
    explicit RepertoryLocal(QWidget *parent = 0);
    ~RepertoryLocal();

    //远程获取商家库存
    void GetRepertoryLocal();
    //设置
    void SetRepertoryLocal(const int &cmd, const QJsonValue &value);

    //绘制柱状图的主要函数
    void SetBarchart(QString name, int all_num, int sale_num, int shipment_num, int residu_num);
    void setOrientation(QStringList d_distros);

private slots:
    void on_tableWidget_clicked(const QModelIndex &index);

    void on_pb_break_clicked();

private:
    Ui::RepertoryLocal *ui;

    QJsonArray m_array;

    //绘制直方图所需
    DistroChartItem *d_barChartItem;
};

#endif // REPERTORYLOCAL_H
