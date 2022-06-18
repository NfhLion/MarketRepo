#include "marketwidget.h"
#include "ui_marketwidget.h"
#include "filesocket.h"

MarketWidget::MarketWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MarketWidget)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::FramelessWindowHint);

    m_isCloseSystem = false;
    m_isFirstGet = true;
    m_Timer = new QTimer(this);
    SetPushButter();

    m_homeWidget = new HomeWidget(ui->widget);
    m_chatMainWidget = new ChatMain(ui->widget);
    m_localWidget = new LocalWidget(ui->widget);
    m_memberWidget = new MemberWidget(ui->widget);
    m_saleWidget = new SaleWidget(ui->widget);
    m_billWidget = new BillWidget(ui->widget);
    m_userWidget = new UserWidget(ui->widget);
    m_homeWidget->show();
    m_chatMainWidget->hide();
    m_localWidget->hide();
    m_memberWidget->hide();
    m_saleWidget->hide();
    m_billWidget->hide();
    m_userWidget->hide();
    m_msgProc = NULL;

    FileSocket::GetFileSocket()->ConnectToHost(Config::m_nFileHostAddr, Config::m_nFilePort);
    connect(FileSocket::GetFileSocket(), &FileSocket::signalStatus, [=](char state){
        switch (state) {
        case CMD_DisConnectedHost_B:
            ui->lb_file->setText("与文件服务器断开");
            break;
        case CMD_ConnectedHost_A:
            QJsonObject obj;
            obj.insert("type", CMD_FileUserID_E);
            obj.insert("id", Config::m_strUserName);
            QJsonDocument doc(obj);
            QByteArray data = doc.toJson();

            if(FileSocket::GetFileSocket()->SendMessage(data)){
                ui->lb_file->setText("已连接文件服务器");
            }
            break;
        }
    });
    connect(FileSocket::GetFileSocket(), &FileSocket::SignalReadFileSuccess,
            this, &MarketWidget::SlotReadFileSuccess);
    connect(m_Timer, &QTimer::timeout, this, &MarketWidget::GetDateTime);
}

MarketWidget::~MarketWidget()
{
    if(m_msgProc != NULL){
        delete m_msgProc;
    }
    if(m_homeWidget != NULL){
        delete m_homeWidget;
        m_homeWidget = NULL;
    }
    delete ui;
}

//获取时间
void MarketWidget::GetDateTime()
{
    QTime timeNow = QTime::currentTime();
    ui->lb_time->setText((QDate::currentDate().toString(tr("yyyy年MM月dd日 dddd")))
                           + tr("  %1").arg(timeNow.toString()));
}

//设置按钮
void MarketWidget::SetPushButter()
{
    ui->pb_home->setIcon(QPixmap(":/ico/first.png"));
    ui->pb_local->setIcon(QPixmap(":/ico/shop.png"));
    ui->pb_sale->setIcon(QPixmap(":/ico/xiaoshou.png"));
    ui->pb_member->setIcon(QPixmap(":/ico/member.png"));
    ui->pb_chat->setIcon(QPixmap(":/ico/diaglo.png"));
    ui->pb_bill->setIcon(QPixmap(":/ico/sort.png"));
    ui->pb_update->setIcon(QPixmap(":/ico/mima.png"));
    ui->pb_first->setIcon(QPixmap(":/ico/firstr.png"));
    ui->pb_quit->setIcon(QPixmap(":/ico/go.jpg"));
}

void MarketWidget::mousePressEvent(QMouseEvent *event)
{
     this->windowPos = this->pos();                // 获得部件当前位置
     this->mousePos = event->globalPos();     // 获得鼠标位置
     this->dPos = mousePos - windowPos;       // 移动后部件所在的位置
}

void MarketWidget::mouseMoveEvent(QMouseEvent *event)
{
//    this->move(event->globalPos() - );
//    event->accept();
    this->move(event->globalPos() - this->dPos);
}

void MarketWidget::closeEvent(QCloseEvent *ev)
{
    if(!m_isCloseSystem)
    {
        SlotUserLogout();
        ev->ignore();
    }else
    {
        ev->accept();
    }
}

void MarketWidget::paintEvent(QPaintEvent *)
{
    QPainter p(this);
    QPixmap pix(":/image/climain.jpg");
    p.drawPixmap(0,0,pix);
}

void MarketWidget::SlotUserLogout(void)
{
    ///用户退出请求
    QJsonArray array = ExecSql::GetExecSql()->SearchMyAllStateFriendID(Config::m_strUserName, "在线");
    QJsonObject json;
    json.insert("type", CMD_UserExit_X);
    if(!array.isEmpty()){
        json.insert("array", array);
    }

    QJsonDocument doc(json);
    QByteArray data = doc.toJson();
    MsgSocket::GetMsgSocket()->SendMessage(data);
}

void MarketWidget::SlotReturnUserLogout(const int &cmd)
{
    if(cmd == CMD_Sucess_Y){
        m_isCloseSystem = true;
        this->close();
    }
}

void MarketWidget::SetMsgProc(MsgProc *msgproc)
{
    m_msgProc = msgproc;



    m_homeWidget->GetHomeCommodity()->GetAllCommodityInfo();
    connect(m_msgProc, &MsgProc::SignalReturnParseResult, this, &MarketWidget::SlotReturnParseResult);
    connect(m_msgProc, &MsgProc::SignalReturnUserLogout, this, &MarketWidget::SlotReturnUserLogout);
    m_Timer->start();
}



void MarketWidget::SlotReturnParseResult(const int &cmd_type, const int &cmd_result, const QJsonValue &value)
{
    switch (cmd_type)
    {
    case CMD_AllCommodity_D :       DealAllCommodity(cmd_result, value); break;
    case CMD_GetShipmentForID_O :   m_localWidget->GetLocalRecord()->SetShipmentForID(cmd_result, value); break;
    case CMD_UpdateShipment_R :     DeadUpdateShipment(cmd_result, value); break;
    case CMD_GLocalCommodity_U :    m_localWidget->GetLocalCommodity()->SetLocalCommodity(cmd_result, value); break;
    case CMD_GetMyMemmber :         m_memberWidget->GetMemberInfoWidget()->SetMemberInfo(cmd_result, value); break;
    case CMD_SearchMember :         m_memberWidget->GetMemberInfoWidget()->SetSearchMember(cmd_result, value); break;
    case CMD_InsertMember :         m_memberWidget->GetInsertMember()->SetInsertMember(cmd_result, value); break;
    case CMD_SaleSearchMember :     m_saleWidget->GetSaleFront()->SetMemberInfo(cmd_result, value); break;
    case CMD_FinishSale :           m_saleWidget->GetSaleFront()->SetFinishSale(cmd_result, value); break;
    case CMD_UpdatePassword :       if(cmd_result == CMD_Sucess_Y){
                                        QMessageBox::information(this, "修改密码", "修改密码成功，请重新登录！");
                                        this->close();
                                    }break;

    case ORD_GetMyFriends :         DealGetFriends(cmd_result, value); break;
    case ORD_UserOnLine :           m_chatMainWidget->UpdateFriendState(cmd_result, value); break;
    case ORD_UserOffLine :          m_chatMainWidget->UpdateFriendState(cmd_result, value); break;
    case ORD_SendMsg :              m_chatMainWidget->SetFrientSendMsg(cmd_result, value); break;
    case ORD_NoReadMsg :            DealGetNoReadMsg(cmd_result, value); break;
    case ORD_SearchFriend :         m_chatMainWidget->SetSearchFriend(cmd_result, value); break;
    case ORD_InsertFriend :         m_chatMainWidget->SetInsertUser(cmd_result, value); break;
    case ORD_InsertUserResult :     m_chatMainWidget->SetInsertUserResult(cmd_result, value); break;
    default:
        break;
    }
}

///获取所有好友状态后要做的事
void MarketWidget::DealGetFriends(const int &cmd_result, const QJsonValue &value)
{
    //获取未读的聊天记录
    m_chatMainWidget->GetNoReadMsg();
    m_chatMainWidget->UpdateLocalFriend(cmd_result, value);
}

///获取未读聊天记录后要做的事
void MarketWidget::DealGetNoReadMsg(const int &cmd_result, const QJsonValue &value)
{
    //通知好友自己上线
    m_chatMainWidget->SetUserOnLine();
    m_chatMainWidget->SetNoReadMsg(cmd_result, value);
}

///获取所有商品信息后
void MarketWidget::DealAllCommodity(const int &cmd_result, const QJsonValue &value)
{
    if(m_isFirstGet == true){
        //获取好友状态
        m_chatMainWidget->GetFriendState();
        m_isFirstGet = false;
    }
    m_homeWidget->GetHomeCommodity()->SetAllCommodityInfo(cmd_result, value);
}


///处理更新调货信息
void MarketWidget::DeadUpdateShipment(const int &cmd_result, const QJsonValue &value)
{
    if(cmd_result == CMD_Sucess_Y){
        QString state = value.toString();
        if(state == "审核中")
            QMessageBox::information(this, "申请调货", "申请调货成功，请等待总部处理！");
        else if(state == "调货中")
            QMessageBox::information(this, "调货处理", "调货已成功，请等待收货后确收到！");
        else if(state == "已取消")
            QMessageBox::information(this, "调货处理", "取消调货成功！");
        else if(state == "已收到")
            QMessageBox::information(this, "调货处理", "调货完成！");
        m_localWidget->GetLocalRecord()->GetShipmentForID();
    }
}

///读取文件成功槽函数
void MarketWidget::SlotReadFileSuccess(const int &cmd)
{
    switch (cmd) {
    case CMD_UploadFile_G : m_homeWidget->GetHomeCommodity()->SetCommodityImage(); break;
    default:
        break;
    }
}

void MarketWidget::on_pb_home_clicked()
{
    m_homeWidget->show();
    m_localWidget->hide();
    m_chatMainWidget->hide();
    m_memberWidget->hide();
    m_saleWidget->hide();
    m_billWidget->hide();
    m_userWidget->hide();
}

void MarketWidget::on_pb_local_clicked()
{
    m_homeWidget->hide();
    m_localWidget->show();
    m_chatMainWidget->hide();
    m_memberWidget->hide();
    m_saleWidget->hide();
    m_billWidget->hide();
    m_userWidget->hide();
    m_localWidget->GetLocalRecord()->GetShipmentForID();
}

void MarketWidget::on_pb_chat_clicked()
{
    m_memberWidget->hide();
    m_chatMainWidget->show();
    m_localWidget->hide();
    m_homeWidget->hide();
    m_saleWidget->hide();
    m_billWidget->hide();
    m_userWidget->hide();
}

void MarketWidget::on_pb_member_clicked()
{
    m_memberWidget->show();
    m_chatMainWidget->hide();
    m_localWidget->hide();
    m_homeWidget->hide();
    m_saleWidget->hide();
    m_billWidget->hide();
    m_userWidget->hide();
    m_memberWidget->GetMemberInfoWidget()->GetMemberInfo();
}

void MarketWidget::on_pb_sale_clicked()
{
    m_memberWidget->hide();
    m_chatMainWidget->hide();
    m_localWidget->hide();
    m_homeWidget->hide();
    m_saleWidget->show();
    m_billWidget->hide();
    m_userWidget->hide();
}

void MarketWidget::on_pb_bill_clicked()
{
    m_memberWidget->hide();
    m_chatMainWidget->hide();
    m_localWidget->hide();
    m_homeWidget->hide();
    m_saleWidget->hide();
    m_billWidget->show();
    m_userWidget->hide();
}

void MarketWidget::on_pb_update_clicked()
{
    m_memberWidget->hide();
    m_chatMainWidget->hide();
    m_localWidget->hide();
    m_homeWidget->hide();
    m_saleWidget->hide();
    m_billWidget->hide();
    m_userWidget->show();
}

void MarketWidget::on_pb_close_clicked()
{
    this->close();
}

void MarketWidget::on_pb_min_clicked()
{
    this->showMinimized();
}

void MarketWidget::on_pb_quit_clicked()
{
    this->close();
}

void MarketWidget::on_pb_first_clicked()
{
    m_homeWidget->show();
    m_localWidget->hide();
    m_chatMainWidget->hide();
    m_memberWidget->hide();
    m_saleWidget->hide();
    m_billWidget->hide();
    m_userWidget->hide();
}
