#ifndef INSERTMEMBER_H
#define INSERTMEMBER_H

#include <QWidget>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>

namespace Ui {
class InsertMember;
}

class InsertMember : public QWidget
{
    Q_OBJECT

public:
    explicit InsertMember(QWidget *parent = 0);
    ~InsertMember();

    void SetInsertMember(const int &cmd, const QJsonValue &value);

private slots:
    void on_pb_insert_clicked();

    void on_pb_platina_clicked();

    void on_pb_diamond_clicked();

private:
    Ui::InsertMember *ui;
};

#endif // INSERTMEMBER_H
