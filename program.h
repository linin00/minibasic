#ifndef EXECUTIONTREE_H
#define EXECUTIONTREE_H
#include <QString>
#include "expression.h"
#include "statement.h"
#include <QVector>
#include <QStringList>
#include <QTextBrowser>
#include <QMessageBox>
#include <QLineEdit>
#include <QPushButton>
class Program {
public://构建
    void build();//逐行读取input，判断合法性，构建语句树，并根据行号存储其指针到program向量中
private:
    statement* build(QString);//利用字符串，构建并返回可执行语句树的指针
    expression* buildExp(QStringList);
    QString buildtree(int level, expression* exp);//递归
    QString buildtree(int i);//生成语法树的打印字符串
public:
    Program() {//初始化
    }
    ~Program() {}//析构
public://读取
    void read_from_input(QString inputStr);//从输入中读取内容存到input中
    void readVal(QString inputStr);//输入变量值
    void read_from_files(QString inputStr);//从文件中读取内容存到input中
public://控制
    void clear();//清除
    void highlight();//高亮
    void showIdent();
    void identityoff();//变量假死
public://运行
    void run();//运行语法树并生成结果的打印字符串
    void RUN();//运行语法树并生成结果的打印字符串
    void Debug();//运行语法树并生成结果的打印字符串
private:
    QVector<IdentifierExp*> identifier;//存储标识符的向量
    QVector<statement*> program;//存储可执行语句树的向量
    int numOfError = 0;
public://程序数据
    QString input;//用于显示的字符串
    QStringList code;//解析后的代码表
    QString TREE;//打印的语法树
    QString RESULT;//打印的结果
    QStringList file;//从文件获取的代码表
    QList<int> highlight_pos_err;//高亮定位_红
    int highlight_pos_now = -1;//高亮定位_绿
    expression* idenNow = nullptr;//输入的变量地址
public://程序状态
    int line = 0;//执行序号
    bool state = true;//执行状态
    bool inputFF = false;//输入状态
    bool error = false;//构建是否出错
    bool debug = false;
    bool single = false;
    bool debug_error = false;
public://控件指针
    QTextBrowser* Code;
    QTextBrowser* Result;
    QTextBrowser* Tree;
    QTextBrowser* Ident;
    QLineEdit* Input;
    QPushButton* Load;
    QPushButton* Clear;


};
#endif // EXECUTIONTREE_H
