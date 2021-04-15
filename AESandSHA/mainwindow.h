#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>
#include <QFileDialog>
#include <QMessageBox>
#include <QDebug>
#include <QFile>
#include <QByteArray>
#include "aes.h"
#include "sha.h"

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

    AES aes;

private slots:
    void on_pB_selectfile_clicked();

    void on_pB_encryption_clicked();

    void on_pB_decryption_clicked();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
