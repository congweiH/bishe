/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.9.9
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QPushButton *pB_encryption;
    QPushButton *pB_decryption;
    QGroupBox *groupBox;
    QPushButton *pB_selectfile;
    QLineEdit *lineEdit_filepath;
    QGroupBox *groupBox_2;
    QPushButton *pB_savepasswd;
    QPushButton *pB_passwdfile;
    QCheckBox *cB_usepasswdfile;
    QTextEdit *textEdit_passwd;
    QGroupBox *groupBox_3;
    QComboBox *cB_mode;
    QProgressBar *progressBar;
    QPushButton *pB_cancle;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(882, 515);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QStringLiteral("centralwidget"));
        pB_encryption = new QPushButton(centralwidget);
        pB_encryption->setObjectName(QStringLiteral("pB_encryption"));
        pB_encryption->setGeometry(QRect(180, 370, 141, 41));
        pB_decryption = new QPushButton(centralwidget);
        pB_decryption->setObjectName(QStringLiteral("pB_decryption"));
        pB_decryption->setGeometry(QRect(510, 370, 161, 41));
        groupBox = new QGroupBox(centralwidget);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        groupBox->setGeometry(QRect(20, 30, 841, 81));
        pB_selectfile = new QPushButton(groupBox);
        pB_selectfile->setObjectName(QStringLiteral("pB_selectfile"));
        pB_selectfile->setGeometry(QRect(670, 20, 141, 41));
        lineEdit_filepath = new QLineEdit(groupBox);
        lineEdit_filepath->setObjectName(QStringLiteral("lineEdit_filepath"));
        lineEdit_filepath->setGeometry(QRect(30, 20, 631, 41));
        groupBox_2 = new QGroupBox(centralwidget);
        groupBox_2->setObjectName(QStringLiteral("groupBox_2"));
        groupBox_2->setGeometry(QRect(20, 120, 671, 241));
        pB_savepasswd = new QPushButton(groupBox_2);
        pB_savepasswd->setObjectName(QStringLiteral("pB_savepasswd"));
        pB_savepasswd->setGeometry(QRect(520, 200, 131, 28));
        pB_passwdfile = new QPushButton(groupBox_2);
        pB_passwdfile->setObjectName(QStringLiteral("pB_passwdfile"));
        pB_passwdfile->setGeometry(QRect(160, 30, 111, 28));
        cB_usepasswdfile = new QCheckBox(groupBox_2);
        cB_usepasswdfile->setObjectName(QStringLiteral("cB_usepasswdfile"));
        cB_usepasswdfile->setGeometry(QRect(40, 40, 111, 19));
        textEdit_passwd = new QTextEdit(groupBox_2);
        textEdit_passwd->setObjectName(QStringLiteral("textEdit_passwd"));
        textEdit_passwd->setGeometry(QRect(30, 60, 631, 131));
        groupBox_3 = new QGroupBox(centralwidget);
        groupBox_3->setObjectName(QStringLiteral("groupBox_3"));
        groupBox_3->setGeometry(QRect(700, 120, 161, 241));
        cB_mode = new QComboBox(groupBox_3);
        cB_mode->setObjectName(QStringLiteral("cB_mode"));
        cB_mode->setGeometry(QRect(30, 110, 91, 31));
        progressBar = new QProgressBar(centralwidget);
        progressBar->setObjectName(QStringLiteral("progressBar"));
        progressBar->setGeometry(QRect(20, 430, 661, 21));
        progressBar->setValue(24);
        pB_cancle = new QPushButton(centralwidget);
        pB_cancle->setObjectName(QStringLiteral("pB_cancle"));
        pB_cancle->setGeometry(QRect(710, 420, 121, 31));
        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName(QStringLiteral("menubar"));
        menubar->setGeometry(QRect(0, 0, 882, 26));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName(QStringLiteral("statusbar"));
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", Q_NULLPTR));
        pB_encryption->setText(QApplication::translate("MainWindow", "\345\212\240\345\257\206", Q_NULLPTR));
        pB_decryption->setText(QApplication::translate("MainWindow", "\350\247\243\345\257\206", Q_NULLPTR));
        groupBox->setTitle(QApplication::translate("MainWindow", "\346\226\207\344\273\266", Q_NULLPTR));
        pB_selectfile->setText(QApplication::translate("MainWindow", "\351\200\211\346\213\251\346\226\207\344\273\266", Q_NULLPTR));
        groupBox_2->setTitle(QApplication::translate("MainWindow", "\345\217\243\344\273\244", Q_NULLPTR));
        pB_savepasswd->setText(QApplication::translate("MainWindow", "\344\277\235\345\255\230\345\217\243\344\273\244\345\210\260\346\226\207\344\273\266\344\270\255", Q_NULLPTR));
        pB_passwdfile->setText(QApplication::translate("MainWindow", "\351\200\211\346\213\251\345\257\206\351\222\245\346\226\207\344\273\266", Q_NULLPTR));
        cB_usepasswdfile->setText(QApplication::translate("MainWindow", "\344\275\277\347\224\250\345\212\240\345\257\206\346\226\207\344\273\266", Q_NULLPTR));
        groupBox_3->setTitle(QApplication::translate("MainWindow", "\346\250\241\345\274\217", Q_NULLPTR));
        cB_mode->clear();
        cB_mode->insertItems(0, QStringList()
         << QApplication::translate("MainWindow", "ECB", Q_NULLPTR)
         << QApplication::translate("MainWindow", "CBC", Q_NULLPTR)
         << QApplication::translate("MainWindow", "CFB", Q_NULLPTR)
         << QApplication::translate("MainWindow", "OFB", Q_NULLPTR)
         << QApplication::translate("MainWindow", "CTR", Q_NULLPTR)
        );
        pB_cancle->setText(QApplication::translate("MainWindow", "\345\217\226\346\266\210", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
