#ifndef STATEMENT_H
#define STATEMENT_H
#include "expression.h"

class statement //语句类的父类
{
public:
    int lineNum = -1;
    QString root;//根节点
    statement(){//初始化
        root.clear();
    }
    ~statement(){}
private:
};

class RemStmt: public statement {//右子树为空，左子树是字符串
private:
    QString left;
    QString right;
public:
    RemStmt() {//初始化
        root = "REM";
        left.clear();
        right = nullptr;
    }
    ~RemStmt() {}
    void setLeft (QString inputStr) {
        left = inputStr;
    }
};

class LetStmt: public statement{//左子树是标识符，右子树是表达式
private:
    IdentifierExp* left;
    expression* right;
public:
    LetStmt() {
        root = "LET =";
        left = nullptr;
        right = nullptr;
    }
    void setLeft(IdentifierExp* in) {
        left = in;
    }
    void setRight(expression* in) {
        right = in;
    }
    ~LetStmt() {}
};

class PrintStmt: public statement {//左子树是表达式，右子树为空
private:
    expression* left;
    QString right;
public:
    PrintStmt() {
        root = "PRINT";
        left = nullptr;
        right = nullptr;
    }
    void setLeft(expression* le) {
        left = le;
    }
    ~PrintStmt() {}
};

class InputStmt: public statement {//左子树是表达式（标识符），右子树是空
private:
    expression* left;
    QString right;
public:
    InputStmt() {
        root = "INPUT";
        left = nullptr;
        right.clear();
    }
    void setLeft(expression* le) {
        left = le;
    }
    ~InputStmt() {}
};

class GotoStmt: public statement {//左子树是表达式，右子树是空
private:
    expression* left;
    QString right;
public:
    GotoStmt() {
        root = "GOTO";
        left = nullptr;
        right = nullptr;
    }
    void setLeft(expression* le) {
        left = le;
    }
    ~GotoStmt() {}
};

class IfStmt: public statement {//四个子节点
private:
    QString op;
    expression* then;
    expression* left;
    expression* right;
public:
    IfStmt() {
        root = "IF THEN";
        left = nullptr;
        right = nullptr;
        op = nullptr;
        then = nullptr;
    }
    void set(QString OP, expression* Th, expression* Le, expression* Ri) {
        op = OP;
        then = Th;
        left = Le;
        right = Ri;
    }
    ~IfStmt() {}
};

class EndStmt: public statement {//左右为空
private:
    QString left;
    QString right;
public:
    EndStmt() {
        root = "END";
        left = nullptr;
        right = nullptr;
    }
    ~EndStmt() {}
};

#endif // STATEMENT_H
