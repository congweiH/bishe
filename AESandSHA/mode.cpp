#include "mode.h"

Mode::Mode()
{

}

void Mode::encryption()
{
    // 密钥扩展
    aes.keyExpansion();

    qDebug() << Manager::mode;

    switch (Manager::mode) {
    case 0:
        ECB_en();
        break;
    case 1:
        CBC_en();
        break;
    case 2:
        CFB_en();
        break;
    case 3:
        OFB_en();
        break;
    case 4:
        CTR_en();
        break;
    }
}

void Mode::decryption()
{
    // 密钥扩展
    aes.keyExpansion();

    qDebug() << Manager::mode;

    switch (Manager::mode) {
    case 0:
        ECB_de();
        break;
    case 1:
        CBC_de();
        break;
    case 2:
        CFB_de();
        break;
    case 3:
        OFB_de();
        break;
    case 4:
        CTR_de();
        break;
    }
}

void Mode::ECB_en()
{
    for(int i = 0; i < Manager::data.size(); i+=16){
        int p = i;
        byte state[4][4];
        for(int i = 0; i < 4; i++){
            for(int j = 0;j < 4; j++){
                state[i][j] = Manager::data[p++];
            }
        }
        aes.block_en(state);
        p = i;
        for(int i = 0; i < 4; i++){
            for(int j = 0;j < 4; j++){
                Manager::data[p++] = state[i][j];
            }
        }
    }
}

void Mode::ECB_de()
{
    for(int i = 0; i < Manager::data.size(); i += 16){
        int p = i;
        byte state[4][4];
        for(int i = 0; i < 4; i++){
            for(int j = 0; j < 4; j++){
                state[i][j] = Manager::data[p++];
            }
        }
        // 解密一组
        aes.block_de(state);

        p = i;
        // 保存一组加密后的结果
        for(int i = 0; i < 4; i++){
            for(int j = 0; j < 4; j++){
                Manager::data[p++] = state[i][j];
            }
        }
    }
}

void Mode::CBC_en()
{
    byte IV[16] = {0x54, 0x34, 0x56, 0xf4, 0x34, 0x45, 0xa2, 0xd3, 0x78, 0x95, 0xab, 0xca, 0xcd, 0xdb, 0xde, 0xde};
    for(int i = 0; i < Manager::data.size(); i+=16){
        int p = i;
        int count = 0;
        byte state[4][4];
        for(int i = 0; i < 4; i++){
            for(int j = 0;j < 4; j++){
                state[i][j] = Manager::data[p++];
                state[i][j] ^= IV[count++];
            }
        }
        aes.block_en(state);
        p = i;
        count = 0;
        for(int i = 0; i < 4; i++){
            for(int j = 0;j < 4; j++){
                Manager::data[p++] = state[i][j];
                IV[count++] = state[i][j];
            }
        }
    }
}

void Mode::CBC_de()
{
    byte IV[16] = {0x54, 0x34, 0x56, 0xf4, 0x34, 0x45, 0xa2, 0xd3, 0x78, 0x95, 0xab, 0xca, 0xcd, 0xdb, 0xde, 0xde};
    byte temp[16];
    for(int i = 0; i < Manager::data.size(); i += 16){
        int p = i;
        int count = 0;
        byte state[4][4];
        for(int i = 0; i < 4; i++){
            for(int j = 0; j < 4; j++){
                state[i][j] = Manager::data[p++];
                temp[count++] = state[i][j];
            }
        }
        // 解密一组
        aes.block_de(state);

        p = i;
        count = 0;
        // 保存一组解密后的结果
        for(int i = 0; i < 4; i++){
            for(int j = 0; j < 4; j++){
                state[i][j] ^= IV[count++];
                Manager::data[p++] = state[i][j];
            }
        }
        for(int i = 0; i < 16; i++){
            IV[i] = temp[i];
        }
    }
}

void Mode::CFB_en()
{
    byte IV[16] = {0x54, 0x34, 0x56, 0xf4, 0x34, 0x45, 0xa2, 0xd3, 0x78, 0x95, 0xab, 0xca, 0xcd, 0xdb, 0xde, 0xde};
    for(int i = 0; i < Manager::data.size(); i+=16){
        int p = i;
        int count = 0;
        byte state[4][4];
        for(int i = 0; i < 4; i++){
            for(int j = 0;j < 4; j++){
                state[i][j] = IV[count++];
            }
        }
        aes.block_en(state);
        p = i;
        count = 0;
        for(int i = 0; i < 4; i++){
            for(int j = 0; j < 4; j++){
                Manager::data[p] ^= state[i][j];
                IV[count++] = Manager::data[p];
                p++;
            }
        }
    }
}

void Mode::CFB_de()
{
    byte IV[16] = {0x54, 0x34, 0x56, 0xf4, 0x34, 0x45, 0xa2, 0xd3, 0x78, 0x95, 0xab, 0xca, 0xcd, 0xdb, 0xde, 0xde};
    for(int i = 0; i < Manager::data.size(); i += 16){
        int p = i;
        int count = 0;
        byte state[4][4];
        for(int i = 0; i < 4; i++){
            for(int j = 0; j < 4; j++){
                state[i][j] = IV[count++];
            }
        }
        // 注意这里是加密一组
        aes.block_en(state);

        for(int i = 0; i < 16; i++){
            IV[i] = Manager::data[i];
        }

        p = i;
        count = 0;
        // 保存一组解密后的结果
        for(int i = 0; i < 4; i++){
            for(int j = 0; j < 4; j++){
                Manager::data[p++] ^= state[i][j];
            }
        }
    }
}

void Mode::OFB_en()
{
    byte IV[16] = {0x54, 0x34, 0x56, 0xf4, 0x34, 0x45, 0xa2, 0xd3, 0x78, 0x95, 0xab, 0xca, 0xcd, 0xdb, 0xde, 0xde};
    for(int i = 0; i < Manager::data.size(); i+=16){
        int p = i;
        int count = 0;
        byte state[4][4];
        for(int i = 0; i < 4; i++){
            for(int j = 0;j < 4; j++){
                state[i][j] = IV[count++];
            }
        }
        aes.block_en(state);

        count = 0;
        for(int i = 0; i < 4; i++){
            for(int j = 0; j < 4; j++){
                IV[count++] = state[i][j];
            }
        }

        p = i;
        for(int i = 0; i < 4; i++){
            for(int j = 0; j < 4; j++){
                Manager::data[p++] ^= state[i][j];
            }
        }
    }
}

void Mode::OFB_de()
{
    byte IV[16] = {0x54, 0x34, 0x56, 0xf4, 0x34, 0x45, 0xa2, 0xd3, 0x78, 0x95, 0xab, 0xca, 0xcd, 0xdb, 0xde, 0xde};
    for(int i = 0; i < Manager::data.size(); i += 16){
        int p = i;
        int count = 0;
        byte state[4][4];
        for(int i = 0; i < 4; i++){
            for(int j = 0; j < 4; j++){
                state[i][j] = IV[count++];
            }
        }
        // 注意这里是加密一组
        aes.block_en(state);

        count = 0;
        for(int i = 0; i < 4; i++){
            for(int j = 0; j < 4; j++){
                IV[count++] = state[i][j];
            }
        }

        p = i;
        // 保存一组解密后的结果
        for(int i = 0; i < 4; i++){
            for(int j = 0; j < 4; j++){
                Manager::data[p++] ^= state[i][j];
            }
        }
    }
}

void Mode::CTR_en()
{
    byte IV[16] = {0x54, 0x34, 0x56, 0xf4, 0x34, 0x45, 0xa2, 0xd3, 0x78, 0x95, 0xab, 0xca, 0xcd, 0xdb, 0xde, 0xde};
    for(int i = 0; i < Manager::data.size(); i+=16){
        int p = i;
        int count = 0;
        byte state[4][4];
        for(int i = 0; i < 4; i++){
            for(int j = 0;j < 4; j++){
                state[i][j] = IV[count++];
            }
        }
        aes.block_en(state);
        p = i;
        for(int i = 0; i < 4; i++){
            for(int j = 0; j < 4; j++){
                Manager::data[p++] ^= state[i][j];
            }
        }

        // 向量加1
        plusOne(IV);
    }
}

void Mode::CTR_de()
{
    byte IV[16] = {0x54, 0x34, 0x56, 0xf4, 0x34, 0x45, 0xa2, 0xd3, 0x78, 0x95, 0xab, 0xca, 0xcd, 0xdb, 0xde, 0xde};
    for(int i = 0; i < Manager::data.size(); i+=16){
        int p = i;
        int count = 0;
        byte state[4][4];
        for(int i = 0; i < 4; i++){
            for(int j = 0;j < 4; j++){
                state[i][j] = IV[count++];
            }
        }
        aes.block_en(state);
        p = i;
        for(int i = 0; i < 4; i++){
            for(int j = 0; j < 4; j++){
                Manager::data[p++] ^= state[i][j];
            }
        }

        // 向量加1
        plusOne(IV);
    }
}

void Mode::plusOne(byte IV[16])
{
    bitset<128> p;
    int t = 0;
    for(int i = 15; i >= 0; i--){
        for(int j = 0; j < 8; j++){
            p[t++] = IV[i][j];  // p[0] 是低位
        }
    }
    bitset<128> incment(1);
    int c = 0;
    for(int i = 0; i < 128; i++){
        p[i] = (p[i] + incment[i] + c) % 2;
        if(p[i] + incment[i] + c >= 2){
            c = 1;
        }else if(p[i] + incment[i] + c < 2){
            c = 0;
        }
    }
    t = 0;
    for(int i = 15; i >= 0; i--){
        for(int j = 0; j < 8; j++){
            IV[i][j] = p[t++];  // p[0] 是低位
        }
    }

}
