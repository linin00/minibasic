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
    void on_INPUT_textChanged();//槽函数

    void on_LOAD_clicked();

    void on_RUN_clicked();

    void on_CLEAR_clicked();

private:
    Program *program = new Program;//语法树
    Ui::minibasic *ui;//私有对象，不知道是什么
};
#endif // MINIBASIC_H
