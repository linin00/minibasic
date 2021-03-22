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
}

minibasic::~minibasic()
{
    delete ui;
}


void minibasic::on_textEdit_textChanged()//从输入框获取命令
{
    program->read_from_input(ui -> textEdit-> toPlainText());
    ui -> textBrowser_2 -> setText(program->program);
}

void minibasic::on_pushButton_clicked()//从文件读取命令
{
    QString fileName = QFileDialog::getOpenFileName(//读取的是文件绝对路径
            this,
            tr("选择basic程序文件"),
            "/",
            tr("basic(*.basic)"));
    if (fileName.isEmpty()) {
            QMessageBox::warning(this, "Warning!", "Failed to open the basic program!");
        }
        else {
            QFile file(fileName);
            QString tempStr;
            if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {//如果误写为WriteOnly，文件内容会被擦除
                program->program = nullptr;
                QMessageBox::warning(this, "Warning!", "Failed to open the basic program!");
            }
            else {
                tempStr = file.readAll();
                program->read_from_files(tempStr);
            }
            file.close();
        }
    ui -> textBrowser_2 -> setText(program->program);
}
void minibasic::on_pushButton_2_clicked()//运行程序，打印结果和语句树
{

}


void minibasic::on_pushButton_3_clicked()//清空代码、运行结果、代码树
{
    program->clear();
    ui -> textBrowser_2 -> setText(program->program);
}
