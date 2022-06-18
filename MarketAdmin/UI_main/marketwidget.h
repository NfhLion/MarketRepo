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
#include "shopwidget.h"
#include "repertorywidget.h"
#include "chatmain.h"
#include "billwidget.h"
#include "userwidget.h"

#include "load.h"
#include "msgproc.h"

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

protected:
    void closeEvent(QCloseEvent *ev);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void paintEvent(QPaintEvent *);

private slots:
    void on_pb_home_clicked();

    void on_pb_shop_clicked();

    void on_pb_repertory_clicked();

    void on_pb_chat_clicked();

    void on_pb_bill_clicked();

    void on_pb_user_clicked();

    void on_pb_min_clicked();

    void on_pb_close_clicked();

    void on_pb_first_clicked();

    void on_pb_quit_clicked();

private:
    Ui::MarketWidget *ui;

    bool m_isCloseSystem;       //是否关闭系统
    bool m_isFirstGet;          //是否第一次进入系统

    HomeWidget      *m_homeWidget;
    ShopWidget      *m_shopWidget;
    RepertoryWidget *m_repertoryWidget;
    ChatMain        *m_chatMainWidget;
    BillWidget      *m_billWidget;
    UserWidget      *m_userWidget;
    MsgProc         *m_msgProc;

    QTimer *m_Timer;
    QPoint windowPos;
    QPoint mousePos;
    QPoint dPos;
};

#endif // MARKETWIDGET_H
