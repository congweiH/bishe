#include "aes.h"

AES::AES()
{

}
/*
 * 密钥扩展，将原始的16字节密钥key变成44字节w
 */
void AES::keyExpansion()
{
    // 将初始密码每一列都转换成一个字 得出 w[0], w[1], w[2], w[3]
    for(int i = 0; i < 4; i++){
        w[i] = Manager::Word(Manager::key[4*i], Manager::key[4*i+1], Manager::key[4*i+2], Manager::key[4*i+3]);
    }
    // 扩充40个新列，总共构成44列, j代表第几轮
    for(int i = 4; i < 44; i++){
        word temp = w[i-1];
        if(i % 4 == 0){
            temp = SubWord(RotWord(temp)) ^ Rcon[i/4];
        }
        w[i] = w[i-4] ^ temp;
    }
}
/*
 * 对word中每一个字进行S盒变换
 */
word AES::SubWord(word t)
{
    byte a[4];
    Manager::slip(t, a[0], a[1], a[2], a[3]);

    for(int i = 0; i < 4; i++){
        a[i] = SubByte(a[i]);
    }
    // 将4个字节组成1个字
    return Manager::Word(a[0], a[1], a[2], a[3]);
}
byte AES::SubByte(byte t)
{
    int high, low;
    // 将第一个字节分成两部分
    Manager::slip(t, high, low);
    // 高四位作为行值，低四位作为列值
    return SBox[high][low];
}

byte AES::SubByte1(byte t)
{
    int high, low;
    // 将第一个字节分成两部分
    Manager::slip(t, high, low);
    // 高四位作为行值，低四位作为列值
    return SBoxInv[high][low];
}
/*
 * 左循环一个字节[t0, t1, t2, t3] -> [t1, t2, t3 ,t0]
 */
word AES::RotWord(word t)
{
    byte a[4];
    Manager::slip(t, a[0], a[1], a[2], a[3]);
    return Manager::Word(a[1], a[2], a[3], a[0]);
}

word AES::RotWord1(word t)
{
    byte a[4];
    Manager::slip(t, a[0], a[1], a[2], a[3]);
    return Manager::Word(a[3], a[0], a[1], a[2]);
}
/*
 * 加密一组的过程
 */
void AES::block_en(byte state[16])
{
    //  轮密钥加 w[0, 3]
    addRoundKey(state, 0);

    // 前9轮操作 w[4, 39]
    for(int i = 4; i < 40 ; i+=4){
        // 1. 字节替换
        SubBytes(state);
        // 2. 行移位
        ShiftRows(state);
        // 3. 列混淆
        MixColumns(state);
        // 4. 轮密钥加
        addRoundKey(state, i);
    }

    // 第10轮操作, 没有列混淆 w[40, 43]
    SubBytes(state);
    ShiftRows(state);
    addRoundKey(state, 40);
}
/*
 * 一组的解密
 */
void AES::block_de(byte *state)
{
    addRoundKey(state, 40);
    // 前 9 次
    for(int i = 36; i>=4; i-=4){
        // inv shift rows
        ShiftRows1(state);
        // inv subbytes
        SubBytes1(state);
        // add round key
        addRoundKey(state, i);
        // inv mix colums
        MixColumns1(state);
    }
    // 第10次
    ShiftRows1(state);
    SubBytes1(state);
    addRoundKey(state, 0);
}
/*
 * 轮密钥加：扩展后的密钥w[start,start+4]和状态矩阵state异或
 */
void AES::addRoundKey(byte *state, int start)
{
    byte a[4];
    // 一共4列
    for(int j = 0; j < 4; j++){
        // state第i列与 w 第i个字异或
        Manager::slip(w[start+j], a[0], a[1], a[2], a[3]);
        for(int i = 0; i < 4; i++){
            state[i*4+j] ^= a[i];
        }
    }
}
/*
 * 字节替换
 */
void AES::SubBytes(byte *state)
{
    for(int i = 0; i < 4; i++){
        for(int j = 0; j < 4; j++){
            state[i*4+j] = SubByte(state[i*4+j]);
        }
    }
}
// 逆字节替换
void AES::SubBytes1(byte *state)
{
    for(int i = 0; i < 4; i++){
        for(int j = 0; j < 4; j++){
            state[4*i+j] = SubByte1(state[4*i+j]);
        }
    }
}
/*
 * 行移位，i行左循环移位i个字节
 */
void AES::ShiftRows(byte *state)
{
    for(int i = 0; i < 4; i++){
        word t = Manager::Word(state[4*i+0], state[4*i+1], state[4*i+2], state[4*i+3]);
        for(int j = 0; j < i; j++){ // 左移位i次
            RotWord(t);
        }
        Manager::slip(t, state[4*i+0], state[4*i+1], state[4*i+2], state[4*i+3]);
    }
}
/*
 * 逆行移位：i行右移i个字
 */
void AES::ShiftRows1(byte *state)
{
    for(int i = 0; i < 4; i++){
        word t = Manager::Word(state[4*i+0], state[4*i+1], state[4*i+2], state[4*i+3]);
        for(int j = 0; j < i; j++){ // 右移位i次
            RotWord1(t);
        }
        Manager::slip(t, state[4*i+0], state[4*i+1], state[4*i+2], state[4*i+3]);
    }
}
/*
 * 列混淆
 */
void AES::MixColumns(byte *state)
{
    byte temp[16];
    for(int i = 0; i < 16; i++){
        temp[i] = state[i];
    }
    // j列
    for(int j = 0; j < 4; j++){
        state[4*0+j] = mul(0x02, temp[4*0+j]) ^ mul(0x03, temp[4*1+j]) ^ mul(0x01, temp[4*2+j]) ^ mul(0x01, temp[4*3+j]);
        state[4*1+j] = mul(0x01, temp[4*0+j]) ^ mul(0x02, temp[4*1+j]) ^ mul(0x03, temp[4*2+j]) ^ mul(0x01, temp[4*3+j]);
        state[4*2+j] = mul(0x01, temp[4*0+j]) ^ mul(0x01, temp[4*1+j]) ^ mul(0x02, temp[4*2+j]) ^ mul(0x03, temp[4*3+j]);
        state[4*3+j] = mul(0x03, temp[4*0+j]) ^ mul(0x01, temp[4*1+j]) ^ mul(0x01, temp[4*2+j]) ^ mul(0x02, temp[4*3+j]);
    }
}

void AES::MixColumns1(byte *state)
{
    byte temp[16];
    for(int i = 0; i < 16; i++){
        temp[i] = state[i];
    }

    for(int j = 0; j < 4; j++){
        state[4*0+j] = mul(0x0E, temp[4*0+j]) ^ mul(0x0B, temp[4*1+j]) ^ mul(0x0D, temp[4*2+j]) ^ mul(0x09, temp[4*3+j]);
        state[4*1+j] = mul(0x09, temp[4*0+j]) ^ mul(0x0E, temp[4*1+j]) ^ mul(0x0B, temp[4*2+j]) ^ mul(0x0D, temp[4*3+j]);
        state[4*2+j] = mul(0x0D, temp[4*0+j]) ^ mul(0x09, temp[4*1+j]) ^ mul(0x0E, temp[4*2+j]) ^ mul(0x0B, temp[4*3+j]);
        state[4*3+j] = mul(0x0B, temp[4*0+j]) ^ mul(0x0D, temp[4*1+j]) ^ mul(0x09, temp[4*2+j]) ^ mul(0x0E, temp[4*3+j]);
    }
}
// TODO GF(2^8)上的乘法 (难点)
byte AES::mul(byte a, byte b)
{
    byte p = 0, h;

    for(int i = 0;i < 8; i++){
        if((b & byte(1)) != 0){
            p ^= a;
        }
        h = (byte) (a & byte(0x80));
        a <<= 1;
        if (h != 0) {
            a ^= 0x1b; /* x^8 + x^4 + x^3 + x + 1 */
        }
        b >>= 1;
    }
    return p;
}
