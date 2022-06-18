#include "marketlogin.h"
#include "ui_marketlogin.h"
#include "config.h"
#include "control.h"
#include "msgdeal.h"
#include "execsql.h"

#include <QMessageBox>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDebug>

MarketLogin::MarketLogin(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MarketLogin)
{
    ui->setupUi(this);

    m_isConnected = false;
    m_marketUI = NULL;
    m_msgProc = new MsgProc;
    //消息线程启动
    m_msgProc->start();


    this->setWindowFlags(Qt::FramelessWindowHint);
    ui->stackedWidget->setAttribute(Qt::WA_TranslucentBackground);

    MsgSocket::GetMsgSocket()->ConnectToHost(Config::m_nMsgHostAddr, Config::m_nMsgPort);

    connect(MsgSocket::GetMsgSocket(), &MsgSocket::signalStatus, [=](char state){
        switch (state) {
        case CMD_DisConnectedHost_B:
            m_isConnected = false;
            ui->lb_server->setText("与服务器断开");
            break;
        case CMD_ConnectedHost_A: // 登陆验证成功
            m_isConnected = true;
            ui->lb_server->setText("已连接服务器");
            break;
        default:
            break;
        }
    });

    connect(m_msgProc, &MsgProc::SignalReturnUserLogin, this, &MarketLogin::SlotReturnUserLogin);
}

MarketLogin::~MarketLogin()
{
    if(m_marketUI != NULL){
        delete m_marketUI;
        m_marketUI = NULL;
    }
    delete ui;
}

void MarketLogin::mousePressEvent(QMouseEvent *event)
{
     this->windowPos = this->pos();                // 获得部件当前位置
     this->mousePos = event->globalPos();     // 获得鼠标位置
     this->dPos = mousePos - windowPos;       // 移动后部件所在的位置
}

void MarketLogin::mouseMoveEvent(QMouseEvent *event)
{
//    this->move(event->globalPos() - );
//    event->accept();
    this->move(event->globalPos() - this->dPos);
}

void MarketLogin::on_pb_login_clicked()
{
    if(m_isConnected == false){
        QMessageBox::information(this, "链接服务器", "未连接服务器，请等待！");

        MsgSocket::GetMsgSocket()->ConnectToHost(Config::m_nMsgHostAddr, Config::m_nMsgPort);
        return ;
    }

    if(ui->le_id->text().isEmpty())
    {
        QMessageBox::warning(this, "警告", "用户名不能为空!",
                             QMessageBox::Retry|QMessageBox::Close);
    }else if(ui->le_pswd->text().isEmpty())
    {
        QMessageBox::warning(this, "警告", "密码不能为空!",
                             QMessageBox::Retry|QMessageBox::Close);
    }else
    {
        QJsonObject json;
        json.insert("msg_type", MsgText);
        json.insert("type", CMD_UserLogin_C);
        json.insert("user_id", ui->le_id->text());
        json.insert("user_pswd", ui->le_pswd->text());
        QJsonDocument doc(json);
        QByteArray data = doc.toJson();

        MsgSocket::GetMsgSocket()->SendMessage(data);
    }
}

void MarketLogin::SlotReturnUserLogin(const int &cmd)
{
    if(cmd == CMD_Sucess_Y){
        //保存此用户到配置文件
        Config::m_strUserName = ui->le_id->text();
        Config::m_strPassword = ui->le_pswd->text();
        Config::SetSettingFile("UserCfg", "User", Config::m_strUserName);
        Config::SetSettingFile("UserCfg", "Passwd", Config::m_strPassword);
        m_marketUI = new MarketWidget;
        m_marketUI->SetMsgProc(m_msgProc);


        m_marketUI->show();
        this->close();
    }else{
        QMessageBox::critical(this, "登录错误", "请检查登录信息!");
    }

    return ;
}

void MarketLogin::on_pb_modf_clicked()
{
    ui->le_msgip->setEnabled(true);
    ui->le_msgport->setEnabled(true);
    ui->le_fileip->setEnabled(true);
    ui->le_fileport->setEnabled(true);
}

void MarketLogin::on_pb_set_clicked()
{
    MsgSocket::GetMsgSocket()->CloseConnected();
    m_isConnected = false;
    ui->stackedWidget->setCurrentIndex(1);
    ui->lb_server->setText("正在链接服务器...");
}

void MarketLogin::on_pb_bank_clicked()
{
    MsgSocket::GetMsgSocket()->ConnectToHost(Config::m_nMsgHostAddr, Config::m_nMsgPort);
    ui->stackedWidget->setCurrentIndex(0);
}

void MarketLogin::on_pb_ok_clicked()
{
    Config::m_nMsgHostAddr  = ui->le_msgip->text();
    Config::m_nMsgPort      = ui->le_msgport->text().toInt();
    Config::m_nFileHostAddr = ui->le_fileip->text();
    Config::m_nFilePort     = ui->le_fileport->text().toInt();

    Config::SaveConfig();
    QMessageBox::information(this, "服务器配置", "服务器配置成功!");
    ui->le_msgip->setEnabled(false);
    ui->le_msgport->setEnabled(false);
    ui->le_fileip->setEnabled(false);
    ui->le_fileport->setEnabled(false);
}

///关闭
void MarketLogin::on_pb_close_clicked()
{
    MsgSocket::GetMsgSocket()->CloseConnected();
    this->close();
}

///缩小
void MarketLogin::on_pb_min_clicked()
{
    this->showMinimized();
}

void MarketLogin::paintEvent(QPaintEvent *)
{
    QPainter p(this);
    QPixmap pix(":/image/watch.png");
    p.drawPixmap(0,0,pix);
}
