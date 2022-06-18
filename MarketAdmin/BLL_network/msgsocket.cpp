#include "msgsocket.h"
#include "config.h"
#include "control.h"
#include "globalvariable.h"

#include <QHostAddress>

MsgSocket *MsgSocket::m_msgSocket = NULL;
MsgSocket::MsgSocket(QObject *parent) : QObject(parent)
{
    m_tcpSocket = new QTcpSocket(this);
    flag = false;

    connect(m_tcpSocket, &QTcpSocket::readyRead, this, &MsgSocket::SlotReadyRead);
    connect(m_tcpSocket, &QTcpSocket::connected, this, &MsgSocket::SlotConnected);
    connect(m_tcpSocket, &QTcpSocket::disconnected, this, &MsgSocket::SlotDisconnected);
}

MsgSocket::~MsgSocket()
{
    if(m_msgSocket != NULL){
        delete m_msgSocket;
        m_msgSocket = NULL;
    }
}

MsgSocket *MsgSocket::GetMsgSocket()
{
    static QMutex mutex;
    if(m_msgSocket == NULL){
        QMutexLocker locker(&mutex);

        if (!m_msgSocket)
        {
            m_msgSocket = new MsgSocket();
        }
    }

    return m_msgSocket;
}

///检查链接
void MsgSocket::CheckConnected()
{
    if (m_tcpSocket->state() != QTcpSocket::ConnectedState)
    {
        m_tcpSocket->connectToHost(Config::m_nMsgHostAddr, Config::m_nMsgPort);
    }
}

///主动断开链接
void MsgSocket::CloseConnected()
{
    if (m_tcpSocket->isOpen())
    {
        m_tcpSocket->abort();
    }
}

///链接服务器接口
void MsgSocket::ConnectToHost(const QString &host, const quint16 &port)
{
    if (m_tcpSocket->isOpen())
    {
        m_tcpSocket->abort();
    }
    m_tcpSocket->connectToHost((QHostAddress)host, port);
}

///发送消息接口
bool MsgSocket::SendMessage(const QByteArray &data)
{

    // 连接服务器
    if (!m_tcpSocket->isOpen())
    {
        m_tcpSocket->connectToHost(Config::m_nMsgHostAddr, Config::m_nMsgPort);
        m_tcpSocket->waitForConnected(1000);
    }
    // 超时1s后还是连接不上，直接返回
    if (!m_tcpSocket->isOpen())
    {
        return false;
    }
    if(m_tcpSocket == NULL || !flag)
    {
        return false;
    }
    if(m_tcpSocket->write(data.toStdString().data()) == -1)
    {
        return false;
    }

    qDebug() << "client: " <<  data.toStdString().data();

    return true;
}


///断开服务器成功返回
void MsgSocket::SlotDisconnected()
{
    qDebug() << "has disconnecetd";
    m_tcpSocket->abort();
    flag = false;
    emit signalStatus(CMD_DisConnectedHost_B);
}

///链接服务器成功返回
void MsgSocket::SlotConnected()
{
    qDebug() << "has connecetd";
    flag = true;
    emit signalStatus(CMD_ConnectedHost_A);
}


///读取消息接口
void MsgSocket::SlotReadyRead()
{
    QByteArray data;
    while(m_tcpSocket->bytesAvailable() > 0)
    {
        data.append(m_tcpSocket->readAll());

        //阻塞10毫秒，保障滑动窗口满了以后，Server第二次发过来的数据可以读到
        m_tcpSocket->waitForReadyRead(10);
    }
    GlobalVariable::g_msgQueue.append(data);
    qDebug() << m_tcpSocket->bytesAvailable() << " " << data.length();
    qDebug() << QString(data).toStdString().data();
}

