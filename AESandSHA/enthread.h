#ifndef MYTHREAD_H
#define MYTHREAD_H

#include <QThread>
#include "cryption.h"

class EnThread : public QThread
{
    Q_OBJECT
public:
    explicit EnThread(QObject *parent = nullptr);


    Cryption cryption;


protected:
    void run() override;

signals:

    void isDone();

};

#endif // MYTHREAD_H
