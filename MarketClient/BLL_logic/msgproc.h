#ifndef MSGPROC_H
#define MSGPROC_H

#include <QThread>

#include "control.h"
#include "config.h"
#include "execsql.h"
#include "globalvariable.h"

#include <QMessageBox>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonParseError>
#include <QJsonArray>

class MsgProc : public QThread
{
    Q_OBJECT

protected:
    void run();

signals:
    void SignalReturnUserLogin(const int &cmd_result);
    void SignalReturnUserLogout(const int &cmd_result);
    void SignalReturnParseResult(const int &cmd_type, const int &cmd_result, const QJsonValue &value);

public slots:

public:
    explicit MsgProc(QThread *parent = nullptr);
     ~MsgProc();

    void ExitThread(void);

private:
    bool m_isExit;

    void ParseUserAsk(const QByteArray &data);

    void ParseUserLogin(const QJsonObject &obj);
    void ParseUserLogout(const QJsonObject &obj);
    void ParseUserOnLine(const int cmd_type, const QJsonObject &obj);
    void ParseUserOffLine(const int cmd_type, const QJsonObject &obj);
    void ParseSendMsg(const int cmd_type, const QJsonObject &obj);
    void ParseNoReadMsg(const int cmd_type, const QJsonObject &obj);
    void ParseGetMyFriends(const int cmd_type, const QJsonObject &obj);
    void ParseSearchFriendForID(const int cmd_type, const QJsonObject &obj);
    void ParseInsertUser(const int cmd_type, const QJsonObject &obj);
    void ParseInsertUserResult(const int cmd_type, const QJsonObject &obj);
    void ParseAllCommodityInfo(const int cmd_type, const QJsonObject &obj);
    void ParseShipmentForID(const int cmd_type, const QJsonObject &obj);
    void ParseUpdateShipment(const int cmd_type, const QJsonObject &obj);
    void ParseGetLocalCommodity(const int cmd_type, const QJsonObject &obj);
    void ParseMemberInfoForID(const int cmd_type, const QJsonObject &obj);
    void ParseSearchMember(const int cmd_type, const QJsonObject &obj);
    void ParseInsertMember(const int cmd_type, const QJsonObject &obj);
    void ParseSaleSearchMember(const int cmd_type, const QJsonObject &obj);
    void ParseFinishSale(const int cmd_type, const QJsonObject &obj);
    void ParseUpdatePassword(const int cmd_type, const QJsonObject &obj);
};

#endif // MSGPROC_H
