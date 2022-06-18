#ifndef SEARCHFRIEND_H
#define SEARCHFRIEND_H

#include <QWidget>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

namespace Ui {
class SearchFriend;
}

class SearchFriend : public QWidget
{
    Q_OBJECT

public:
    explicit SearchFriend(QWidget *parent = 0);
    ~SearchFriend();

    void SetId(const QString &id);
    void SetName(const QString &name);
    void SetState(const QString &state);

    const QString GetId();
    const QString GetName();

private slots:
    void on_pb_search_clicked();

    void on_pb_add_clicked();

    void on_pb_close_clicked();

private:
    Ui::SearchFriend *ui;

    QString m_id;
    QString m_name;
    QString m_state;
};

#endif // SEARCHFRIEND_H
