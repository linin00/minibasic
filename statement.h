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
        right = nullptr;
        left = nullptr;
    }
    ~statement(){}
    virtual expression* Left(){
        return left;
    }
    virtual expression* Right(){
        return right;
    }
    virtual QString OP(){
        return "";
    }
    virtual expression* TAR(){
        return nullptr;
    }
private:
    expression* left;
    expression* right;
};

class RemStmt: public statement {//右子树为空，左子树是字符串
private:
    IdentifierExp* left;
    expression* right;
public:
    RemStmt() {//初始化
        root = "REM";
        left = nullptr;
        right = nullptr;
    }
    ~RemStmt() {}
    void setLeft (QString inputStr) {
        left = new IdentifierExp;
        left->root = inputStr;
    }
    virtual expression* Left(){
        return left;
    }
    virtual expression* Right(){
        return right;
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
    virtual expression* Left(){
        return left;
    }
    virtual expression* Right(){
        return right;
    }
};

class PrintStmt: public statement {//左子树是表达式，右子树为空
private:
    expression* left;
    expression* right;
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
    expression* Left(){
        return left;
    }
    expression* Right(){
        return right;
    }
};

class InputStmt: public statement {//左子树是表达式（标识符），右子树是空
private:
    expression* left;
    expression* right;
public:
    InputStmt() {
        root = "INPUT";
        left = nullptr;
        right = nullptr;
    }
    void setLeft(expression* le) {
        left = le;
    }
    ~InputStmt() {}
    expression* Left(){
        return left;
    }
    expression* Right(){
        return right;
    }
};

class GotoStmt: public statement {//左子树是表达式，右子树是空
private:
    expression* left;
    expression* right;
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
    expression* Left(){
        return left;
    }
    expression* Right(){
        return right;
    }
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
    expression* Left(){
        return left;
    }
    expression* Right(){
        return right;
    }
    QString OP(){
        return op;
    }
    expression* TAR(){
        return then;
    }
};

class EndStmt: public statement {//左右为空
private:
    expression* left;
    expression* right;
public:
    EndStmt() {
        root = "END";
        left = nullptr;
        right = nullptr;
    }
    ~EndStmt() {}
    expression* Left(){
        return left;
    }
    expression* Right(){
        return right;
    }
};

#endif // STATEMENT_H
