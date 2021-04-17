#include "sha.h"

SHA::SHA()
{

}
/*
 *整个业务逻辑整合，将被外界调用
 */
void SHA::sha1(QString passwd, word res[])
{
    // 1. 消息填充
    extend(passwd);

    word H[5] = { 0x67452301, 0xEFCDAB89, 0x98BADCFE, 0x10325476, 0xC3D2E1F0};

    for(int i = 0; i < p.size(); i += 64){
        // 2. 扩展成80
        createW(i);
        // 3. 计算
        Do(H);
    }
    // 返回结果
    for(int i = 0; i < 5; i++){
        res[i] = H[i];
    }

    cout << "sha-1 result:" << endl;
    cout<<setw(8)<< setfill('0')<<hex<<H[0].to_ulong();
    cout<<setw(8)<< setfill('0')<<hex<<H[1].to_ulong();
    cout<<setw(8)<< setfill('0')<<hex<<H[2].to_ulong();
    cout<<setw(8)<< setfill('0')<<hex<<H[3].to_ulong();
    cout<<setw(8)<< setfill('0')<<hex<<H[4].to_ulong()<<endl<<endl;
}
/*
 * 信息扩展: 变成512位. 这里passwd口令不超过55个字符【后面改进】
 */
void SHA::extend(QString passwd)
{
    p.clear();
    // 放原始数据
    for(int i = 0; i < passwd.size(); i++){
        p.push_back(passwd[i].unicode());
    }
    // 填充之前的长度
    int len = p.size();

    // 填充1000 0000 这个是必须要填充的
    p.push_back(128); // 128 对应 1000 0000

    while(p.size() * 8 % 512 != 448){
        p.push_back(0);   // 后面填充0
    }

    // 填充长度，占64位, 8个字节
    bitset<64> t = len * 8;
    // 将64位分成8个字节
    byte res[8];
    slip(t, res);
    for(int i = 0; i < 8;i++){
        p.push_back(res[i]);
    }

}
word SHA::S(word x, int n)
{
    word temp1 = x << n;
    word temp2 = x >> (32-n);
    return temp1 | temp2;
}

word SHA::ft(int t, word b, word c, word d)
{
    word temp;
    if(0<=t && t<=19) temp = (b&c) | ((~b)&d);
    else if(20<=t && t <= 39) temp = b ^ c ^ d;
    else if(40<=t && t <= 59) temp = (b&c) | (b&d) | (c&d);
    else if(60<=t && t <= 79) temp = b ^ c ^ d;
    return temp;
}

word SHA::kt(int t)
{
    word temp;
    if(0<=t && t<=19) temp = 0x5A827999;
    else if(20<=t && t <=39) temp = 0x6ED9EBA1;
    else if(40<=t && t <=59) temp = 0x8F1BBCDC;
    else if(60<=t && t <=79) temp = 0xCA62C1D6;
    return temp;
}
/*
 * 将这16份子明文分组扩充到80份子明文分组，我们记为W[k]（k= 0, 1,……79）
 * 扩充方法：
 *      Wt = Mt , 当0≤t≤15

        Wt = (W[t-3] ⊕ W[t-8] ⊕ W[t-14] ⊕ W[t-16]) <<< 1, 当16≤t≤79
 */
void SHA::createW(int start)
{
    // 分成16份子明文分组，每一份32个字节
    int len = 0;
    for(int j = start; j < start+64; j += 4){
        M[len++] = Manager::Word(p[j], p[j+1], p[j+2], p[j+3]);
    }

    for(int t = 0; t < 16; t++){
        W[t] = M[t];
    }
    for(int t = 16; t < 80; t++){
        W[t] = S(W[t-3] ^ W[t-8] ^ W[t-14] ^ W[t-16], 1);
    }
}
/*
 * 模2^32相加
 */
word SHA::add(word a, word b)
{
    word c;
    int d = 0; // 进位
    for(int i = 0; i < 32; i++){
        c[i] = (a[i] + b[i] + d) % 2;
        if(a[i] + b[i] + d >= 2){
            d = 1;
        }else if(a[i] + b[i] + d < 2){
            d = 0;
        }
    }
    return c;
}
/*
 * SHA-1有4轮计算，每一轮包括20个步骤，一共80步
 */
void SHA::Do(word H[5])
{
    word A = H[0];
    word B = H[1];
    word C = H[2];
    word D = H[3];
    word E = H[4];
    // 80 次
    for(int t = 0 ; t < 80; t++){
        word temp = add(add(add(add(S(A,5), ft(t,B,C,D)),E), W[t]),kt(t));
        E = D;
        D = C;
        C = S(B, 30);
        B = A;
        A = temp;
    }
    H[0] = add(H[0], A);
    H[1] = add(H[1], B);
    H[2] = add(H[2], C);
    H[3] = add(H[3], D);
    H[4] = add(H[4], E);

}

void SHA::slip(bitset<64> t, byte res[8])
{
    int p = 0;
    for(int i = 7; i >= 0; i--){
        for(int j = 0; j < 8; j++){
            res[i][j] = t[p++];
        }
    }
}

