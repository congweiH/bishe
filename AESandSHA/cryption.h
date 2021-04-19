#ifndef CRYPTION_H
#define CRYPTION_H

#include <QObject>
#include "manager.h"
#include "mode.h"

class Cryption : public QObject
{
    Q_OBJECT
public:
    explicit Cryption(QObject *parent = nullptr);

    // 加密和解密
    void encryption();
    void decryption();

    Mode mode;

signals:
    void changeValue(double value);
};

#endif // CRYPTION_H
