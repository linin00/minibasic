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
    program->Clear = ui->CLEAR;
    program->Load = ui->LOAD;

}
minibasic::~minibasic()
{
    delete ui;
}
void minibasic::on_INPUT_returnPressed()
{
    //输入变量
    if (program->debug && program->state) return;//debug状态不能修改代码
    if (!program->state) {
        QString temp = ui -> INPUT -> text();//捕获内容
        temp.replace(QRegularExpression("[\\s]+"), " ");
        temp.remove(QRegularExpression("^ +\\s*"));
        temp.remove(QRegularExpression("\\s* +$"));

        if(program->idenNow->type == "DOUBLE"){

             QString V = temp.split(" ")[1];//去掉输入提示
             V.remove(QRegularExpression("^ +\\s*"));
             V.remove(QRegularExpression("\\s* +$"));
             double val = V.toDouble();//转换为数字
             //如果输入合法
             if (QString::number(val) == V) {
                 *program->idenNow->setvalue() = val;//将输入记录到目的地址
             }
             //非法输入
             else {
                 QMessageBox::warning(this, "Warning!", temp + "\n非法输入");
                 return;
             }
         }
        else if (program->idenNow->type == "STR"){
            QStringList a = temp.split(" ");
            a.erase(a.begin());//去掉输入提示
            QString V = a.join(" ");
            V.remove(QRegularExpression("^ +\\s*"));
            V.remove(QRegularExpression("\\s* +$"));
            //如果输入合法
            if (V.contains(QRegularExpression("^[\\\'\\\"](.*)[\\\'\\\"]$"))) {
                program->idenNow->setvalue_str(V);//将输入记录到目的地址
            }
            //非法输入
            else {
                QMessageBox::warning(this, "Warning!", temp + "\n非法输入");
                return;
            }
        }
        program->idenNow = nullptr;//重置目标
        program->state = true;//切换程序状态
        program->showIdent();
        if (program->debug) program->Debug();
        else program->run();//继续运行
        return;
     }
    //否则
    //program->clear();//清空程序存储结构
    //清空两个窗口
    else {
        if (ui->INPUT->text() == "") return;
        ui->RESULT->clear();
        ui->TREE->clear();
        program -> read_from_input(ui -> INPUT ->text());//实时读取
        ui -> CODE -> setText(program->input);//实时显示

        if (!program->single) {
            ui -> INPUT -> clear();
        }
        else program->single = true;

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
            ui -> IDENT -> clear();

            tempStr = file.readAll();//读取文件内容
            program->read_from_files(tempStr);//记录到input
        }

        file.close();//关闭文件
    }
}
void minibasic::on_RUN_clicked()//运行程序，打印结果和语句树；在输入变量值时判断合法性以及读取变量值
{ 
     if (!program->state) return;//输入变量状态点击无用
     //运行程序
     if (!program->debug) program->TREE.clear();//debug状态不清空语法树
     ui -> TREE -> clear();//在多次连续运行时可以清空语法树窗口
     ui -> RESULT -> clear();//在多次连续运行时可以清空结果窗口
     ui -> IDENT -> clear();//在多次连续运行时可以清空变量窗口
     program->run();
     //运行结束之后清空结果树和语法树，但不影响显示
     program->RESULT.clear();
     program->TREE.clear();
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
    ui -> IDENT -> clear();
}
void minibasic::on_Debug_clicked()
{
    if (!program->state || !program->code.size()) return;
    if (program->debug && program->line == 0) {
        if (!program->debug_error)
            QMessageBox::information(this,"结束","被调试的程序正常结束");
        ui->LOAD->setEnabled(true);
        ui->CLEAR->setEnabled(true);
        program->debug = false;
        program->highlight_pos_now = -1;
        program->highlight();
        program->RESULT.clear();
        program->TREE.clear();
        return;
    }
    if (!program->debug) {
        program->identityoff();
        ui->LOAD->setEnabled(false);
        ui->CLEAR->setEnabled(false);
        program->TREE.clear();
        program->RESULT.clear();
        ui->IDENT->clear();
        ui->RESULT->clear();
        program->debug = true;
    }
    program->Debug();
}
