#ifndef LOCALRECORD_H
#define LOCALRECORD_H

#include <QWidget>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>

namespace Ui {
class LocalRecord;
}

class LocalRecord : public QWidget
{
    Q_OBJECT

signals:
    void SignalCanelShipmentInfo(const QString ship_id, const QString ship_fid);
    void SignalConfirmShipment(const QString ship_id, const QString ship_qid, const QString ship_fid,
                               const QString ship_comid, const QString ship_comnum);

public:
    explicit LocalRecord(QWidget *parent = 0);
    ~LocalRecord();

    void GetShipmentForID();
    void SetShipmentForID(const int &cmd, const QJsonValue &value);

private slots:
    void on_pb_break_clicked();

    void SlotCanelShipmentInfo(const QString ship_id, const QString ship_fid);
    void SlotConfirmShipment(const QString ship_id, const QString ship_qid, const QString ship_fid,
                               const QString ship_comid, const QString ship_comnum);

private:
    Ui::LocalRecord *ui;

    QJsonArray m_array;
};

#endif // LOCALRECORD_H
