#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>
#include <QFileDialog>
#include <QMessageBox>
#include <QDebug>
#include <QFile>
#include <QByteArray>
#include <QFileInfo>
#include <QDataStream>
#include <QTextStream>
#include <QProgressBar>
#include "aes.h"
#include "sha.h"
#include "mode.h"
#include "enthread.h"
#include "dethread.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    SHA sha;

    EnThread *enThread;
    DeThread *deThread;

    QString passwd;

    // 检查口令是否合法
    bool checkPasswd();

    void setPasswd();

    void getMode();
    // 加密解密之前的工作
    void init();

    void valueChanged(double value);
private slots:
    void on_pB_selectfile_clicked();

    void on_pB_encryption_clicked();

    void on_pB_decryption_clicked();

    void on_lineEdit_filepath_editingFinished();


    void on_cB_usepasswdfile_stateChanged(int state);

    void on_pB_passwdfile_clicked();

    void on_pB_savepasswd_clicked();

    void on_pB_cancle_clicked();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
