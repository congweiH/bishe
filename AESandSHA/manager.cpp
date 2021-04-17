#include "manager.h"

Manager::Manager()
{

}
// 静态变量必须要这样告诉编译器申请了空间
QString Manager::filepath = "";
byte Manager::key[16] = {};
QVector<byte> Manager::data = {};
int Manager::addSize = 0;
char Manager::addChar = 'a';
int Manager::mode = -1;
QString Manager::addSuffix = "_en";
/*
 * 从明文文件中读取数据到data，并填充
 */
void Manager::readDataFromPlainFile()
{
    data.clear();

    QFile file(filepath);
    file.open(QFile::ReadOnly);

    if(file.isOpen()){
        // 一次读取所有内容
        QByteArray cont = file.readAll();
        file.close();

        // 将读取的内容转换成我们这里用的byte类型
        for(int i = 0; i < cont.size(); i++){
            data.push_back((byte)cont[i]);
        }

        // 判断是否需要填充
        if(data.size() % 16 != 0){
            // 还需要addSize个字节满足要求
            addSize = 16 - data.size() % 16;
            // 需要填充addSize个字符, 这里随便填充都可以 我这里填充 'a'
            for(int i = 0; i < addSize; i++){
                data.push_back((byte)(addChar));
            }
        }
    }else{
        qDebug() << filepath << "打开失败";
    }
}

void Manager::readDataFromCipherFile()
{
    data.clear();

    QFile file(filepath);
    file.open(QFile::ReadOnly);

    if(file.isOpen()){

        QByteArray cont = file.readAll();

        // 读取模式, 0-15是密钥
        mode = ((byte)cont[16]).to_ulong();

        // 读取大小
        addSize = ((byte)cont[17]).to_ulong();

        for(int i = 18; i < cont.size(); i++){
            data.push_back((byte)cont[i]);
        }
        file.close();
    }else{
        qDebug() << filepath << "打开失败";
    }
}

void Manager::saveDataToPlainFile()
{
    QFileInfo fileInfo = QFileInfo(filepath);
    // 文件名字
    QString file_name = fileInfo.fileName();
    // 文件名字，不含后缀
    QString base_name = fileInfo.baseName();
    // 文件后缀, 去掉"_en"
    QString file_suffix = fileInfo.suffix();
    // 绝对路劲
    QString file_path = fileInfo.absolutePath();

    QString savePath = file_path + "/" + base_name + "." + file_suffix.remove(addSuffix);

    QFile file(savePath);
    file.open(QFile::ReadWrite);
    QDataStream out(&file);
    if(file.isOpen()){
        for(int i = 0; i < data.size(); i++){
            out << (qint8)data[i].to_ulong();
        }
        file.close();
    }else{
        qDebug() << savePath << "打开失败";
    }
}

void Manager::saveDataToCipherFile()
{
    // 在源文件同一位置自动生成加密文件，保持后缀一致，并添加作为加密文件的标识   xxx.text -> xxx.text_en   如果有"_en"标识这是一个加密文件
    QFileInfo fileInfo = QFileInfo(filepath);
    // 文件名字
    QString file_name = fileInfo.fileName();
    // 文件名字，不含后缀
    QString base_name = fileInfo.baseName();
    // 文件后缀
    QString file_suffix = fileInfo.suffix();
    // 绝对路劲
    QString file_path = fileInfo.absolutePath();


    QString savePath = file_path + "/" + base_name + "." + file_suffix + addSuffix;

    // 如果存在该文件，则删除已存在的文件，重新创建一个
    QFileInfo fileinfo(savePath);

    QFile file(savePath);
    file.open(QFile::ReadWrite);
    QDataStream out(&file);

    if(file.isOpen()){
        // 写密码, 占16个字节
        for(int i = 0; i < 16; i++){
            out << (qint8)key[i].to_ulong();
        }
        // 写模式，占一个字节
        out << (qint8)mode;

        // 写填充长度，因为128bit一组，也就是15个字节，所以最多填充16个字符，用4位其实就够了, 这里用8位
        out << (qint8)addSize;


        for(int i = 0; i < data.size(); i++){
            out << (qint8)data[i].to_ulong();
        }
        file.close();
    }else{
        qDebug() << savePath << "文件打开失败";
    }
}
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
void Manager::slip(word t, byte & a, byte & b, byte &c, byte & d)
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
/*
 * 将四个字节变成一个字
 */
word Manager::Word(byte a, byte b, byte c, byte d)
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
