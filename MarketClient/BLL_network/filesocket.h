#ifndef FILESOCKET_H
#define FILESOCKET_H

#include <QObject>
#include <QTcpSocket>

#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonParseError>
#include <QDebug>

#include "control.h"

class FileSocket : public QObject
{
    Q_OBJECT
public:
    explicit FileSocket(QObject *parent = nullptr);
    ~FileSocket();

    ///单例模式运行
    static FileSocket *GetFileSocket();

    //检查socket链接
    void CheckConnected();
    //关闭链接
    void CloseConnected();

    // 连接服务器
    void ConnectToHost(const QString &host, const quint16 &port);

    // socket消息发送封装
    bool SendMessage(const QByteArray &data);

    //设置文件名
    void SetFileName(const QJsonObject &obj);

signals:
    //链接服务器成功信号
    void signalStatus(const char &state);

    //读完图片文件成功信号
    void SignalReadFileSuccess(const int &cmd);

private slots:
    // 与服务器断开链接
    void SlotDisconnected();
    // 链接上服务器
    void SlotConnected();
    // tcp消息处理
    void SlotReadyRead();

private:
    static FileSocket *m_fileSocket;

    QTcpSocket *m_tcpSocket;
    bool flag;

    int m_cmd;                          //区分哪种类型的文件

///----------------------------------------------------------------///
    bool    isJson;                     //判断是否为文件头部信息
    qint64     recvsize;                   //每次读多少
    QFile   file;                       //文件
    QString filename;                   //文件名
    qint64     filesize;                   //文件大小
};

#endif // FILESOCKET_H
