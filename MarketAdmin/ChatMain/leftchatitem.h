#ifndef LEFTCHATITEM_H
#define LEFTCHATITEM_H

#include <QWidget>
#include <QImage>
#include <QPixmap>

namespace Ui {
class LeftChatItem;
}

class LeftChatItem : public QWidget
{
    Q_OBJECT

public:
    explicit LeftChatItem(QWidget *parent = 0);
    ~LeftChatItem();

    void setID(const QString &id);
    void setTime(const QString& time);
    void setImage(const QImage& image);
    void setMsg(const QString &msg);

private:
    Ui::LeftChatItem *ui;
    QString m_id;
    QString m_time;
    QImage  m_image;
    QString m_msg;
};
typedef QList<LeftChatItem *> LeftChatItemList;

#endif // LEFTCHATITEM_H
