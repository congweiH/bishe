#ifndef SHA_H
#define SHA_H

#include <QString>
#include <QDebug>
#include <bitset>
#include <iostream>
#include <bits/stdc++.h>
#include <QVector>
#pragma once
using namespace std;

typedef bitset<8> byte; // 字节
typedef bitset<32> word;  // 字


/*
 * SHA-1原理讲解：https://www.cnblogs.com/sky-heaven/p/10138539.html
 * SHA-1：将用户输入的任意长度的口令转换成固定长度的160位，选取其中128位作为AES-128的密钥
 * 实现步骤：
 *      一、将口令转化成01串
 *          比如口令是 "abc", 因为 'a'=97, b='98', c='99'. 所以对应的01串为 01100001 01100010 01100011
 *
 *      二、对这个01串补位操作
 *          使得长度对512求余的结果为448，补位操作：填充比特位的最高位补一个1，其余的位补0
 *          如果补位之前就满足对512求余的结果为448，也要进行补位，在其后补一位1即可
 *
 *          比如上面的例子补位后：
 *          01100001 01100010 01100011 10(423位0)
 *
 *          然后将补位后的01串变成16进制串：
 *              61626380 00000000 00000000 00000000
                00000000 00000000 00000000 00000000
                00000000 00000000 00000000 00000000
                00000000 00000000
 *
 *      三、附加长度值
 *         在16进制串后面附加64位的信息，用来表示原始信息长度，然后16进制串就是512的倍数，如果口令长度不大于2^64，那么前面补0
 *         比如"abc"口令占3个字节，长度为24位，换算为16进制为0x18
 *         所以16进制串变成：
 *              61626380 00000000 00000000 00000000
                00000000 00000000 00000000 00000000
                00000000 00000000 00000000 00000000
                00000000 00000000 00000000 00000018

        四、初始化缓存
            一个160位MD缓冲区用以保存中间和最终散列函数的结果。它可以表示为5个32位的寄存器（H0，H1，H2，H3，H4），初始化为
            H0=0x67452301
            H1=0xEFCDAB89
            H2=0x98BADCFE
            H3=0x98BADCFE
            H4=0xC3D2E1F0

        五、计算16进制串

 */

class SHA
{
public:
    SHA();

    void sha1(QString passwd, word res[5]);

    // 信息扩展
    void extend(QString passwd);

    word S(word x, int n);  // 循环左移n位
    word ft(int t, word b, word c, word d);
    word kt(int t);
    void createW(int start);   // [p[start], p[end])
    word add(word a, word b);// 模2^32加函数
    void Do(word H[5]); // 4 轮计算

    void slip(word t, byte &a,byte &b,byte &c,byte &d);  // 将第一字分成4个字节
    void slip(bitset<64> t, byte res[8]);   // 将64位分成8个字节
    word Word(byte a,byte b, byte c,byte d); // 将4个字节拼成一个字





    // 存放扩充后的数据
    QVector<byte> p;

    // 512位分成16个字，我们使用M[k]（k= 0, 1,……15）来表示这16份子明文分组
    word M[16];
    //之后还要将这16份子明文分组扩充到80份子明文分组，我们记为W[k]（k= 0, 1,……79）
    word W[80];

};

#endif // SHA_H
