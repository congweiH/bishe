#include "cryption.h"

Cryption::Cryption(QObject *parent) : QObject(parent)
{

}

void Cryption::encryption()
{

    qDebug() << Manager::mode;

    QFile readfile(Manager::filepath);
    readfile.open(QFile::ReadOnly);

    // 在源文件同一位置自动生成加密文件，保持后缀一致，并添加作为加密文件的标识   xxx.text -> xxx.text_en   如果有"_en"标识这是一个加密文件
    QFileInfo fileInfo = QFileInfo(Manager::filepath);
    // 文件名字
    QString file_name = fileInfo.fileName();
    // 文件名字，不含后缀
    QString base_name = fileInfo.baseName();
    // 文件后缀
    QString file_suffix = fileInfo.suffix();
    // 绝对路劲
    QString file_path = fileInfo.absolutePath();
    QString savePath = file_path + "/" + base_name + "." + file_suffix + Manager::addSuffix;

    QFile savefile(savePath);
    savefile.open(QFile::WriteOnly);
    QDataStream out(&savefile);
    // 写密码, 占16个字节
    for(int i = 0; i < 16; i++){
        out << (qint8)Manager::key[i];
    }
    // 写模式，占一个字节
    out << (qint8)Manager::mode;

    // 求填充长度
    Manager::addSize = 16 - readfile.size() % 16;

    // 写填充长度，因为128bit一组，也就是15个字节，所以最多填充16个字符，用4位其实就够了, 这里用8位
    out << (qint8)Manager::addSize;
    qDebug() << "写填充长度" << Manager::addSize;


    Manager::data = new byte[Manager::readBytes];

    long long sum = 0;

    // 一次读入文件的一部分进行加密
    while((Manager::dataSize=readfile.read((char *)Manager::data, Manager::readBytes))!=0){

        if(Manager::dataSize < Manager::readBytes){
            for(int i = 0; i < Manager::addSize; i++){
                Manager::data[Manager::dataSize++] = Manager::addChar;
            }
        }

        sum += Manager::dataSize;

        //value = 1.0  * sum / readfile.size();

        switch (Manager::mode) {
        case 0:
            mode.ECB_en();
            break;
        case 1:
            mode.CBC_en();
            break;
        case 2:
            mode.CFB_en();
            break;
        case 3:
            mode.OFB_en();
            break;
        case 4:
            mode.CTR_en();
            break;
        }
//        int af = clock();
        emit changeValue(100.0 * sum / readfile.size());
//        int ab = clock();

//        qDebug() << "一次发送信号的时间" << ab - af;

        // 写入文件
        for(int i = 0; i < Manager::dataSize; i++){
            out << (qint8)Manager::data[i];
        }
    }
    readfile.close();
    savefile.close();

}

void Cryption::decryption()
{

    qDebug() << Manager::mode;

    QFile readfile(Manager::filepath);
    readfile.open(QFile::ReadOnly);

    // 读取填充长度
    char * sizebuffer=new char[1];
    for(int i = 0; i < 18; i++){
        readfile.read(sizebuffer, 1);
        Manager::addSize = ((byte)sizebuffer[0]);
    }

    QFileInfo fileInfo = QFileInfo(Manager::filepath);
    // 文件名字
    QString file_name = fileInfo.fileName();
    // 文件名字，不含后缀
    QString base_name = fileInfo.baseName();
    // 文件后缀, 去掉"_en"
    QString file_suffix = fileInfo.suffix();
    // 绝对路劲
    QString file_path = fileInfo.absolutePath();

    QString savePath = file_path + "/" + base_name + "." + file_suffix.remove(Manager::addSuffix);
    QFile savefile(savePath);
    savefile.open(QFile::ReadWrite);
    QDataStream out(&savefile);

    int times;
    if(readfile.size() % Manager::readBytes == 0){
        times = readfile.size() / Manager::readBytes;
    }else{
        times = readfile.size() / Manager::readBytes + 1;
    }

    qDebug() << "times" << times;

    Manager::data = new byte[Manager::readBytes];

    int sum = 0;

    while((Manager::dataSize=readfile.read((char*)Manager::data, Manager::readBytes))!=0){


        // 更新进度条
        sum += Manager::dataSize;

        switch (Manager::mode) {
        case 0:
            mode.ECB_de();
            break;
        case 1:
            mode.CBC_de();
            break;
        case 2:
            mode.CFB_de();
            break;
        case 3:
            mode.OFB_de();
            break;
        case 4:
            mode.CTR_de();
            break;
        }

        emit changeValue(100.0 * sum / readfile.size());


        times --;
        if(times==0){
            // 去掉后面添加的字符
            qDebug() << "addSize" << Manager::addSize;
            while(Manager::addSize--){
                Manager::dataSize--;
            }
        }

        for(int i = 0; i < Manager::dataSize; i++){
            out << (qint8)Manager::data[i];
        }

    }
    readfile.close();
    savefile.close();
}
