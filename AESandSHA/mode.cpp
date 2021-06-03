#include "mode.h"

Mode::Mode(QObject *parent): QObject(parent)
{

}

void Mode::ECB_en()
{
    aes.keyExpansion();
    for(int r = 0; r < Manager::dataSize; r += 16){
        aes.block_en(Manager::data+r);
    }
}

void Mode::ECB_de()
{
    aes.keyExpansion();
    for(int r = 0; r < Manager::dataSize; r += 16){
        // 解密一组
        aes.block_de(Manager::data+r);
    }
}

void Mode::CBC_en()
{
    aes.keyExpansion();
    byte IV[16] = {0x54, 0x34, 0x56, 0xf4, 0x34, 0x45, 0xa2, 0xd3, 0x78, 0x95, 0xab, 0xca, 0xcd, 0xdb, 0xde, 0xde};
    for(int r = 0; r < Manager::dataSize; r+=16){

        for(int i = 0; i < 16; i++){
            Manager::data[r+i] ^= IV[i];
        }
        aes.block_en(Manager::data+r);

        for(int i = 0; i < 16; i++){
            IV[i] = Manager::data[r+i];
        }
    }
}

void Mode::CBC_de()
{
    aes.keyExpansion();
    byte IV[16] = {0x54, 0x34, 0x56, 0xf4, 0x34, 0x45, 0xa2, 0xd3, 0x78, 0x95, 0xab, 0xca, 0xcd, 0xdb, 0xde, 0xde};
    byte temp[16];
    for(int r = 0; r < Manager::dataSize; r += 16){

        for(int i = 0; i < 16; i++){
            temp[i] = Manager::data[r+i];
        }

        // 解密一组
        aes.block_de(Manager::data+r);

        // 保存一组解密后的结果
        for(int i = 0; i < 16; i++){
            Manager::data[r+i] ^= IV[i];
            IV[i] = temp[i];
        }
    }
}

void Mode::CFB_en()
{
    aes.keyExpansion();
    byte IV[16] = {0x54, 0x34, 0x56, 0xf4, 0x34, 0x45, 0xa2, 0xd3, 0x78, 0x95, 0xab, 0xca, 0xcd, 0xdb, 0xde, 0xde};
    for(int r = 0; r < Manager::dataSize; r+=16){

        aes.block_en(IV);

        for(int i = 0; i < 16; i++){
            Manager::data[r+i] ^= IV[i];
            IV[i] = Manager::data[r+i];
        }
    }
}

void Mode::CFB_de()
{
    aes.keyExpansion();
    byte IV[16] = {0x54, 0x34, 0x56, 0xf4, 0x34, 0x45, 0xa2, 0xd3, 0x78, 0x95, 0xab, 0xca, 0xcd, 0xdb, 0xde, 0xde};
    byte temp[16];
    for(int r = 0; r < Manager::dataSize; r += 16){

        // 注意这里是加密一组
        aes.block_en(IV);

        for(int i = 0; i < 16; i++){
            temp[i] = Manager::data[r+i];
            Manager::data[r+i] ^= IV[i];
            IV[i] = temp[i];
        }
    }
}

void Mode::OFB_en()
{
    aes.keyExpansion();
    byte IV[16] = {0x54, 0x34, 0x56, 0xf4, 0x34, 0x45, 0xa2, 0xd3, 0x78, 0x95, 0xab, 0xca, 0xcd, 0xdb, 0xde, 0xde};
    for(int r = 0; r < Manager::dataSize; r+=16){

        aes.block_en(IV);

        for(int i = 0; i < 16; i++){
            Manager::data[r+i] ^= IV[i];
        }
    }
}

void Mode::OFB_de()
{
    aes.keyExpansion();
    byte IV[16] = {0x54, 0x34, 0x56, 0xf4, 0x34, 0x45, 0xa2, 0xd3, 0x78, 0x95, 0xab, 0xca, 0xcd, 0xdb, 0xde, 0xde};
    for(int r = 0; r < Manager::dataSize; r += 16){

        // 注意这里是加密一组
        aes.block_en(IV);

        for(int i = 0; i < 16; i++){
            Manager::data[r+i] ^= IV[i];
        }

    }
}

void Mode::CTR_en()
{
    aes.keyExpansion();
    byte IV[16] = {0x54, 0x34, 0x56, 0xf4, 0x34, 0x45, 0xa2, 0xd3, 0x78, 0x95, 0xab, 0xca, 0xcd, 0xdb, 0xde, 0xde};
    for(int r = 0; r < Manager::dataSize; r+=16){

        aes.block_en(IV);

        for(int i = 0; i < 16; i++){
            Manager::data[r+i] ^= IV[i];
        }

        // 向量加1
        plusOne(IV);
    }
}

void Mode::CTR_de()
{
    aes.keyExpansion();
    byte IV[16] = {0x54, 0x34, 0x56, 0xf4, 0x34, 0x45, 0xa2, 0xd3, 0x78, 0x95, 0xab, 0xca, 0xcd, 0xdb, 0xde, 0xde};
    for(int r = 0; r < Manager::dataSize; r += 16){

        aes.block_en(IV);

        for(int i = 0; i < 16; i++){
            Manager::data[r+i] ^= IV[i];
        }

        // 向量加1
        plusOne(IV);
    }
}

void Mode::plusOne(byte IV[16])
{
    word t = Manager::Word(IV[12], IV[13], IV[14], IV[15]);
    t += 1;
    Manager::slip(t, IV[12], IV[13], IV[14], IV[15]);
}
