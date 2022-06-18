#ifndef MERBERINFO_H
#define MERBERINFO_H

#include <QWidget>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>

namespace Ui {
class MemberInfo;
}

class MemberInfo : public QWidget
{
    Q_OBJECT

public:
    explicit MemberInfo(QWidget *parent = 0);
    ~MemberInfo();

    void GetMemberInfo();
    void SetMemberInfo(const int &cmd, const QJsonValue &value);
    void SetMemberTableWidget();
    void SetSearchMember(const int &cmd, const QJsonValue &value);

private slots:
    void on_pb_break_clicked();

    void on_pb_yselect_clicked();

private:
    Ui::MemberInfo *ui;

    QJsonArray m_array;
};

#endif // MERBERINFO_H
