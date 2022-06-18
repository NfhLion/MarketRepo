#ifndef MEMBERWIDGET_H
#define MEMBERWIDGET_H

#include <QWidget>

#include "memberinfo.h"
#include "insertmember.h"

namespace Ui {
class MemberWidget;
}

class MemberWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MemberWidget(QWidget *parent = 0);
    ~MemberWidget();

    MemberInfo *GetMemberInfoWidget();
    InsertMember *GetInsertMember();

private slots:
    void on_pb_come_clicked();

    void on_pb_localcom_clicked();

private:
    Ui::MemberWidget *ui;

    MemberInfo   *m_memberInfo;
    InsertMember *m_insertMember;
};

#endif // MEMBERWIDGET_H
