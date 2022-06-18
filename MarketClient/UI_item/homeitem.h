#ifndef HOMEITEM_H
#define HOMEITEM_H

#include <QWidget>
#include <QList>
#include <QImage>

namespace Ui {
class HomeItem;
}

class HomeItem : public QWidget
{
    Q_OBJECT

signals:
    void SignalAllCommodityInfo(int number);

public:
    explicit HomeItem(QWidget *parent = 0);
    ~HomeItem();

    void SetId(const QString &id);
    void SetName(const QString &name);
    void SetOldPrice(const QString &price);
    void SetNewPrice(const QString &price);
    void SetCallNum(const QString &num);
    void SetHead(const QImage &image);
    void SetNumber(const int &number);

    void SetHeadPath(const QString &path);

    const QString GetId() const;
    const QString GetName() const;
    const QString GetOldPrice() const;
    const QString GetNewPrice() const;
    const QString GetCallNum() const;
    const QImage GetHead() const;

    const QString GetHeadPath() const;

private slots:
    void on_pb_all_clicked();

private:
    Ui::HomeItem *ui;

    QString m_id;
    QString m_name;
    QString m_oldPrice;
    QString m_newPrice;
    QString m_callNum;
    QString m_path;
    QImage  m_image;

    int m_number;
};

typedef QList<HomeItem *> HomeItemList;

#endif // HOMEITEM_H
