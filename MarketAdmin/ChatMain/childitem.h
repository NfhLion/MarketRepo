#ifndef CHILDITEM_H
#define CHILDITEM_H

#include <QWidget>
#include <QImage>

namespace Ui {
class ChildItem;
}

class ChildItem : public QWidget
{
    Q_OBJECT

signals:
    void SignalChatMsg(const QString &id);
    void SignalFriendInfo(const QString &id);

public:
    explicit ChildItem(QWidget *parent = 0);
    ~ChildItem();

    int     m_num;      //用于消息定时器来闪烁用
    void setID(const QString &id);
    void setName(const QString& name);
    void setLine(const QString& line);
    void setImage(const QImage& image);
    void setState(const int num);

    const QString GetID()const;
    const QString GetName()const;
    const QString GetLine()const;
    const QImage  GetImage()const;

private slots:
    void on_pb_msg_clicked();

    void on_pb_info_clicked();

private:
    Ui::ChildItem *ui;

    QString m_id;
    QString m_name;
    QString m_line;
    QImage  m_image;
};

typedef QMap<QString, ChildItem *> ChildItemMap;
typedef QList<ChildItem *>         ChildItemList;


#endif // CHILDITEM_H
