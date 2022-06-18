#include "marketwidget.h"
#include "ui_marketwidget.h"
#include "filesocket.h"

MarketWidget::MarketWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MarketWidget)
{
    ui->setupUi(this);

    SetPushButter();
    this->setWindowFlags(Qt::FramelessWindowHint);
    m_isCloseSystem = false;
    m_isFirstGet = true;
    m_Timer = new QTimer(this);

    m_homeWidget = new HomeWidget(ui->widget);
    m_shopWidget = new ShopWidget(ui->widget);
    m_repertoryWidget= new RepertoryWidget(ui->widget);
    m_chatMainWidget = new ChatMain(ui->widget);
    m_billWidget = new BillWidget(ui->widget);
    m_userWidget = new UserWidget(ui->widget);

    m_homeWidget->show();
    m_shopWidget->hide();
    m_repertoryWidget->hide();
    m_chatMainWidget->hide();
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

void MarketWidget::paintEvent(QPaintEvent *)
{
    QPainter p(this);
    QPixmap pix(":/image/climain.jpg");
    p.drawPixmap(0,0,pix);
}

MarketWidget::~MarketWidget()
{
    delete ui;
}

//设置按钮
void MarketWidget::SetPushButter()
{
    ui->pb_home->setIcon(QPixmap(":/ico/first.png"));
    ui->pb_shop->setIcon(QPixmap(":/ico/shop.png"));
    ui->pb_chat->setIcon(QPixmap(":/ico/diaglo.png"));
    ui->pb_bill->setIcon(QPixmap(":/ico/sort.png"));
    ui->pb_repertory->setIcon(QPixmap(":/ico/xiaoshou.png"));
    ui->pb_user->setIcon(QPixmap(":/ico/mima.png"));
    ui->pb_first->setIcon(QPixmap(":/ico/firstr.png"));
    ui->pb_quit->setIcon(QPixmap(":/ico/go.jpg"));
}

//获取时间
void MarketWidget::GetDateTime()
{
    QTime timeNow = QTime::currentTime();
    ui->lb_time->setText((QDate::currentDate().toString(tr("yyyy年MM月dd日 dddd")))
                           + tr("  %1").arg(timeNow.toString()));
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
    case CMD_AllShopInfo_J :        m_shopWidget->GetShopInfo()->SetShopArray(cmd_result, value); break;
    case CMD_CommodityForShop_K :   m_shopWidget->GetShopInfo()->SetShopCommodityAll(cmd_result, value); break;
    case CMD_GetRepertoryLocal_L :  m_repertoryWidget->GetRepertoryLocal()->SetRepertoryLocal(cmd_result, value); break;
    case CMD_GetRepertoryShip_M :   m_repertoryWidget->GetRepertoryShip()->SetRepertoryShipment(cmd_result, value); break;
    case CMD_GetCommodityForID_N :  m_repertoryWidget->GetRepertoryShip()->SetCommodityTableWidget(cmd_result, value); break;
    case CMD_UpdateShipment_R :     DeadUpdateShipment(cmd_result, value); break;
    case CMD_InsertShop :           m_shopWidget->GetInsertShop()->SetInsertShop(cmd_result, value); break;
    case CMD_UpdatePassword :       if(cmd_result == CMD_Sucess_Y){
                                        QMessageBox::information(this,"修改密码","修改密码成功,请重新登录!");
                                        this->close();
                                    } break;

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

///处理更新调货信息
void MarketWidget::DeadUpdateShipment(const int &cmd_result, const QJsonValue &value)
{
    if(cmd_result == CMD_Sucess_Y){
        QString state = value.toString();
        if(state == "审核中")
            QMessageBox::information(this, "申请调货", "有店铺申请调货，请总部处理！");
        else if(state == "调货中")
            QMessageBox::information(this, "调货处理", "调货已成功，请等待对方收到货！");
        else if(state == "已取消")
            QMessageBox::information(this, "调货处理", "对方以取消调货！");
        else if(state == "已收到")
            QMessageBox::information(this, "调货处理", "对方已收到货物！");
        m_repertoryWidget->GetRepertoryShip()->GetRepertoryShipment();
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

///获取所有商品后
void MarketWidget::DealAllCommodity(const int &cmd_result, const QJsonValue &value)
{
    if(m_isFirstGet == true){
        //获取好友状态
        m_chatMainWidget->GetFriendState();
        m_isFirstGet = false;
    }
    m_homeWidget->GetHomeCommodity()->SetAllCommodityInfo(cmd_result, value);
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

void MarketWidget::SlotUserLogout()
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


void MarketWidget::on_pb_home_clicked()
{
    m_homeWidget->show();
    m_shopWidget->hide();
    m_repertoryWidget->hide();
    m_chatMainWidget->hide();
    m_billWidget->hide();
    m_userWidget->hide();
}


void MarketWidget::on_pb_shop_clicked()
{
    m_homeWidget->hide();
    m_shopWidget->show();
    m_repertoryWidget->hide();
    m_chatMainWidget->hide();
    m_billWidget->hide();
    m_userWidget->hide();
    m_shopWidget->GetShopInfo()->SetShopInfo();
}

void MarketWidget::on_pb_repertory_clicked()
{
    m_homeWidget->hide();
    m_shopWidget->hide();
    m_repertoryWidget->show();
    m_chatMainWidget->hide();
    m_billWidget->hide();
    m_userWidget->hide();
    m_repertoryWidget->GetRepertoryShip()->GetRepertoryShipment();
}

void MarketWidget::on_pb_chat_clicked()
{
    m_homeWidget->hide();
    m_shopWidget->hide();
    m_repertoryWidget->hide();
    m_chatMainWidget->show();
    m_billWidget->hide();
    m_userWidget->hide();
}

void MarketWidget::on_pb_bill_clicked()
{
    m_homeWidget->hide();
    m_shopWidget->hide();
    m_repertoryWidget->hide();
    m_chatMainWidget->hide();
    m_billWidget->show();
    m_userWidget->hide();
}

void MarketWidget::on_pb_user_clicked()
{
    m_homeWidget->hide();
    m_shopWidget->hide();
    m_repertoryWidget->hide();
    m_chatMainWidget->hide();
    m_billWidget->hide();
    m_userWidget->show();
}

void MarketWidget::on_pb_min_clicked()
{
    this->showMinimized();
}

void MarketWidget::on_pb_close_clicked()
{
    this->close();
}

void MarketWidget::on_pb_first_clicked()
{
    m_homeWidget->show();
    m_shopWidget->hide();
    m_repertoryWidget->hide();
    m_chatMainWidget->hide();
    m_billWidget->hide();
    m_userWidget->hide();
}

void MarketWidget::on_pb_quit_clicked()
{
    this->close();
}
