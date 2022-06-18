#ifndef INSERTSHOP_H
#define INSERTSHOP_H

#include <QWidget>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>

namespace Ui {
class InsertShop;
}

class InsertShop : public QWidget
{
    Q_OBJECT

public:
    explicit InsertShop(QWidget *parent = 0);
    ~InsertShop();

    void SetInsertShop(const int &cmd, const QJsonValue &value);

private slots:
    void on_pb_update_clicked();

    void on_pb_ok_clicked();

private:
    Ui::InsertShop *ui;
};

#endif // INSERTSHOP_H
