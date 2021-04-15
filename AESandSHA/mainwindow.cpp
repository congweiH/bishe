#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}
/*
 * 选择加密文件
 */
void MainWindow::on_pB_selectfile_clicked()
{
    QString filePath = QFileDialog::getOpenFileName(this, tr("文件对话框"), tr("."));
    ui->lineEdit_filepath->setText(filePath);
}
/*
 * 加密按钮
 */
void MainWindow::on_pB_encryption_clicked()
{
    QString filePath = ui->lineEdit_filepath->text();
    QString passwd = ui->textEdit_passwd->toPlainText();

//    qDebug() << passwd;

    // 如果没有选择文件 或者 没有输入口令，则发出提示信息
    if(filePath == "" || passwd == ""){
        QMessageBox::warning(this, tr("错误"), tr("未选择文件或未输入口令!"), QMessageBox::Ok);
        return;
    }
    // 利用sha-1将口令变成密钥
    // sha-1加密后的结果
    word res[5];
    sha.sha1(passwd, res);

    qDebug() << "【开始加密】";

    aes.encryption(filePath, res);

    qDebug() << "【加密完成】";
}
/*
 * 解密
 */
void MainWindow::on_pB_decryption_clicked()
{
    QString filepath = QFileDialog::getOpenFileName(this, "选择文件", ".");
    QString passwd = ui->textEdit_passwd->toPlainText();
    // 利用sha-1将口令变成密钥
    // sha-1加密后的结果
    qDebug() << "开始解密";
    qDebug() << passwd;
    word res[5];
    sha.sha1(passwd, res);
    // 解密
    aes.decryption(filepath, res);

    qDebug() << "【解密完成】";
}
