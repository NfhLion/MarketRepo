#ifndef LOCALWIDGET_H
#define LOCALWIDGET_H

#include <QWidget>

#include "localcommodity.h"
#include "localrecord.h"

namespace Ui {
class LocalWidget;
}

class LocalWidget : public QWidget
{
    Q_OBJECT

public:
    explicit LocalWidget(QWidget *parent = 0);
    ~LocalWidget();

    LocalRecord *GetLocalRecord();
    LocalCommodity *GetLocalCommodity();

private slots:
    void on_pb_localcom_clicked();


    void on_pb_come_clicked();

private:
    Ui::LocalWidget *ui;

    LocalCommodity *m_localCommodity;
    LocalRecord    *m_localRecord;
};

#endif // LOCALWIDGET_H
