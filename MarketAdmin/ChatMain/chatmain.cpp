#include "chatmain.h"
#include "ui_chatmain.h"
#include "FgGroupListWidget.h"
#include "childitem.h"
#include "FgGroupItem.h"

#include "config.h"
#include "control.h"
#include "globalvariable.h"
#include "msgsocket.h"
#include "execsql.h"

#include <QMessageBox>
#include <QTime>
#include <QScrollBar>

ChatMain::ChatMain(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ChatMain)
{
    ui->setupUi(this);

    //设置好友列表
    SetChatList();

    m_searchFriend = NULL;

    m_childItemMap.clear();
    m_childItemList.clear();
    m_chatWidgetList.clear();
    m_chatWidgetMap.clear();

    connect(this, &ChatMain::SignaltSendMsgTimer, this, &ChatMain::SlotSendMsgTimer);
    connect(this, &ChatMain::SignalInsertUserYes, this, &ChatMain::SlotInsertUserYes);
    connect(this, &ChatMain::SignalInsertUserNo, this, &ChatMain::SlotInsertUserNo);
    ui->widget->setEnabled(false);

    ui->pb_addFriend->setIcon(QPixmap(":/ico/insert.png"));
    ui->pb_addtable->setIcon(QPixmap(":/ico/liebiao.png"));
    ui->pb_back->setIcon(QPixmap(":/ico/back.png"));


    m_tableLine = 0;
    ui->tableWidget->clear();
    ui->tableWidget->setColumnCount(3);
    // set header lables
    QStringList headers;
    headers << "申请人" << "操作" << "操作";
    ui->tableWidget->setHorizontalHeaderLabels(headers);
    ui->tableWidget->setRowCount(30);

    //设置横向滚动条
    ui->tableWidget->horizontalScrollBar()->setStyleSheet("QScrollBar{background:transparent; height:10px;}"
                                                          "QScrollBar::handle{background:lightgray; border:2px solid transparent; border-radius:5px;}"
                                                          "QScrollBar::handle:hover{background:gray;}"
                                                          "QScrollBar::sub-line{background:transparent;}"
                                                          "QScrollBar::add-line{background:transparent;}");

    //设置纵向滚动条
    ui->tableWidget->verticalScrollBar()->setStyleSheet("QScrollBar{background-color:rgb(218,222,223); width:10px;}"
                                                         "QScrollBar::handle{background-color:rgb(180, 180, 180); border:2px solid transparent; border-radius:5px;}"
                                                         "QScrollBar::handle:hover{background-color:rgb(139, 139, 139);}"
                                                         "QScrollBar::sub-line{background:transparent;}"
                                                         "QScrollBar::add-line{background:transparent;}");

    //设置横向滚动条
    ui->treeWidget_friend->horizontalScrollBar()->setStyleSheet("QScrollBar{background:transparent; height:10px;}"
                                                          "QScrollBar::handle{background:lightgray; border:2px solid transparent; border-radius:5px;}"
                                                          "QScrollBar::handle:hover{background:gray;}"
                                                          "QScrollBar::sub-line{background:transparent;}"
                                                          "QScrollBar::add-line{background:transparent;}");

    //设置纵向滚动条
    ui->treeWidget_friend->verticalScrollBar()->setStyleSheet("QScrollBar{background-color:rgb(218,222,223); width:10px;}"
                                                         "QScrollBar::handle{background-color:rgb(180, 180, 180); border:2px solid transparent; border-radius:5px;}"
                                                         "QScrollBar::handle:hover{background-color:rgb(139, 139, 139);}"
                                                         "QScrollBar::sub-line{background:transparent;}"
                                                         "QScrollBar::add-line{background:transparent;}");
    //设置横向滚动条
    ui->treeWidget_group->horizontalScrollBar()->setStyleSheet("QScrollBar{background:transparent; height:10px;}"
                                                          "QScrollBar::handle{background:lightgray; border:2px solid transparent; border-radius:5px;}"
                                                          "QScrollBar::handle:hover{background:gray;}"
                                                          "QScrollBar::sub-line{background:transparent;}"
                                                          "QScrollBar::add-line{background:transparent;}");

    //设置纵向滚动条
    ui->treeWidget_group->verticalScrollBar()->setStyleSheet("QScrollBar{background-color:rgb(218,222,223); width:10px;}"
                                                         "QScrollBar::handle{background-color:rgb(180, 180, 180); border:2px solid transparent; border-radius:5px;}"
                                                         "QScrollBar::handle:hover{background-color:rgb(139, 139, 139);}"
                                                         "QScrollBar::sub-line{background:transparent;}"
                                                         "QScrollBar::add-line{background:transparent;}");
}

ChatMain::~ChatMain()
{
    if(m_searchFriend != NULL){
        m_searchFriend->close();
        delete m_searchFriend;
    }

    delete ui;
}

void ChatMain::UpdateLocalFriend(const int &cmd, const QJsonValue &value)
{
    if(cmd == CMD_Sucess_Y){
        QJsonArray array = value.toArray();
        if(array.isEmpty()){
            return ;
        }
        for(int i = 0; i < array.size(); i++){
            QJsonObject obj = array.at(i).toObject();
            //更新本地数据库中我的好友的状态
            ExecSql::GetExecSql()->UpdateMyFriendState(Config::m_strUserName,
                                                       obj.value("id").toString(),
                                                       obj.value("state").toString());
        }

        SetFriendList();
    }
}

///设置初始好友列表
void ChatMain::SetChatList()
{
    int line = 0;   //用来标识第几行好友组

    QString str_line2 = "连锁店";
    QString str_line3 = "顾客";
    ui->treeWidget_friend->insertGroupItem(line, str_line2);
    ui->treeWidget_friend->insertGroupItem(line + 1, str_line3);
}

///设置初始好友列表里的好友
void ChatMain::SetFriendList()
{
    int line = 0;   //用来标识第几行好友组
    m_array = ExecSql::GetExecSql()->SearchMyFriend(Config::m_strUserName);
    for(int i = 0; i < m_array.size(); i++){
        //设置好友列表
        QJsonObject obj = m_array.at(i).toObject();
        ChildItem *chitem = new ChildItem(this);
        chitem->setID(obj.value("id").toString());
        chitem->setName(obj.value("name").toString());
        chitem->setLine(obj.value("state").toString());
        chitem->setState(-1);//设置初始状态
        chitem->setImage(QImage(":/image/shop.png"));
        connect(chitem, &ChildItem::SignalChatMsg, this, &ChatMain::SlotChatMsg);
        connect(chitem, &ChildItem::SignalFriendInfo, this, &ChatMain::SlotFriendInfo);
        m_childItemList.append(chitem);
        m_childItemMap.insert(obj.value("id").toString(), chitem);

        if(obj.value("type").toString() == "shop"){
            ui->treeWidget_friend->addChildItem(ui->treeWidget_friend->topLevelItem(line), chitem);
        }else{
            ui->treeWidget_friend->addChildItem(ui->treeWidget_friend->topLevelItem(line + 1), chitem);
        }
        //给每个好友列表设置定时器进行有好友发消息时通知
        QTimer *msgTimer = new QTimer(this);
        m_msgTimerMap.insert(obj.value("id").toString(), msgTimer);
        connect(msgTimer, &QTimer::timeout, [=]{
            emit SignaltSendMsgTimer(obj.value("id").toString());
        });

        //设置聊天界面
        ChatWidget *cwitem = new ChatWidget(ui->widget);
        cwitem->setID(obj.value("id").toString());
        cwitem->setName(obj.value("name").toString());
        cwitem->setImage(QImage(":/image/shop.png"));
        m_chatWidgetList.append(cwitem);
        m_chatWidgetMap.insert(obj.value("id").toString(), cwitem);
    }
}

///发消息对应的槽函数
void ChatMain::SlotChatMsg(const QString &id)
{
    ui->stackedWidget_chat->setCurrentIndex(1);
    ui->widget->setEnabled(true);
    for(int i = 0; i < m_chatWidgetList.length(); i++){
        if(id != m_chatWidgetList.at(i)->GetID()){
            m_chatWidgetList.at(i)->hide();
            m_chatWidgetList.at(i)->setShow(false);//设置该好友对应的界面不显示
        }
    }

    if(m_msgTimerMap[id]->isActive()){
        m_msgTimerMap[id]->stop();
        m_childItemMap[id]->setState(-1);
        m_childItemMap[id]->m_num = 0;
    }

    m_chatWidgetMap[id]->show();
    m_chatWidgetMap[id]->setShow(true);//设置该好友对应的界面显示
}

///查看好友资料
void ChatMain::SlotFriendInfo(const QString &id)
{
    ui->stackedWidget_chat->setCurrentIndex(0);

    for(int i = 0; i < m_chatWidgetList.length(); i++){
        m_chatWidgetList.at(i)->hide();
        m_chatWidgetList.at(i)->setShow(false);//设置该好友对应的界面不显示
    }

    ui->widget->setEnabled(false);
}

///查找自己的在线好友并通知他们自己上线
void ChatMain::SetUserOnLine()
{
    //查找自己的在线好友并通知他们自己上线
    QJsonArray friend_array = ExecSql::GetExecSql()->SearchMyAllStateFriendID(Config::m_strUserName, QString("在线"));
    if(!friend_array.isEmpty()){
        QJsonObject json;
        json.insert("type", ORD_UserOnLine);
        json.insert("array", friend_array);
        QJsonDocument doc(json);
        QByteArray data = doc.toJson();
        MsgSocket::GetMsgSocket()->SendMessage(data);
    }
    return ;
}

///设置好友发送的消息到列表
void ChatMain::SetFrientSendMsg(const int &cmd, const QJsonValue &value)
{
    if(cmd == CMD_Sucess_Y){
        QJsonObject obj = value.toObject();
        QString msgid = obj.value("msgid").toString();
        QString fid = obj.value("fid").toString();
        QString time = obj.value("time").toString();
        QString msg = obj.value("msg").toString();
        QString size = obj.value("size").toString();

        m_chatWidgetMap[fid]->SetListWidgetRight(fid, time, msg, QImage(":/image/shop.png"));
        //添加聊天记录到数据库
        ExecSql::GetExecSql()->InsertChatMsg(msgid, fid, time, size, msg, QString("read"));
        if(m_chatWidgetMap[fid]->GetShow() == false){//通过判断该好友界面是否显示来进行定时器的设置
            m_msgTimerMap[fid]->start(500);
        }

    }
}

///获取好友的状态
void ChatMain::GetFriendState()
{
    //获取好友状态
    QJsonArray array = ExecSql::GetExecSql()->SearchMyAllFriendID(Config::m_strUserName);
    if(!array.isEmpty()){
        QJsonObject json;
        json.insert("type", ORD_GetMyFriends);
        json.insert("array", array);
        QJsonDocument doc(json);
        QByteArray data = doc.toJson();
        MsgSocket::GetMsgSocket()->SendMessage(data);
    }
}

///获取未读的聊天记录
void ChatMain::GetNoReadMsg()
{
    //获取未读的聊天记录
    QJsonObject json;
    json.insert("type", ORD_NoReadMsg);
    json.insert("userid", Config::m_strUserName);
    QJsonDocument doc(json);
    QByteArray data = doc.toJson();
    MsgSocket::GetMsgSocket()->SendMessage(data);
}

///设置未读的聊天记录
void ChatMain::SetNoReadMsg(const int &cmd, const QJsonValue &value)
{
    if(cmd == CMD_Sucess_Y && value.isArray()){
        QMessageBox::information(this, "未读记录", "您有未读的聊天记录！");
        QJsonArray array = value.toArray();
        for(int i = 0; i < array.size(); i++){
            QJsonObject obj = array.at(i).toObject();
            QString msgid = obj.value("msg_id").toString();
            QString fid = obj.value("msg_fid").toString();

            QString time;
            QTime timeNow = QTime::currentTime();
            time = (QString)((QDate::currentDate().toString(tr("yyyy-MM-dd")))
                                   + tr(" %1").arg(timeNow.toString()));

            QString msg = obj.value("msg_info").toString();
            QString size = obj.value("msg_size").toString();
            m_chatWidgetMap[fid]->SetListWidgetRight(fid, time, msg, QImage(":/image/shop.png"));
            //添加聊天记录到数据库
            ExecSql::GetExecSql()->InsertChatMsg(msgid, fid, time, size, msg, QString("read"));
            if(m_chatWidgetMap[fid]->GetShow() == false && !m_msgTimerMap[fid]->isActive()){//通过判断该好友界面是否显示来进行定时器的设置
                m_msgTimerMap[fid]->start(500);
            }
        }
    }
}

///设置添加好友时查找到的
void ChatMain::SetSearchFriend(const int &cmd, const QJsonValue &value)
{
    if(cmd == CMD_Sucess_Y && value.isObject()){
        QJsonObject obj = value.toObject();
        m_searchFriend->SetState(obj.value("state").toString());
        m_searchFriend->SetId(obj.value("id").toString());
        m_searchFriend->SetName(obj.value("name").toString());
    }else{
        QMessageBox::information(this, "查找用户", "该用户不存在，请检查输入！");
        m_searchFriend->close();
    }
    return ;
}

///设置添加好友
void ChatMain::SetInsertUser(const int &cmd, const QJsonValue &value)
{
    if(cmd == CMD_Sucess_Y){
        QMessageBox::information(this, "添加好友", "您有新的好友请求添加！");

        QJsonObject obj = value.toObject();
        if(m_tableLine >= 30){
            m_tableLine = 0;
            ui->tableWidget->clear();
            ui->tableWidget->setColumnCount(3);
            // set header lables
            QStringList headers;
            headers << "申请人" << "操作" << "操作";
            ui->tableWidget->setHorizontalHeaderLabels(headers);
            ui->tableWidget->setRowCount(30);
        }

        QTableWidgetItem *item = new QTableWidgetItem(obj.value("qname").toString());
        ui->tableWidget->setItem(m_tableLine, 0, item);
        item->setTextAlignment(Qt::AlignCenter);    //文字居中显示


        QPushButton *pBq = new QPushButton();
        pBq->setText("同意");
        connect(pBq, &QPushButton::clicked, [=]{
            emit SignalInsertUserYes(m_tableLine, obj.value("qid").toString(),
                                     obj.value("qname").toString(),
                                     obj.value("qtype").toString());
        });
        // 在QTableWidget中添加控件
        ui->tableWidget->setCellWidget(m_tableLine,1,pBq);

        QPushButton *pBs = new QPushButton();
        pBs->setText("拒绝");
        connect(pBs, &QPushButton::clicked, [=]{
            emit SignalInsertUserNo(m_tableLine, obj.value("qid").toString());
        });
        ui->tableWidget->setCellWidget(m_tableLine,2,pBs);

        m_tableLine++;
    }
}


///同意添加
void ChatMain::SlotInsertUserYes(const int line, const QString &id, const QString &name, const QString &type)
{
    QMessageBox::information(this, "添加好友", "添加成功！");
    ExecSql::GetExecSql()->InsertFriend(Config::m_strUserName, id, name, type, QString("在线"));
    if (line != -1){
        ui->tableWidget->removeRow(line);
    }

    int line_tree = 0;

    ChildItem *chitem = new ChildItem(this);
    chitem->setID(id);
    chitem->setName(name);
    chitem->setLine(QString("在线"));
    chitem->setState(-1);//设置初始状态
    chitem->setImage(QImage(":/image/shop.png"));
    connect(chitem, &ChildItem::SignalChatMsg, this, &ChatMain::SlotChatMsg);
    connect(chitem, &ChildItem::SignalFriendInfo, this, &ChatMain::SlotFriendInfo);
    m_childItemList.append(chitem);
    m_childItemMap.insert(id, chitem);

    if(type == "shop"){
        ui->treeWidget_friend->addChildItem(ui->treeWidget_friend->topLevelItem(line_tree), chitem);
    }else{
        ui->treeWidget_friend->addChildItem(ui->treeWidget_friend->topLevelItem(line_tree + 1), chitem);
    }
    //给每个好友列表设置定时器进行有好友发消息时通知
    QTimer *msgTimer = new QTimer(this);
    m_msgTimerMap.insert(id, msgTimer);
    connect(msgTimer, &QTimer::timeout, [=]{
        emit SignaltSendMsgTimer(id);
    });

    //设置聊天界面
    ChatWidget *cwitem = new ChatWidget(ui->widget);
    cwitem->setID(id);
    cwitem->setName(name);
    cwitem->setImage(QImage(":/image/shop.png"));
    m_chatWidgetList.append(cwitem);
    m_chatWidgetMap.insert(id, cwitem);

    //发送同意消息给对方
    QJsonObject json;
    json.insert("type", ORD_InsertUserResult);
    json.insert("state", QString("Yes"));
    json.insert("mtype", QString("market"));
    json.insert("qid", id);
    QJsonDocument doc(json);
    QByteArray data = doc.toJson();
    MsgSocket::GetMsgSocket()->SendMessage(data);
}
///拒绝添加
void ChatMain::SlotInsertUserNo(const int line, const QString &id)
{
    QMessageBox::information(this, "添加好友", "已拒绝添加！");
    if (line != -1){
        ui->tableWidget->removeRow(line);
    }
    //发送同意消息给对方
    QJsonObject json;
    json.insert("type", ORD_InsertUserResult);
    json.insert("state", QString("No"));
    QJsonDocument doc(json);
    QByteArray data = doc.toJson();
    MsgSocket::GetMsgSocket()->SendMessage(data);
}

///添加好友返回
void ChatMain::SetInsertUserResult(const int &cmd, const QJsonValue &value)
{
    if(cmd == CMD_Sucess_Y){
        QJsonObject obj = value.toObject();
        if(obj.value("state").toString() == "Yes"){
            QJsonObject obj_json = obj.value("obj").toObject();
            QString type = obj.value("qtype").toString();
            QString id = obj_json.value("id").toString();
            QString name = obj_json.value("name").toString();
            QString state = obj_json.value("state").toString();
            //更新数据库
            ExecSql::GetExecSql()->InsertFriend(Config::m_strUserName, id, name, type, state);
            //设置好友列表
            int line_tree = 0;

            ChildItem *chitem = new ChildItem(this);
            chitem->setID(id);
            chitem->setName(name);
            chitem->setLine(state);
            chitem->setState(-1);//设置初始状态
            chitem->setImage(QImage(":/image/shop.png"));
            connect(chitem, &ChildItem::SignalChatMsg, this, &ChatMain::SlotChatMsg);
            connect(chitem, &ChildItem::SignalFriendInfo, this, &ChatMain::SlotFriendInfo);
            m_childItemList.append(chitem);
            m_childItemMap.insert(id, chitem);

            if(type == "shop"){
                ui->treeWidget_friend->addChildItem(ui->treeWidget_friend->topLevelItem(line_tree), chitem);
            }else{
                ui->treeWidget_friend->addChildItem(ui->treeWidget_friend->topLevelItem(line_tree + 1), chitem);
            }
            //给每个好友列表设置定时器进行有好友发消息时通知
            QTimer *msgTimer = new QTimer(this);
            m_msgTimerMap.insert(id, msgTimer);
            connect(msgTimer, &QTimer::timeout, [=]{
                emit SignaltSendMsgTimer(id);
            });

            //设置聊天界面
            ChatWidget *cwitem = new ChatWidget(ui->widget);
            cwitem->setID(id);
            cwitem->setName(name);
            cwitem->setImage(QImage(":/image/shop.png"));
            m_chatWidgetList.append(cwitem);
            m_chatWidgetMap.insert(id, cwitem);

            QMessageBox::information(this, "添加好友", QString("%1已添加你为好友").arg(obj_json.value("id").toString()));
        }else{
            QMessageBox::information(this, "添加好友", QString("%1拒绝添加你为好友").arg(obj.value("id").toString()));
        }
    }
}

///消息定时器对应的槽函数
void ChatMain::SlotSendMsgTimer(const QString &id)
{
    m_childItemMap[id]->setState(m_childItemMap[id]->m_num);
    m_childItemMap[id]->m_num++;
}


///更新好友状态
void ChatMain::UpdateFriendState(const int &cmd, const QJsonValue &value)
{
    if(cmd == CMD_Sucess_Y){
        QJsonObject obj = value.toObject();
        QString id = obj.value("id").toString();
        QString state = obj.value("state").toString();
        ExecSql::GetExecSql()->UpdateMyFriendState(Config::m_strUserName, id, state);
        m_childItemMap[id]->setLine(state);
    }
}

///添加好友
void ChatMain::on_pb_addFriend_clicked()
{
    if(m_searchFriend == NULL){
        m_searchFriend = new SearchFriend;
    }
    m_searchFriend->show();
}

void ChatMain::on_pb_back_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}

void ChatMain::on_pb_addtable_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}
