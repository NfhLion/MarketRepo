#ifndef HOMENOTICE_H
#define HOMENOTICE_H

#include <QWidget>

namespace Ui {
class HomeNotice;
}

class HomeNotice : public QWidget
{
    Q_OBJECT

public:
    explicit HomeNotice(QWidget *parent = 0);
    ~HomeNotice();

private:
    Ui::HomeNotice *ui;
};

#endif // HOMENOTICE_H
