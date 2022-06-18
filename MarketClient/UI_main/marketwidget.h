#ifndef MARKETWIDGET_H
#define MARKETWIDGET_H

#include <QWidget>
#include <QCloseEvent>
#include <QPaintEvent>
#include <QPainter>
#include <QTimer>
#include <QTime>
#include <QMouseEvent>

#include "homewidget.h"
#include "localwidget.h"
#include "chatmain.h"
#include "memberwidget.h"
#include "salewidget.h"
#include "billwidget.h"

#include "msgproc.h"
#include "userwidget.h"

namespace Ui {
class MarketWidget;
}

class MarketWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MarketWidget(QWidget *parent = 0);
    ~MarketWidget();

    void SetPushButter();
    void GetDateTime();

    void SetMsgProc(MsgProc *msgproc);

    void DealGetFriends(const int &cmd_result, const QJsonValue &value);
    void DealGetNoReadMsg(const int &cmd_result, const QJsonValue &value);
    void DealAllCommodity(const int &cmd_result, const QJsonValue &value);
    void DeadUpdateShipment(const int &cmd_result, const QJsonValue &value);

public slots:
    void SlotReturnParseResult(const int &cmd_type, const int &cmd_result, const QJsonValue &value);
    void SlotReadFileSuccess(const int &cmd);

    void SlotUserLogout(void);
    void SlotReturnUserLogout(const int &cmd);

    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);

protected:
    void closeEvent(QCloseEvent *ev);
protected:
    void paintEvent(QPaintEvent *);

private slots:
    void on_pb_home_clicked();

    void on_pb_local_clicked();

    void on_pb_chat_clicked();

    void on_pb_member_clicked();

    void on_pb_sale_clicked();

    void on_pb_bill_clicked();

    void on_pb_update_clicked();

    void on_pb_close_clicked();

    void on_pb_min_clicked();

    void on_pb_quit_clicked();

    void on_pb_first_clicked();

private:
    Ui::MarketWidget *ui;

    bool m_isCloseSystem;       //是否关闭系统
    bool m_isFirstGet;

    HomeWidget   *m_homeWidget;
    LocalWidget  *m_localWidget;
    ChatMain     *m_chatMainWidget;
    MemberWidget *m_memberWidget;
    SaleWidget   *m_saleWidget;
    BillWidget   *m_billWidget;
    UserWidget   *m_userWidget;
    MsgProc      *m_msgProc;

    QTimer *m_Timer;
    QPoint windowPos;
    QPoint mousePos;
    QPoint dPos;
};

#endif // MARKETWIDGET_H
