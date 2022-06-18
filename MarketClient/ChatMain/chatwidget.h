#ifndef CHATWIDGET_H
#define CHATWIDGET_H

#include <QWidget>
#include <QMap>
#include <QList>

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

#include "leftchatitem.h"
#include "rightchatitem.h"

namespace Ui {
class ChatWidget;
}

class ChatWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ChatWidget(QWidget *parent = 0);
    ~ChatWidget();

    void setID(const QString &id);
    void setName(const QString& name);
    void setImage(const QImage& image);
    void setShow(bool isShow);

    const QString GetID()const;
    const QString GetName()const;
    const QImage  GetImage()const;
    const bool    GetShow()const;

    void SetListWidgetLeft(const QString &id, const QString time, const QString &msg, const QImage &image);
    void SetListWidgetRight(const QString &id, const QString time, const QString &msg, const QImage &image);

private slots:
    void on_pb_send_clicked();

private:
    Ui::ChatWidget *ui;

    QString m_id;
    QString m_name;
    QImage  m_image;
    int     m_line;

    bool    m_isShow;
};

typedef QMap<QString, ChatWidget *> ChatWidgetMap;
typedef QList<ChatWidget *>         ChatWidgetList;

#endif // CHATWIDGET_H
