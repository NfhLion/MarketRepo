#ifndef SHOPITEM_H
#define SHOPITEM_H

#include <QWidget>
#include <QImage>

namespace Ui {
class ShopItem;
}

class ShopItem : public QWidget
{
    Q_OBJECT

signals:
    void SignalAllShopInfo(int number);

public:
    explicit ShopItem(QWidget *parent = 0);
    ~ShopItem();

    void SetShopID(const QString &id);
    void SetName(const QString &name);
    void SetPhone(const QString &phone);
    void SetSite(const QString &site);
    void SetTime(const QString &time);
    void SetHead(const QImage &image);
    void SetNumber(const int &number);

    const QString GetShopID();
    const QString GetName();
    const QString GetPhone();
    const QString GetSite();
    const QString GetTime();
    const QString GetNumber();
    const QImage  GetHead();

private slots:
    void on_pb_enter_clicked();

private:
    Ui::ShopItem *ui;

    QString m_shid;
    QString m_name;
    QString m_phone;
    QString m_site;
    QString m_time;
    QString m_number;
    QImage  m_image;
};

typedef QList<ShopItem *> ShopItemList;

#endif // SHOPITEM_H
