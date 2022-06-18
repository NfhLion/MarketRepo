#ifndef FILEMSGPROC_H
#define FILEMSGPROC_H

#include <QThread>

class FileMsgProc : public QThread
{
    Q_OBJECT
public:
    explicit FileMsgProc(QThread *parent = nullptr);

signals:

public slots:
};

#endif // FILEMSGPROC_H
