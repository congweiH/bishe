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
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QPushButton *pB_selectfile;
    QPushButton *pB_encryption;
    QLabel *label;
    QLineEdit *lineEdit_filepath;
    QTextEdit *textEdit_passwd;
    QPushButton *pB_decryption;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(882, 636);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QStringLiteral("centralwidget"));
        pB_selectfile = new QPushButton(centralwidget);
        pB_selectfile->setObjectName(QStringLiteral("pB_selectfile"));
        pB_selectfile->setGeometry(QRect(640, 80, 171, 41));
        pB_encryption = new QPushButton(centralwidget);
        pB_encryption->setObjectName(QStringLiteral("pB_encryption"));
        pB_encryption->setGeometry(QRect(70, 410, 291, 41));
        label = new QLabel(centralwidget);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(40, 160, 71, 31));
        lineEdit_filepath = new QLineEdit(centralwidget);
        lineEdit_filepath->setObjectName(QStringLiteral("lineEdit_filepath"));
        lineEdit_filepath->setGeometry(QRect(60, 80, 551, 41));
        textEdit_passwd = new QTextEdit(centralwidget);
        textEdit_passwd->setObjectName(QStringLiteral("textEdit_passwd"));
        textEdit_passwd->setGeometry(QRect(150, 200, 471, 151));
        pB_decryption = new QPushButton(centralwidget);
        pB_decryption->setObjectName(QStringLiteral("pB_decryption"));
        pB_decryption->setGeometry(QRect(460, 410, 291, 41));
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
        pB_selectfile->setText(QApplication::translate("MainWindow", "\351\200\211\346\213\251\346\226\207\344\273\266", Q_NULLPTR));
        pB_encryption->setText(QApplication::translate("MainWindow", "\345\212\240\345\257\206", Q_NULLPTR));
        label->setText(QApplication::translate("MainWindow", "\350\276\223\345\205\245\345\217\243\344\273\244", Q_NULLPTR));
        pB_decryption->setText(QApplication::translate("MainWindow", "\350\247\243\345\257\206", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
