#include "filesocket.h"

#include <QMutex>
#include <QHostAddress>
#include <QString>

#include "config.h"
#include "globalvariable.h"

FileSocket::FileSocket(QObject *parent) : QObject(parent)
{
    m_tcpSocket = new QTcpSocket(this);
    flag = false;
    isJson = true;
    recvsize = 0;
    filesize = 0;

    connect(m_tcpSocket, &QTcpSocket::readyRead, this, &FileSocket::SlotReadyRead);
    connect(m_tcpSocket, &QTcpSocket::connected, this, &FileSocket::SlotConnected);
    connect(m_tcpSocket, &QTcpSocket::disconnected, this, &FileSocket::SlotDisconnected);
}

FileSocket::~FileSocket()
{
    if(m_fileSocket != NULL){
        delete m_fileSocket;
        m_fileSocket = NULL;
    }
}

FileSocket *FileSocket::m_fileSocket = NULL;

FileSocket *FileSocket::GetFileSocket()
{
    static QMutex mutex;
    if(m_fileSocket == NULL){
        QMutexLocker locker(&mutex);

        if (!m_fileSocket)
        {
            m_fileSocket = new FileSocket();
        }
    }

    return m_fileSocket;
}

///检查链接
void FileSocket::CheckConnected()
{
    if (m_tcpSocket->state() != QTcpSocket::ConnectedState)
    {
        m_tcpSocket->connectToHost(Config::m_nFileHostAddr, Config::m_nFilePort);
    }
}

///主动断开链接
void FileSocket::CloseConnected()
{
    if (m_tcpSocket->isOpen())
    {
        m_tcpSocket->abort();
    }
}

///链接服务器接口
void FileSocket::ConnectToHost(const QString &host, const quint16 &port)
{
    if (m_tcpSocket->isOpen())
    {
        m_tcpSocket->abort();
    }
    m_tcpSocket->connectToHost((QHostAddress)host, port);
}

///发送消息接口
bool FileSocket::SendMessage(const QByteArray &data)
{
    // 连接服务器
    if (!m_tcpSocket->isOpen())
    {
        m_tcpSocket->connectToHost(Config::m_nFileHostAddr, Config::m_nFilePort);
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
    qDebug() << "SendMessage" << data.toStdString().data();
    return true;
}

///断开服务器成功返回
void FileSocket::SlotDisconnected()
{
    qDebug() << "has disconnecetd fileserver";
    m_tcpSocket->abort();
    flag = false;
    emit signalStatus(CMD_DisConnectedHost_B);
}

///链接服务器成功返回
void FileSocket::SlotConnected()
{
    qDebug() << "has connecetd fileserver";
    flag = true;
    emit signalStatus(CMD_ConnectedHost_A);
}

///读取消息接口
void FileSocket::SlotReadyRead()
{
    QByteArray data = m_tcpSocket->readAll();
    if (isJson == true){      
        qDebug() << QString(data).toStdString().data();

        QJsonObject obj = QJsonDocument::fromJson(data).object();
        m_cmd = obj.value("type").toInt();

        switch(m_cmd)
        {
        case CMD_UploadFile_G : filename = Config::m_strGoodsPath + obj.value("new_filename").toString();
                                GlobalVariable::g_compathMap[obj.value("com_id").toString()] = filename;
                                break;
        }
        filesize = obj.value("filesize").toInt();

        qDebug() << filename << " " << filesize;
        //打开文件
        file.setFileName(filename);
        if(file.open(QIODevice::WriteOnly))
        {
            isJson = false;
            QJsonObject json;
            json.insert("type", CMD_GetFileHead_I);
            json.insert("path", obj.value("old_filename").toString());
            QJsonDocument doc(json);
            QByteArray data = doc.toJson();
            this->SendMessage(data);
        }else
        {
            qDebug() << "打开文件出错";
        }

    } else{
        qint64 len = file.write(data);
        qDebug() << len << " " << recvsize;
        if(len > 0)
        {
            recvsize += len;
        }
        if(recvsize == filesize)
        {
            qDebug() << recvsize;
            file.close();
            recvsize = 0;
            filename.clear();
            filesize = 0;
            isJson = true;

            emit SignalReadFileSuccess(m_cmd);
        }
    }
}


