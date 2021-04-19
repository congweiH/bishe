#ifndef MODEL_H
#define MODEL_H


#include "manager.h"
#include "aes.h"
#include <time.h>
/*
 * 加密五种模式
 *
 *  0 ECB
 *  1 CBC
 *  2 CFB
 *  3 OFB
 *  4 CTR
 */
class Mode : public QObject
{

    Q_OBJECT

public:
    explicit Mode(QObject *parent = nullptr);

    AES aes;

    // ECB模式 加密
    void ECB_en();
    // ECB模式 解密
    void ECB_de();

    // CBC模式
    void CBC_en();
    void CBC_de();

    // CFB
    void CFB_en();
    void CFB_de();

    // OFB
    void OFB_en();
    void OFB_de();

    // CTR
    void CTR_en();
    void CTR_de();
    void plusOne(byte IV[16]);



};

#endif // MODEL_H
