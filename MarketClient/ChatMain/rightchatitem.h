#ifndef RIGHTCHATITEM_H
#define RIGHTCHATITEM_H

#include <QWidget>
#include <QImage>
#include <QPixmap>

namespace Ui {
class RightChatItem;
}

class RightChatItem : public QWidget
{
    Q_OBJECT

public:
    explicit RightChatItem(QWidget *parent = 0);
    ~RightChatItem();

    void setID(const QString &id);
    void setTime(const QString& time);
    void setImage(const QImage& image);
    void setMsg(const QString &msg);

private:
    Ui::RightChatItem *ui;
    QString m_id;
    QString m_time;
    QImage  m_image;
    QString m_msg;
};
typedef QList<RightChatItem *> RightChatItemList;

#endif // RIGHTCHATITEM_H
