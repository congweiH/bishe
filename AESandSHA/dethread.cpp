#include "dethread.h"

DeThread::DeThread(QObject *parent) : QThread(parent)
{

}

void DeThread::run()
{
    int a = clock();
    cryption.decryption();
    int b = clock();

    qDebug() << "解密时间" << b-a;

    emit isDone();
}
