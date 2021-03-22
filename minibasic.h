#ifndef MINIBASIC_H
#define MINIBASIC_H

#include <QMainWindow>
#include "program.h"
QT_BEGIN_NAMESPACE
namespace Ui { class minibasic; }
QT_END_NAMESPACE

class minibasic : public QMainWindow //minibasic类继承自QMainWindow，这个类封装所有需要的功能
{
    Q_OBJECT //不知道是什么

public:
    minibasic(QWidget *parent = nullptr); //构造函数
    ~minibasic();//析构函数

private slots:
    void on_textEdit_textChanged();//槽函数

    void on_pushButton_2_clicked();

    void on_pushButton_clicked();

    void on_pushButton_3_clicked();

private:
    Ui::minibasic *ui;//私有对象，不知道是什么
    Program *program = new Program;//语法树
};
#endif // MINIBASIC_H
