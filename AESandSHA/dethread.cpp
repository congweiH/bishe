#include "dethread.h"

DeThread::DeThread(QObject *parent) : QThread(parent)
{

}

void DeThread::run()
{
    cryption.decryption();

    emit isDone();
}
