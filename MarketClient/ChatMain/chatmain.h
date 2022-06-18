#ifndef CHATMAIN_H
#define CHATMAIN_H

#include <QWidget>

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QString>
#include <QJsonValue>
#include <QTimer>

#include "chatwidget.h"
#include "childitem.h"
#include "searchfriend.h"

typedef QMap<QString, QTimer*> MsgTimerMap;

namespace Ui {
class ChatMain;
}

class ChatMain : public QWidget
{
    Q_OBJECT

signals:
    void SignaltSendMsgTimer(const QString &id);

    void SignalInsertUserYes(const int line, const QString &id, const QString &name, const QString &type);
    void SignalInsertUserNo(const int line, const QString &id);

public:
    explicit ChatMain(QWidget *parent = 0);
    ~ChatMain();

    void setTree();
    void UpdateLocalFriend(const int &cmd, const QJsonValue &value);
    void UpdateFriendState(const int &cmd, const QJsonValue &value);
    void SetChatList();
    void SetFriendList();
    void SetUserOnLine();

    void SetFrientSendMsg(const int &cmd, const QJsonValue &value);

    void GetFriendState();
    void GetNoReadMsg();
    void SetNoReadMsg(const int &cmd, const QJsonValue &value);

    void SetSearchFriend(const int &cmd, const QJsonValue &value);
    void SetInsertUser(const int &cmd, const QJsonValue &value);
    void SetInsertUserResult(const int &cmd, const QJsonValue &value);
public slots:
    void SlotChatMsg(const QString &id);
    void SlotFriendInfo(const QString &id);

    void SlotSendMsgTimer(const QString &id);

    void SlotInsertUserYes(const int line, const QString &id, const QString &name, const QString &type);
    void SlotInsertUserNo(const int line, const QString &id);

private slots:
    void on_pb_addFriend_clicked();

    void on_pb_back_clicked();

    void on_pb_addtable_clicked();

private:
    Ui::ChatMain *ui;

    int             m_tableLine;

    QJsonArray      m_array;
    ChildItemMap    m_childItemMap;
    ChildItemList   m_childItemList;
    ChatWidgetMap   m_chatWidgetMap;
    ChatWidgetList  m_chatWidgetList;

    SearchFriend*   m_searchFriend;

    MsgTimerMap     m_msgTimerMap;

};

#endif // CHATMAIN_H
