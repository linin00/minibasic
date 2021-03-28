#ifndef EXECUTIONTREE_H
#define EXECUTIONTREE_H
#include <QString>
#include "expression.h"
#include "statement.h"
#include <QVector>
#include <QStringList>
#include <QMainWindow>
#include <QTextBrowser>
class Program {
private:
    statement* build(QString);//利用字符串，构建并返回语法树的指针
    expression* buildExp(QStringList);
    QString buildtree(int level, expression* exp);//递归
    QVector<IdentifierExp*> identifier;//存储标识符的向量
    QVector<statement*> program;//存储语句树的向量
public:
    Program() {//初始化
    }
    ~Program() {}//析构
    void read_from_input(QString inputStr);//从输入中读取内容存到input中
    void readVal(QString inputStr);//输入变量值
    void read_from_files(QString inputStr);//从文件中读取内容存到input中
    void build();//逐行读取input，判断合法性，构建语句树，并根据行号存储其指针到program向量中
    void clear();//清除
    QString buildtree(int i);//生成语法树的打印字符串
    void run();//运行语法树并生成结果的打印字符串
    int line = 0;
    expression* idenNow;

    QString input;//输入内容的字符串
    QString input_val;//运行时输入的值
    QString TREE;//打印的语法树
    QString RESULT;//打印的结果
    bool state = true;
    QTextBrowser* Code;
    QTextBrowser* Result;
    QTextBrowser* Tree;
    QTextEdit* Input;
};
#endif // EXECUTIONTREE_H
