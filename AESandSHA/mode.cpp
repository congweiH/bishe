#include "mode.h"

Mode::Mode()
{

}

void Mode::encryption()
{
    // 密钥扩展
    aes.keyExpansion();

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
    // 去掉添加的字符
    for(int i = 0; i < Manager::addSize; i++){
       Manager::data.pop_back();
    }
}

void Mode::CBC_en()
{

}

void Mode::CBC_de()
{

}

void Mode::CFB_en()
{

}

void Mode::CFB_de()
{

}

void Mode::OFB_en()
{

}

void Mode::OFB_de()
{

}

void Mode::CTR_en()
{

}

void Mode::CTR_de()
{

}
