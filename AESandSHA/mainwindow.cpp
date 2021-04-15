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
 * ѡ������ļ�
 */
void MainWindow::on_pB_selectfile_clicked()
{
    QString filePath = QFileDialog::getOpenFileName(this, tr("�ļ��Ի���"), tr("."));
    ui->lineEdit_filepath->setText(filePath);
}
/*
 * ���ܰ�ť
 */
void MainWindow::on_pB_encryption_clicked()
{
    QString filePath = ui->lineEdit_filepath->text();
    QString passwd = ui->textEdit_passwd->toPlainText();

//    qDebug() << passwd;

    // ���û��ѡ���ļ� ���� û���������򷢳���ʾ��Ϣ
    if(filePath == "" || passwd == ""){
        QMessageBox::warning(this, tr("����"), tr("δѡ���ļ���δ�������!"), QMessageBox::Ok);
        return;
    }
    // ����sha-1����������Կ
    // sha-1���ܺ�Ľ��
    word res[5];
    sha.sha1(passwd, res);

    qDebug() << "����ʼ���ܡ�";

    aes.encryption(filePath, res);

    qDebug() << "��������ɡ�";
}
/*
 * ����
 */
void MainWindow::on_pB_decryption_clicked()
{
    QString filepath = QFileDialog::getOpenFileName(this, "ѡ���ļ�", ".");
    QString passwd = ui->textEdit_passwd->toPlainText();
    // ����sha-1����������Կ
    // sha-1���ܺ�Ľ��
    qDebug() << "��ʼ����";
    qDebug() << passwd;
    word res[5];
    sha.sha1(passwd, res);
    // ����
    aes.decryption(filepath, res);

    qDebug() << "��������ɡ�";
}
