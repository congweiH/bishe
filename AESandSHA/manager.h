#ifndef MANAGER_H
#define MANAGER_H


#include <bitset>
#include <QString>
#include <QVector>
#include <QFile>
#include <QDebug>
#include <QFileInfo>
#include <QDataStream>
using namespace std;

typedef uint8_t byte;
typedef uint32_t word;


class Manager
{
public:
    Manager();


    // 设置密钥
    static void setKey(word res[5]);

    static void slip(uint64_t t, byte res[8]);


    // 将一个字分成4个字节
    static void slip(word t, byte & a, byte & b, byte &c, byte & d);
    static word Word(byte a, byte b, byte c, byte d);

    // 用户输入的口令对应的AES的密钥
    static byte key[16];

    // 加密或解密的文件路径
    static QString filepath;
    // 加密文件后缀添加的字符串
    static QString addSuffix;

    // 数据
    static byte* data;
    static int dataSize;

    // 填充的大小
    static int addSize;
    // 填充的字符
    static char addChar;
    // 使用何种模式
    static int mode;
    // 一次从文件中读入或写入多少个字节
    static int readBytes;

};

#endif // MANAGER_H
