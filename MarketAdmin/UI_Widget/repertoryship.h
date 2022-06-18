#ifndef REPERTORYSHIP_H
#define REPERTORYSHIP_H

#include <QWidget>

#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>

namespace Ui {
class RepertoryShip;
}

class RepertoryShip : public QWidget
{
    Q_OBJECT

signals:
    void SignalDealShipmentInfo(const QString &id, const QString &goods_name);


public:
    explicit RepertoryShip(QWidget *parent = 0);
    ~RepertoryShip();

    void GetRepertoryShipment();
    void SetRepertoryShipment(const int &cmd, const QJsonValue &value);

    void SetCommodityTableWidget(const int &cmd, const QJsonValue &value);

public slots:
    void SlotDealShipmentInfo(const QString &id, const QString &goods_name);

private slots:
    void on_pb_ack_clicked();

    void on_pb_analyze_clicked();

    void on_pb_dhcancel_clicked();

    void on_pb_dhok_clicked();

private:
    Ui::RepertoryShip *ui;

    QJsonArray m_array;
    QJsonArray m_comarray;  //供分析最优调货地使用
};

#endif // REPERTORYSHIP_H
