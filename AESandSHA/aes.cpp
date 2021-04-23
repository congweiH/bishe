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
            temp = SubWord(RotWord(temp, 1)) ^ Rcon[i/4];
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
    int high = t >> 4;
    int low = t & 0x0f;
    // 高四位作为行值，低四位作为列值
    return SBox[high][low];
}

byte AES::SubByte1(byte t)
{
    int high = t >> 4;
    int low = t & 0x0f;
    // 高四位作为行值，低四位作为列值
    return SBoxInv[high][low];
}
/*
 * 左循环一个字节[t0, t1, t2, t3] -> [t1, t2, t3 ,t0]
 */
word AES::RotWord(word t, int n)
{
    word high = t << (n*8);
    word low = t >> (32-n*8);
    return high | low;
}
// 右循环一个字节[t0, t1, t2, t3] -> [t3, t0, t1 ,t2]
word AES::RotWord1(word t, int n)
{
    word high = t << (32-n*8);
    word low = t >> (n*8);
    return high | low;
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
//        mix_columns(state);
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
//        inv_mix_columns(state);
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
//    byte a[4];
    // 一共4列
    for(int j = 0; j < 4; j++){
        // state第j列与 w 第j个字异或
        //Manager::slip(w[start+j], a[0], a[1], a[2], a[3]);
//        a[0] = (byte)((w[start+j] >> 24) & 0xff);
//        a[1] = (byte)((w[start+j] >> 16) & 0xff);
//        a[2] = (byte)((w[start+j] >> 8) & 0xff);
//        a[3] = (byte)(w[start+j] & 0xff);
        for(int i = 0; i < 4; i++){
            state[i*4+j] ^= (byte)(w[start+j] >> (24-i*8));
        }
    }
}
/*
 * 字节替换
 */
void AES::SubBytes(byte *state)
{
    uint8_t high;
    uint8_t low;
    for(int i = 0; i < 4; i++){
        for(int j = 0; j < 4; j++){
            high = state[i*4+j] >> 4;
            low = state[i*4+j] & 0x0f;
            state[i*4+j] = SBox[high][low];
        }
    }
}
// 逆字节替换
void AES::SubBytes1(byte *state)
{
    uint8_t high;
    uint8_t low;
    for(int i = 0; i < 4; i++){
        for(int j = 0; j < 4; j++){
            high = state[4*i+j] >> 4;
            low = state[4*i+j] & 0x0f;
            state[4*i+j] = SBoxInv[high][low];
        }
    }
}
/*
 * 行移位，i行左循环移位i个字节
 */
void AES::ShiftRows(byte *state)
{
    for(int i = 0; i < 4; i++){

        word ta = (word)state[4*i+0] << 24;
        word tb = (word)state[4*i+1] << 16;
        word tc = (word)state[4*i+2] << 8;
        word td = (word)state[4*i+3];

        word t = ta | tb | tc | td;

        t = RotWord(t, i);

        state[4*i+0] = (byte)((t >> 24) & 0xff);
        state[4*i+1] = (byte)((t >> 16) & 0xff);
        state[4*i+2] = (byte)((t >> 8) & 0xff);
        state[4*i+3] = (byte)(t & 0xff);
    }
}
/*
 * 逆行移位：i行右移i个字
 */
void AES::ShiftRows1(byte *state)
{
    for(int i = 0; i < 4; i++){
        word ta = (word)state[4*i+0] << 24;
        word tb = (word)state[4*i+1] << 16;
        word tc = (word)state[4*i+2] << 8;
        word td = (word)state[4*i+3];

        word t = ta | tb | tc | td;

        t = RotWord1(t, i);

        state[4*i+0] = (byte)((t >> 24) & 0xff);
        state[4*i+1] = (byte)((t >> 16) & 0xff);
        state[4*i+2] = (byte)((t >> 8) & 0xff);
        state[4*i+3] = (byte)(t & 0xff);

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
//byte AES::mul(byte a, byte b)
//{
//    byte p = 0, h;

//    for(int i = 0; i < 8; i++){
//        if((b & byte(1)) != 0){
//            p ^= a;
//        }
//        h = (byte) (a & byte(0x80));
//        a <<= 1;
//        if (h != 0) {
//            a ^= 0x1b; /* x^8 + x^4 + x^3 + x + 1 */
//        }
//        b >>= 1;
//    }
//    return p;
//}





byte AES::GFMul2(byte s) {
    byte result = s << 1;
    byte a7 = result & 0x0100;

    if(a7 != 0) {
        result = result & 0x00ff;
        result = result ^ 0x1b;
    }

    return result;
}

byte AES::GFMul3(byte s) {
    return GFMul2(s) ^ s;
}

byte AES::GFMul4(byte s) {
    return GFMul2(GFMul2(s));
}

byte AES::GFMul8(byte s) {
    return GFMul2(GFMul4(s));
}

byte AES::GFMul9(byte s) {
    return GFMul8(s) ^ s;
}

 byte AES::GFMul11(byte s) {
    return GFMul9(s) ^ GFMul2(s);
}

byte AES::GFMul12(byte s) {
    return GFMul8(s) ^ GFMul4(s);
}

byte AES::GFMul13(byte s) {
    return GFMul12(s) ^ s;
}

byte AES::GFMul14(byte s) {
    return GFMul12(s) ^ GFMul2(s);
}

/**
 * GF上的二元运算
 */
//byte AES::mul(byte n, byte s) {
//    byte result;

//    if(n == 1)
//        result = s;
//    else if(n == 2)
//        result = GFMul2(s);
//    else if(n == 3)
//        result = GFMul3(s);
//    else if(n == 0x9)
//        result = GFMul9(s);
//    else if(n == 0xb)//11
//        result = GFMul11(s);
//    else if(n == 0xd)//13
//        result = GFMul13(s);
//    else if(n == 0xe)//14
//        result = GFMul14(s);

//    return result;
//}


