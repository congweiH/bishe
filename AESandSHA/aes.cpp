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
        w[i] = Word(key[4*i], key[4*i+1], key[4*i+2], key[4*i+3]);
    }
    // 扩充40个新列，总共构成44列, j代表第几轮
    for(int i = 4; i < 44; i++){
        word temp = w[i-1];
        if(i % 4 == 0){
            temp = SubWord(RotWord(temp)) ^ Rcon[i/4];
        }
        w[i] = w[i-4] ^ temp;
    }

//    for(int i = 0; i < 44; i++){
//        cout << w[i] << endl;
//    }
}
/*
 * 对word中每一个字进行S盒变换
 */
word AES::SubWord(word t)
{
    byte a[4];
    slip(t, a[0], a[1], a[2], a[3]);

    for(int i = 0; i < 4; i++){
        a[i] = SubByte(a[i]);
    }
    // 将4个字节组成1个字
    return Word(a[0], a[1], a[2], a[3]);
}
byte AES::SubByte(byte t)
{
    bitset<4> high, low;
    // 将第一个字节分成两部分
    slip(t, high, low);
    // 高四位作为行值，低四位作为列值
    return SBox[high.to_ulong()][low.to_ulong()];
}

byte AES::SubByte1(byte t)
{
    bitset<4> high, low;
    // 将第一个字节分成两部分
    slip(t, high, low);
    // 高四位作为行值，低四位作为列值
    return SBoxInv[high.to_ulong()][low.to_ulong()];
}
/*
 * 左循环一个字节[t0, t1, t2, t3] -> [t1, t2, t3 ,t0]
 */
word AES::RotWord(word t)
{
    byte a[4];
    slip(t, a[0], a[1], a[2], a[3]);
    return Word(a[1], a[2], a[3], a[0]);
}

word AES::RotWord1(word t)
{
    byte a[4];
    slip(t, a[0], a[1], a[2], a[3]);
    return Word(a[3], a[0], a[1], a[2]);
}
/*
 * 加密一组的过程
 */
void AES::Do(byte state[4][4])
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
void AES::Do1(byte state[4][4])
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
void AES::addRoundKey(byte state[4][4], int start)
{
    byte a[4];
    // 一共4列
    for(int i = 0; i < 4; i++){
        // state第i列与 w 第i个字异或
        slip(w[start+i], a[0], a[1], a[2], a[3]);
        state[0][i] ^= a[0];
        state[1][i] ^= a[1];
        state[2][i] ^= a[2];
        state[3][i] ^= a[3];
    }
}
/*
 * 字节替换
 */
void AES::SubBytes(byte state[4][4])
{
    for(int i = 0; i < 4; i++){
        for(int j = 0; j < 4; j++){
            state[i][j] = SubByte(state[i][j]);
        }
    }
}
// 逆字节替换
void AES::SubBytes1(byte state[4][4])
{
    for(int i = 0; i < 4; i++){
        for(int j = 0; j < 4; j++){
            state[i][j] = SubByte1(state[i][j]);
        }
    }
}
/*
 * 行移位，i行左循环移位i个字节
 */
void AES::ShiftRows(byte state[4][4])
{
    for(int i = 0; i < 4; i++){
        word t = Word(state[i][0], state[i][1], state[i][2], state[i][3]);
        for(int j = 0; j < i; j++){ // 左移位i次
            RotWord(t);
        }
        slip(t, state[i][0], state[i][1], state[i][2], state[i][3]);
    }
}
/*
 * 逆行移位：i行右移i个字
 */
void AES::ShiftRows1(byte state[4][4])
{
    for(int i = 0; i < 4; i++){
        word t = Word(state[i][0], state[i][1], state[i][2], state[i][3]);
        for(int j = 0; j < i; j++){ // 右移位i次
            RotWord1(t);
        }
        slip(t, state[i][0], state[i][1], state[i][2], state[i][3]);
    }
}
/*
 * 列混淆
 */
void AES::MixColumns(byte state[4][4])
{
    byte temp[4][4];
    for(int i = 0; i < 4; i++){
        for(int j = 0; j < 4; j++){
            temp[i][j] = state[i][j];
        }
    }
    for(int j = 0; j < 4; j++){
        state[0][j] = mul(0x02, temp[0][j]) ^ mul(0x03, temp[1][j]) ^ mul(0x01, temp[2][j]) ^ mul(0x01, temp[3][j]);
        state[1][j] = mul(0x01, temp[0][j]) ^ mul(0x02, temp[1][j]) ^ mul(0x03, temp[2][j]) ^ mul(0x01, temp[3][j]);
        state[2][j] = mul(0x01, temp[0][j]) ^ mul(0x01, temp[1][j]) ^ mul(0x02, temp[2][j]) ^ mul(0x03, temp[3][j]);
        state[3][j] = mul(0x03, temp[0][j]) ^ mul(0x01, temp[1][j]) ^ mul(0x01, temp[2][j]) ^ mul(0x02, temp[3][j]);
    }
}

void AES::MixColumns1(byte state[4][4])
{
    byte temp[4][4];
    for(int i = 0; i < 4; i++){
        for(int j = 0; j < 4; j++){
            temp[i][j] = state[i][j];
        }
    }

    for(int j = 0; j < 4; j++){
        state[0][j] = mul(0x0E, temp[0][j]) ^ mul(0x0B, temp[1][j]) ^ mul(0x0D, temp[2][j]) ^ mul(0x09, temp[3][j]);
        state[1][j] = mul(0x09, temp[0][j]) ^ mul(0x0E, temp[1][j]) ^ mul(0x0B, temp[2][j]) ^ mul(0x0D, temp[3][j]);
        state[2][j] = mul(0x0D, temp[0][j]) ^ mul(0x09, temp[1][j]) ^ mul(0x0E, temp[2][j]) ^ mul(0x0B, temp[3][j]);
        state[3][j] = mul(0x0B, temp[0][j]) ^ mul(0x0D, temp[1][j]) ^ mul(0x09, temp[2][j]) ^ mul(0x0E, temp[3][j]);
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
/*
 * 读文件内容
 */
void AES::readFile(QString filepath)
{
    QFile file(filepath);
    file.open(QFile::ReadOnly);

    if(file.isOpen()){
        qDebug() << "test文件打开成功";
    }

    QByteArray cont = file.readAll();

    data.clear();

    // 将读取的内容转换成我们这里用的byte类型
    for(int i = 0; i < cont.size(); i++){
        data.push_back((byte)cont[i]);
    }

    qDebug() << "原始数据大小" << data.size();

    // 原始数据的大小
    int size1 = data.size();

    // 需要填充的个数
    addSize = 0;

    // 检查data是否符合要求。满足16的倍数
    if(data.size() % 16 != 0){
        // 还需要addSize个字节满足要求
        addSize = 16 - data.size() % 16;
        // 需要填充addSize个字符, 这里随便填充都可以 我这里填充 'a'
        for(int i = 0; i < addSize; i++){
            data.push_back((byte)('a'));
        }
    }
    // 填充的大小
    size = data.size() - size1;


    qDebug() << "加密前的大小" << data.size();

}


void AES::saveFile(QString filepath)
{
    QString filePath = QFileDialog::getOpenFileName(nullptr, "保存到", ".");
    // TODO 加密后的数据保存在同一个目录下的文件，同时密钥也要保存在一个文件中

    // TODO 将密钥写入到文件中



    QFile file(filePath);
    file.open(QFile::ReadWrite);
    QDataStream out(&file);

    qDebug() << "加密后的大小" << data1.size();

    int count = 0;

    if(file.isOpen()){
        qDebug() << "文件打开成功";
        for(int i = 0; i < data1.size(); i++){

            out << (qint8)data1[i].to_ulong();
        }
        file.close();
        qDebug() << "写文件完毕";

    }

    qDebug() << "写入文件后文件大小" << file.size();
//    for(int i = 0; i < data1.size(); i++){
//        cout << data1[i] << endl;
//    }
}
/*
 * 读取加密文件
 */
void AES::readFile1(QString filepath)
{
    QFile file(filepath);
    file.open(QFile::ReadOnly);
    if(file.isOpen()){
        QByteArray cont = file.readAll();
        data1.clear();
        for(int i = 0; i < cont.size(); i++){
            data1.push_back((byte)cont[i]);
        }
        file.close();
    }
}

void AES::saveFile1()
{

    cout << "填充的字符个数: " << size << endl;

    // 去掉添加的字符
    while(size--){
        data.pop_back();
    }
//    cout << "还原后:" << endl;
//    cout << data.size() << endl;
//    for(int i = 0; i < data.size(); i++){
//        cout << data[i] << endl;
//    }

    QString filePath = QFileDialog::getOpenFileName(nullptr, "保存到", ".");
    QFile file(filePath);
    file.open(QFile::ReadWrite);
    QDataStream out(&file);
    if(file.isOpen()){
        qDebug() << "retrun文件打开成功";
        for(int i = 0; i < data.size(); i++){
//            cout << (char*)&data[i] << endl;
//            file.write((char*)&data[i]);
            out << (qint8)data[i].to_ulong();
        }
        qDebug() << "写文件完毕";
        file.close();
    }
//    for(int i = 0; i < data1.size(); i++){
//        cout << data1[i] << endl;
//    }
}
/*
 * 从160位 选取 128位作为密钥存放在成员变量key中
 */
void AES::setKey(word res[5])
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

void AES::printState(byte state[4][4])
{
    for(int i = 0;i < 4; i++){
        for(int j = 0;j < 4; j++){
            cout << state[i][j] << ' ';
        }
        cout << endl;
    }
}

/*
 * 所有数据的加密
 */
void AES::encryption(QString filepath, word p[5])
{
    // 1. 设置密码
    setKey(p);
    // 2. 读取文件
    readFile(filepath);
    // 3. 密钥扩展
    keyExpansion();

//    cout << "源文件：" << endl;

//    for(int i = 0; i < data.size();i++){
//        cout << data[i] << endl;
//    }

    data1.clear();

    // 加密所有的数据
    for(int i = 0; i < data.size(); i += 16){
        int p = i;
        // 每组加密16个
        byte state[4][4];
        for(int i = 0; i < 4; i++){
            for(int j = 0; j < 4; j++){
                state[i][j] = data[p++];
            }
        }
        // 加密一组
        Do(state);

        // 保存一组加密后的结果
        for(int i = 0; i < 4; i++){
            for(int j = 0; j < 4; j++){
                data1.push_back(state[i][j]);
//                cout << state[i][j] << endl;
            }
        }
    }
    // 保存到文件
    saveFile(filepath);
//    cout << "加密后" << endl;
//    for(int i = 0; i < data1.size(); i++){
//        cout << data1[i] << endl;
//    }
}

void AES::decryption(QString filepath, word p[])
{
    // 1. 设置密码
    setKey(p);
    // 2. 读取文件, 将加密数据读取到data1中
    readFile1(filepath);
    // 3. 密钥扩展
    keyExpansion();
    // 4. 分组解密

    qDebug() << "======================";

    data.clear();

    qDebug() << "加密前的大小 " << data1.size();

//    for(int i = 0; i < data1.size(); i++){
//        cout << data1[i] << endl;
//    }
    for(int k = 0; k < data1.size(); k += 16){
        int t = k;
        byte state[4][4];
        for(int i = 0; i < 4; i++){
            for(int j = 0; j < 4; j++){
                state[i][j] = data1[t++];
            }
        }
        // 解密一组
        Do1(state);
        // 保存一组加密后的结果
        for(int i = 0; i < 4; i++){
            for(int j = 0; j < 4; j++){
                data.push_back(state[i][j]);
//                cout << state[i][j] << endl;
            }
        }
    }
    qDebug() << "加密后的大小 " << data.size();
//    for(int i = 0; i < data.size(); i++){
//        cout << data[i] << endl;
//    }
//    cout << "----------------------------------------" << endl;
    // 保存到文件
    qDebug() << "***********************************";
    saveFile1();
}

void AES::slip(word t, byte & a, byte & b, byte &c, byte & d)
{
    int len = 0;
    for(int i=0; i < 8; i++){
        d[i] = t[len++];
    }
    for(int i=0; i< 8; i++){
        c[i] = t[len++];
    }
    for(int i=0; i< 8; i++){
        b[i] = t[len++];
    }
    for(int i=0; i< 8; i++){
        a[i] = t[len++];
    }
}

void AES::slip(byte t, bitset<4> &a, bitset<4> &b)
{
    int p = 0;
    for(int i = 0; i < 4; i++){
        b[i] = t[p++];
    }
    for(int i = 0; i < 4; i++){
        a[i] = t[p++];
    }
}
/*
 * 将四个字节变成一个字
 */
word AES::Word(byte a, byte b, byte c, byte d)
{
    word t;
    int p = 0;
    for(int i = 0; i < 8; i++){
        t[p++] = d[i];
    }
    for(int i = 0; i < 8; i++){
        t[p++] = c[i];
    }
    for(int i = 0; i < 8; i++){
        t[p++] = b[i];
    }
    for(int i = 0; i < 8; i++){
        t[p++] = a[i];
    }
    return t;
}

