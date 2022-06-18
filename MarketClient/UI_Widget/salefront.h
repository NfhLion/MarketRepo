#ifndef SALEFRONT_H
#define SALEFRONT_H

#include <QWidget>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>

enum PAY_VALUE{
    PAY_0 = 0,      //现金支付
    PAY_1 = 1,      //微信支付
    PAY_2 = 2       //支付宝支付
};

namespace Ui {
class SaleFront;
}

class SaleFront : public QWidget
{
    Q_OBJECT

public:
    explicit SaleFront(QWidget *parent = 0);
    ~SaleFront();

    void SetMemberInfo(const int &cmd, const QJsonValue &value);
    void SetFinishSale(const int &cmd, const QJsonValue &value);

private slots:
    void on_pb_mselect_clicked();

    void on_pb_cselect_clicked();

    void on_pb_ok_clicked();

    void on_comboBox_currentIndexChanged(int index);

    void on_pb_clear_clicked();

    void on_pb_finish_clicked();

private:
    Ui::SaleFront *ui;

    QJsonObject m_member;
    QJsonObject m_goods;

    QString m_pay;
};

#endif // SALEFRONT_H
