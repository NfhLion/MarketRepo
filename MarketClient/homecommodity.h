#ifndef HOMECOMMODITY_H
#define HOMECOMMODITY_H

#include <QWidget>
#include "msgdeal.h"
#include "msgsocket.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>
#include <QDebug>
#include <QImage>

#include "homeitem.h"

enum Search_Condition{
    Search_None = 0,
    Search_ID,
    Search_Name
};


namespace Ui {
class HomeCommodity;
}

class HomeCommodity : public QWidget
{
    Q_OBJECT

public:
    explicit HomeCommodity(QWidget *parent = 0);
    ~HomeCommodity();

    //远程获取商家所有商品信息
    void GetAllCommodityInfo();
    //远程获取商家所有商品的图片
    void GetAllCommodityImage();

    void SetCommodityImage();

    //设置商品信息
    void SetCommodityItem();

    //判断并设置商品value
    void SetAllCommodityInfo(const int &result_cmd, const QJsonValue &value);

private slots:
    void on_pb_break_clicked();

    void on_pb_ack_clicked();

    void on_comboBox_currentIndexChanged(int index);

    void on_pb_select_clicked();

public slots:
    void SlotAllCommodity(int number);

private:
    Ui::HomeCommodity *ui;
    int                 m_x;
    HomeItemList       *m_homeItemList;

    int m_searchCond;
};

#endif // HOMECOMMODITY_H
