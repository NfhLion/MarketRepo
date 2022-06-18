#ifndef FGCHILDITEM_H
#define FGCHILDITEM_H

#include <QWidget>

namespace Ui {
class FgChildItem;
}

class FgChildItem : public QWidget
{
    Q_OBJECT

public:
    explicit FgChildItem(QWidget *parent = 0);
    ~FgChildItem();

    void setName(const QString& name);
    void setImage(const QPixmap& pixmap);

    QString getName();

private:
    Ui::FgChildItem *ui;
};

#endif // FGCHILDITEM_H
