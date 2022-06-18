#ifndef MARKETLOGIN_H
#define MARKETLOGIN_H

#include <QWidget>
#include <QMouseEvent>
#include "msgsocket.h"

#include "msgproc.h"
#include "marketwidget.h"

namespace Ui {
class MarketLogin;
}

class MarketLogin : public QWidget
{
    Q_OBJECT

public:
    explicit MarketLogin(QWidget *parent = 0);
    ~MarketLogin();

    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);

public slots:
    void SlotReturnUserLogin(const int &cmd);



private slots:
    void on_pb_login_clicked();

    void on_pb_modf_clicked();

    void on_pb_set_clicked();

    void on_pb_bank_clicked();

    void on_pb_ok_clicked();

    void on_pb_close_clicked();

    void on_pb_min_clicked();

protected:
    void paintEvent(QPaintEvent *);

private:
    Ui::MarketLogin *ui;

    int           m_isConnected;

    MsgProc       *m_msgProc;
    MarketWidget  *m_marketUI;

    QPoint windowPos;
    QPoint mousePos;
    QPoint dPos;
};

#endif // MARKETLOGIN_H
