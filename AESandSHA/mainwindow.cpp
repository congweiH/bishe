#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // 程序已启动，加密和解密按钮都不能按，因为这个时候没有选择文件和口令
    ui->pB_decryption->setDisabled(true);
    ui->pB_encryption->setDisabled(true);
    // 选择密钥文件初始化disabled
    ui->pB_passwdfile->setDisabled(true);

    valueChanged(0);
    // 线程
    enThread = new EnThread(this);
    deThread = new DeThread(this);
    connect(&enThread->cryption, &Cryption::changeValue, this, &MainWindow::valueChanged);
    connect(&deThread->cryption, &Cryption::changeValue, this, &MainWindow::valueChanged);

}

MainWindow::~MainWindow()
{
    qDebug() << "析构函数执行";
    delete Manager::data;
    delete enThread;
    delete deThread;
    delete ui;
}

bool MainWindow::checkPasswd()
{
    QFile file(Manager::filepath);
    file.open(QFile::ReadOnly);
    if(file.isOpen()){
        // 这里读第一行就可以了
        QByteArray cont = file.readLine();
        // 读取密码
        byte p[16];
        for(int i = 0; i < 16; i++){
            p[i] = (byte)cont[i];
        }
        file.close();

        // 判断和用户输入的是否一致
        for(int i = 0; i < 16; i++){
            if(Manager::key[i] != p[i]){
                return false;
            }
        }
        return true;
    }else{
        qDebug() << Manager::filepath << "打开失败";
        return false;
    }
}

void MainWindow::setPasswd()
{
    // 说明用的是文件
    if(ui->cB_usepasswdfile->isChecked()==true){
        QFile file(ui->textEdit_passwd->toPlainText());
        file.open(QFile::ReadOnly);
        QTextStream in(&file);
        passwd = in.readAll();
        file.close();
    }else{
        passwd = ui->textEdit_passwd->toPlainText();
    }
    qDebug() << passwd;
}

void MainWindow::getMode()
{
    QFile file(Manager::filepath);
    file.open(QFile::ReadOnly);
    if(file.isOpen()){
        QByteArray cont = file.readLine();
        Manager::mode = ((byte)cont[16]);
        file.close();
    }else{
        qDebug() << Manager::filepath << "打开失败";
    }
    qDebug() << Manager::mode;
}

void MainWindow::init()
{

    setPasswd();

    word res[5];
    sha.sha1(passwd, res);

    // 设置密钥
    Manager::setKey(res);
}
/*
 * 选择加密文件
 */
void MainWindow::on_pB_selectfile_clicked()
{
    QString filePath = QFileDialog::getOpenFileName(this, tr("文件对话框"), tr("."));
    ui->lineEdit_filepath->setText(filePath);

    // 检查文件是否合法，这里不会自动调用，需要手动调用
    on_lineEdit_filepath_editingFinished();
}
/*
 * 加密按钮
 */
void MainWindow::on_pB_encryption_clicked()
{
    init();

    if(passwd == ""){
        QMessageBox::warning(this, "错误", "密码错误");
        return;
    }
    // 设置模式
    Manager::mode = ui->cB_mode->currentIndex();

    ui->statusbar->showMessage("加密中...");

    // 加密
    enThread->start();
    connect(enThread, &EnThread::isDone, this, [=](){
        ui->progressBar->setValue(100);

        // 提示加密完成
        ui->statusbar->showMessage("加密完成!");

        enThread->quit();

    });
}
/*
 * 解密
 */
void MainWindow::on_pB_decryption_clicked()
{
    init();

    if(checkPasswd()==false || passwd == ""){
        QMessageBox::warning(this, "错误", "密码错误");
        return;
    }

    getMode();

    ui->cB_mode->setCurrentIndex(Manager::mode);

    ui->statusbar->showMessage("解密中...");
    // 解密
    deThread->start();
    connect(deThread, &DeThread::isDone, this, [=](){


        ui->progressBar->setValue(100);

        // 提示解密完成
        ui->statusbar->showMessage("解密完成!");

        deThread->quit();
    });

}

void MainWindow::on_lineEdit_filepath_editingFinished()
{
    // 设置文件路径   加入到全局配置
    Manager::filepath = ui->lineEdit_filepath->text();
    QFileInfo fileinfo = QFileInfo(Manager::filepath);

    // 如果是一个有效文件
    if(fileinfo.exists() && fileinfo.isFile()){
        // 删除提示
        ui->lineEdit_filepath->setStyleSheet("background:#ffffff;");
        // 显示文件大小
        ui->statusbar->showMessage(Manager::filepath + " 文件大小:" + QString("%1").arg(fileinfo.size()) + "B");

        // 如果是一个加密文件
        if(fileinfo.suffix().contains(Manager::addSuffix)){
            ui->cB_mode->setDisabled(true);
            ui->pB_encryption->setDisabled(true);
            ui->pB_decryption->setDisabled(false);
        }else{
            ui->cB_mode->setDisabled(false);
            ui->pB_encryption->setDisabled(false);
            ui->pB_decryption->setDisabled(true);
        }
    }else{  // 如果不是一个有效的文件
        // 加密和解密按钮都不能按
        ui->pB_encryption->setDisabled(true);
        ui->pB_decryption->setDisabled(true);
        // 提示报错
        ui->lineEdit_filepath->setStyleSheet("background:#ff0000;");
        ui->statusbar->showMessage("无效的文件路径!");
    }
}
void MainWindow::on_cB_usepasswdfile_stateChanged(int state)
{
    if(state == Qt::Checked){
        ui->pB_passwdfile->setDisabled(false);
        ui->pB_savepasswd->setDisabled(true);
        ui->textEdit_passwd->setReadOnly(true);
    }else{
        ui->pB_passwdfile->setDisabled(true);
        ui->pB_savepasswd->setDisabled(false);
        ui->textEdit_passwd->setReadOnly(false);
    }
}

void MainWindow::on_pB_passwdfile_clicked()
{
    QString filepath = QFileDialog::getOpenFileName();
    ui->textEdit_passwd->setText(filepath);
}

void MainWindow::on_pB_savepasswd_clicked()
{
    QString filepath = QFileDialog::getSaveFileName(this, "保存到", "passwd", tr("*.txt"));

    QFile file(filepath);
    file.open(QFile::WriteOnly);
    if(file.isOpen()){
        QTextStream out(&file);
        QString cont = ui->textEdit_passwd->toPlainText();
        out << cont;
        file.close();
    }else{
        qDebug() << filepath << "打开失败";
    }
}


void MainWindow::valueChanged(double value)
{
    ui->progressBar->setValue(value);
}

void MainWindow::on_pB_cancle_clicked()
{
    if(enThread->isRunning()){
        qDebug() << "en";
        enThread->terminate();
        enThread->wait();
    }
    if(deThread->isRunning()){
        qDebug() << "de";
        deThread->terminate();
        deThread->wait();
    }
    valueChanged(0);
    delete[] Manager::data;
    ui->statusbar->showMessage("取消成功!");
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    qDebug() << "关闭事件触发";
    if(enThread->isRunning()){
        qDebug() << "en";
        enThread->terminate();
        enThread->wait();
    }
    if(deThread->isRunning()){
        qDebug() << "de";
        deThread->terminate();
        deThread->wait();
    }
    delete enThread;
    delete deThread;
    valueChanged(0);
    delete[] Manager::data;
    // 告诉程序这是正常退出
    exit(0);
}
