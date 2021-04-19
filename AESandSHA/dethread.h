#ifndef DETHREAD_H
#define DETHREAD_H

#include <QThread>
#include "cryption.h"

class DeThread : public QThread
{
    Q_OBJECT
public:
    explicit DeThread(QObject *parent = nullptr);

    Cryption cryption;

protected:
    void run() override;

signals:
    void isDone();
};

#endif // DETHREAD_H
