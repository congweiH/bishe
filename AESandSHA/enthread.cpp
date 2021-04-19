#include "enthread.h"

EnThread::EnThread(QObject *parent) : QThread(parent)
{

}

void EnThread::run()
{
    int a = clock();
    cryption.encryption();
    int b = clock();

    qDebug() << "加密时间" << b-a;

    emit isDone();
}
