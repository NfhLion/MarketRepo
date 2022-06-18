#ifndef USERWIDGET_H
#define USERWIDGET_H

#include <QWidget>

#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>

namespace Ui {
class UserWidget;
}

class UserWidget : public QWidget
{
    Q_OBJECT

public:
    explicit UserWidget(QWidget *parent = 0);
    ~UserWidget();

private slots:
    void on_pb_update_clicked();

    void on_pb_ok_clicked();

private:
    Ui::UserWidget *ui;
};

#endif // USERWIDGET_H
