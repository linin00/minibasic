#ifndef EXECUTIONTREE_H
#define EXECUTIONTREE_H
#include <QString>
#define EXCUTE_STAGE false;
#define INPUT_STAGE true;
#include "expression.h"
#include "statement.h"
#include <QVector>
#include <QStringList>
class Program {
private:
    statement* build(QString);//利用字符串，构建并返回语法树的指针
    expression* buildExp(QStringList);
    bool state;//输入状态变量
    QVector<IdentifierExp*> identifier;//存储标识符的向量
    QVector<statement*> program;//存储语句树的向量
public:
    QString input;//输入内容的字符串
    QString TREE;//打印的语法树
    QString RESULT;//打印的结果
    Program() {//初始化
      state = INPUT_STAGE;
    }
    ~Program() {}//析构
    void read_from_input(QString inputStr);//从输入中读取内容存到input中
    void read_from_files(QString inputStr);//从文件中读取内容存到input中
    void build();//逐行读取input，判断合法性，构建语句树，并根据行号存储其指针到program向量中
    void clear();//清除
};
#endif // EXECUTIONTREE_H
