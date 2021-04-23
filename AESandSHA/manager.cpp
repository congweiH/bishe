#include "manager.h"

Manager::Manager()
{

}
// 静态变量必须要这样告诉编译器申请了空间
QString Manager::filepath = "";
QString Manager::savepath = "";
byte Manager::key[16] = {};
byte* Manager::data = nullptr;
int Manager::dataSize = 0;
int Manager::addSize = 0;
char Manager::addChar = 'a';
int Manager::mode = -1;
QString Manager::addSuffix = "_en";
int Manager::readBytes = 1024*10;  // 一次读入1024B，也就是1M

/*
 * 从160位 选取 128位作为密钥存放在成员变量key中
 */
void Manager::setKey(word res[5])
{
    // 这里选前128位作为密钥
    byte a, b, c, d;
    int p = 0;
    for(int i = 0; i < 4; i++){ // 4 * 32 = 128
        slip(res[i], a, b, c, d);
        key[p++] = a;
        key[p++] = b;
        key[p++] = c;
        key[p++] = d;
    }
}

void Manager::slip(uint64_t t, byte res[8])
{
    for(int i = 0; i <8; i++){
        res[i] = (byte)(t >> i*8);
    }
}

void Manager::slip(word t, byte & a, byte & b, byte &c, byte & d)
{
    a = (byte)((t >> 24) & 0xff);
    b = (byte)((t >> 16) & 0xff);
    c = (byte)((t >> 8) & 0xff);
    d = (byte)(t & 0xff);
}
/*
 * 将四个字节变成一个字
 */
word Manager::Word(byte a, byte b, byte c, byte d)
{
    word ta = (word)a << 24;
    word tb = (word)b << 16;
    word tc = (word)c << 8;
    word td = (word)d;
    return ta | tb | tc | td;

}
