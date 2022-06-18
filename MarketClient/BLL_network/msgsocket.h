#ifndef MSGSOCKET_H
#define MSGSOCKET_H

#include <QObject>
#include <QTcpSocket>
#include <QMutex>

class MsgSocket : public QObject
{
    Q_OBJECT
public:
    explicit MsgSocket(QObject *parent = nullptr);
    ~MsgSocket();

    ///单例模式运行
    static MsgSocket *GetMsgSocket();



    //检查socket链接
    void CheckConnected();
    //关闭链接
    void CloseConnected();

    // 连接服务器
    void ConnectToHost(const QString &host, const quint16 &port);

    // socket消息发送封装
    bool SendMessage(const QByteArray &data);

signals:
    //链接服务器成功信号
    void signalStatus(const char &state);

private slots:
    // 与服务器断开链接
    void SlotDisconnected();
    // 链接上服务器
    void SlotConnected();
    // tcp消息处理
    void SlotReadyRead();

private:
    static MsgSocket *m_msgSocket;

    QTcpSocket *m_tcpSocket;
    bool flag;
};



#endif // MSGSOCKET_H
