#include "minibasic.h"
#include "ui_minibasic.h"
#include <QTextEdit>
#include <QFileDialog>
#include <QMessageBox>
#include <QTextStream>
minibasic::minibasic(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::minibasic)
{
    ui->setupUi(this);
    /*将控件指针赋值给program的元素，方便修改显示内容*/
    program->Code = ui->CODE;
    program->Input = ui->INPUT;
    program->Result = ui->RESULT;
    program->Tree = ui->TREE;
}

minibasic::~minibasic()
{
    delete ui;
}


void minibasic::on_INPUT_textChanged()//从输入框获取命令
{
    if (!program->state) return;
    program->clear();
    ui -> RESULT -> setText(program->RESULT);
    ui -> TREE -> setText(program->TREE);
    program -> read_from_input(ui -> INPUT -> toPlainText());//实时读取
    ui -> CODE -> setText(program->input);//实时显示
}

void minibasic::on_LOAD_clicked()//从文件读取命令
{
    QString fileName = QFileDialog::getOpenFileName(//读取记录的是文件路径
            this,
            tr("选择basic程序文件"),
            "/",
            tr("basic(*.basic)"));
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
                program->clear();
                tempStr = file.readAll();//将文件内容全部读到tempStr
                ui -> RESULT -> clear();//清空三个窗口
                ui -> TREE -> clear();
                ui -> INPUT -> clear();
                program->read_from_files(tempStr);//记录到input
                ui -> CODE -> setText(program->input);//实时显示
            }
            file.close();//关闭文件
        }
}
void minibasic::on_RUN_clicked()//运行程序，打印结果和语句树
{
     if (!program->state) {//需要输入时
         QString temp = ui -> INPUT -> toPlainText();//获取
         QString V = temp.split("\n")[1];//去掉第一行的输入提示
         double val = V.toDouble();
         if (QString::number(val) == V) {
             *program->idenNow->value() = val;
             program->idenNow = nullptr;
             program->state = true;
             program->run();
         }
         else abort();//非法输入
     }
     else {
         program->build();
     }
}


void minibasic::on_CLEAR_clicked()//清空代码、运行结果、代码树
{
    program->clear();
    ui -> CODE -> setText(program->input);
    ui -> RESULT -> setText(program->RESULT);
    ui -> TREE -> setText(program->TREE);
    ui -> INPUT -> clear();
}
