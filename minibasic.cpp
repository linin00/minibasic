#include "minibasic.h"
#include "ui_minibasic.h"
#include <QLineEdit>
#include <QFileDialog>
#include <QMessageBox>
#include <QTextStream>
#include <QRegularExpression>
minibasic::minibasic(QWidget *parent): QMainWindow(parent), ui(new Ui::minibasic)
{
    ui->setupUi(this);
    /*将控件指针赋值给program的元素，方便修改显示内容*/
    program->Code = ui->CODE;
    program->Input = ui->INPUT;
    program->Result = ui->RESULT;
    program->Tree = ui->TREE;
    program->Ident = ui->IDENT;

}
minibasic::~minibasic()
{
    delete ui;
}
void minibasic::on_INPUT_returnPressed()
{
    //输入变量
    if (!program->state) {
         //获取输入框内容并处理
         QString temp = ui -> INPUT -> text();//捕获内容
         temp.replace(QRegularExpression("[\\s]+"), " ");
         temp.remove(QRegularExpression("^ +\\s*"));
         temp.remove(QRegularExpression("\\s* +$"));
         QString V = temp.split(" ")[1];//去掉输入提示
         V.remove(QRegularExpression("^ +\\s*"));
         V.remove(QRegularExpression("\\s* +$"));
         double val = V.toDouble();//转换为数字
         //如果输入合法
         if (QString::number(val) == V) {
             *program->idenNow->setvalue() = val;//将输入记录到目的地址
             program->idenNow = nullptr;//重置目标
             program->state = true;//切换程序状态
             program->run();//继续运行
         }
         //非法输入
         else {
             QMessageBox::warning(this, "Warning!", temp + "\n非法输入");
         }
         return;
     }
    //否则
    //program->clear();//清空程序存储结构
    //清空两个窗口
    else {
        ui->RESULT->clear();
        ui->TREE->clear();
        if (ui->INPUT->text() == "") return;
        program -> read_from_input(ui -> INPUT ->text());//实时读取
        ui -> CODE -> setText(program->input);//实时显示
        ui -> INPUT -> clear();

        if (program->inputFF && !program->file.empty()) {//如果现在在读文件而且没读完
            ui->INPUT->setText(program->file[0]);//设置下一条从文件获取的输入
            program->file.pop_front();
        }
        else if (program->inputFF && !program->file.empty()) program->inputFF = false;
        program->highlight();
        return;
    }
}
void minibasic::on_LOAD_clicked()//从文件读取命令
{
    QString fileName = QFileDialog::getOpenFileName(//读取记录的是文件路径
            this,
            tr("选择basic程序文件"),
            "/",
            tr("basic(*)"));
    if (fileName.isEmpty()) {//如果没有这个文件
            QMessageBox::warning(this, "警告!", "没有找到文件");
        }
    else {//如果有这个文件
        QFile file(fileName);
        QString tempStr;
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {//如果误写为WriteOnly，文件内容会被擦除
            //program->program = nullptr;
            QMessageBox::warning(this, "Warning!", "Failed to open the basic program!");
        }
        else {//打开成功
            program->clear();//清空所有存储结构

            ui -> RESULT -> clear();//清空窗口
            ui -> TREE -> clear();
            ui -> INPUT -> clear();
            ui -> CODE -> clear();

            tempStr = file.readAll();//读取文件内容
            program->read_from_files(tempStr);//记录到input
        }

        file.close();//关闭文件
    }
}
void minibasic::on_RUN_clicked()//运行程序，打印结果和语句树；在输入变量值时判断合法性以及读取变量值
{ 
     if (!program->state) return;
     //运行程序
     ui -> TREE -> clear();//在多次连续运行时可以清空语法树窗口
     ui -> RESULT -> clear();//在多次连续运行时可以清空结果窗口
     ui -> IDENT -> clear();//在多次连续运行时可以清空变量窗口
     program->run();
}
void minibasic::on_CLEAR_clicked()//清空代码、运行结果、代码树，注意先后
{
    //清空所有存储结构
    program->clear();

    //清空四个显示窗口
    ui -> CODE -> clear();
    ui -> RESULT -> clear();
    ui -> TREE -> clear();
    ui -> INPUT -> clear();
}


void minibasic::on_Debug_clicked()
{

}
