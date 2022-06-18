#ifndef FGGROUPITEM_H
#define FGGROUPITEM_H

#include <QLabel>

class QPropertyAnimation;
class FgGroupItem : public QLabel
{
    Q_OBJECT
    Q_PROPERTY(int rotation READ rotation WRITE setRotation)
public:
    FgGroupItem(QWidget *parent = Q_NULLPTR);
    ~FgGroupItem();
public:
    void setText(const QString& title);
    void setExpanded(bool expand);

private:
    int rotation();
    void setRotation(int rotation);

private:
    void paintEvent(QPaintEvent *event);

private:
    QPropertyAnimation * m_animation;
    QString m_titleText;
    int m_rotation;

};

#endif // FGGROUPITEM_H
