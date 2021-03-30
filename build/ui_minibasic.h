/********************************************************************************
** Form generated from reading UI file 'minibasic.ui'
**
** Created by: Qt User Interface Compiler version 6.0.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MINIBASIC_H
#define UI_MINIBASIC_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTextBrowser>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_minibasic
{
public:
    QWidget *centralwidget;
    QLabel *label;
    QLabel *label_2;
    QLabel *label_3;
    QPushButton *LOAD;
    QPushButton *RUN;
    QPushButton *CLEAR;
    QLabel *label_4;
    QTextBrowser *CODE;
    QTextBrowser *RESULT;
    QTextBrowser *TREE;
    QTextEdit *INPUT;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *minibasic)
    {
        if (minibasic->objectName().isEmpty())
            minibasic->setObjectName(QString::fromUtf8("minibasic"));
        minibasic->resize(800, 823);
        minibasic->setStyleSheet(QString::fromUtf8(""));
        centralwidget = new QWidget(minibasic);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        label = new QLabel(centralwidget);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(30, 10, 72, 15));
        label->setStyleSheet(QString::fromUtf8("font: 9pt \"Aa\345\215\221\345\276\256\347\232\204\346\210\221\345\226\234\346\254\242\351\201\245\350\277\234\347\232\204\344\275\240 (\351\235\236\345\225\206\344\270\232\344\275\277\347\224\250)\";"));
        label_2 = new QLabel(centralwidget);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(400, 10, 72, 15));
        label_2->setStyleSheet(QString::fromUtf8("font: 9pt \"Aa\345\215\221\345\276\256\347\232\204\346\210\221\345\226\234\346\254\242\351\201\245\350\277\234\347\232\204\344\275\240 (\351\235\236\345\225\206\344\270\232\344\275\277\347\224\250)\";"));
        label_3 = new QLabel(centralwidget);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(30, 290, 91, 16));
        label_3->setStyleSheet(QString::fromUtf8("font: 9pt \"Aa\345\215\221\345\276\256\347\232\204\346\210\221\345\226\234\346\254\242\351\201\245\350\277\234\347\232\204\344\275\240 (\351\235\236\345\225\206\344\270\232\344\275\277\347\224\250)\";"));
        LOAD = new QPushButton(centralwidget);
        LOAD->setObjectName(QString::fromUtf8("LOAD"));
        LOAD->setGeometry(QRect(30, 570, 161, 24));
        LOAD->setStyleSheet(QString::fromUtf8("font: 9pt \"Aa\345\215\221\345\276\256\347\232\204\346\210\221\345\226\234\346\254\242\351\201\245\350\277\234\347\232\204\344\275\240 (\351\235\236\345\225\206\344\270\232\344\275\277\347\224\250)\";"));
        RUN = new QPushButton(centralwidget);
        RUN->setObjectName(QString::fromUtf8("RUN"));
        RUN->setGeometry(QRect(310, 570, 161, 24));
        RUN->setStyleSheet(QString::fromUtf8("font: 9pt \"Aa\345\215\221\345\276\256\347\232\204\346\210\221\345\226\234\346\254\242\351\201\245\350\277\234\347\232\204\344\275\240 (\351\235\236\345\225\206\344\270\232\344\275\277\347\224\250)\";"));
        CLEAR = new QPushButton(centralwidget);
        CLEAR->setObjectName(QString::fromUtf8("CLEAR"));
        CLEAR->setGeometry(QRect(610, 570, 161, 24));
        CLEAR->setStyleSheet(QString::fromUtf8("font: 9pt \"Aa\345\215\221\345\276\256\347\232\204\346\210\221\345\226\234\346\254\242\351\201\245\350\277\234\347\232\204\344\275\240 (\351\235\236\345\225\206\344\270\232\344\275\277\347\224\250)\";"));
        label_4 = new QLabel(centralwidget);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setGeometry(QRect(30, 620, 72, 15));
        CODE = new QTextBrowser(centralwidget);
        CODE->setObjectName(QString::fromUtf8("CODE"));
        CODE->setGeometry(QRect(30, 30, 351, 231));
        CODE->setStyleSheet(QString::fromUtf8(""));
        CODE->setFrameShape(QFrame::Box);
        RESULT = new QTextBrowser(centralwidget);
        RESULT->setObjectName(QString::fromUtf8("RESULT"));
        RESULT->setGeometry(QRect(400, 30, 371, 231));
        RESULT->setFrameShape(QFrame::Box);
        TREE = new QTextBrowser(centralwidget);
        TREE->setObjectName(QString::fromUtf8("TREE"));
        TREE->setGeometry(QRect(30, 310, 741, 231));
        TREE->setFrameShape(QFrame::Box);
        INPUT = new QTextEdit(centralwidget);
        INPUT->setObjectName(QString::fromUtf8("INPUT"));
        INPUT->setGeometry(QRect(33, 644, 741, 111));
        minibasic->setCentralWidget(centralwidget);
        menubar = new QMenuBar(minibasic);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 800, 25));
        minibasic->setMenuBar(menubar);
        statusbar = new QStatusBar(minibasic);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        minibasic->setStatusBar(statusbar);

        retranslateUi(minibasic);

        QMetaObject::connectSlotsByName(minibasic);
    } // setupUi

    void retranslateUi(QMainWindow *minibasic)
    {
        minibasic->setWindowTitle(QCoreApplication::translate("minibasic", "minibasic", nullptr));
        label->setText(QCoreApplication::translate("minibasic", "\344\273\243\347\240\201", nullptr));
        label_2->setText(QCoreApplication::translate("minibasic", "\350\277\220\350\241\214\347\273\223\346\236\234", nullptr));
        label_3->setText(QCoreApplication::translate("minibasic", "\350\257\255\345\217\245\344\270\216\350\257\255\346\263\225\346\240\221", nullptr));
        LOAD->setText(QCoreApplication::translate("minibasic", "\350\275\275\345\205\245\344\273\243\347\240\201\357\274\210LOAD\357\274\211", nullptr));
        RUN->setText(QCoreApplication::translate("minibasic", "RUN", nullptr));
        CLEAR->setText(QCoreApplication::translate("minibasic", "CLEAR", nullptr));
        label_4->setText(QCoreApplication::translate("minibasic", "\345\221\275\344\273\244\350\276\223\345\205\245\347\252\227\345\217\243", nullptr));
    } // retranslateUi

};

namespace Ui {
    class minibasic: public Ui_minibasic {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MINIBASIC_H
