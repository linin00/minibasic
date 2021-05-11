#ifndef STATEMENT_H
#define STATEMENT_H
#include "expression.h"
#include <QRegularExpression>

class statement //语句类的父类
{
public:
    int lineNum = -1;
    QString root;//根节点
public:
    statement(){//初始化
        root.clear();
        right = nullptr;
        left = nullptr;
    }
    ~statement(){}
public:
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
    virtual QString get_form(){
        return "";
    }
    virtual QVector<expression*> get_list(){
        return {nullptr};
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

class PrintfStmt: public statement {//左子树是表达式，右子树为空
private:
    QString form;//输出格式
    QVector<expression*> list;//参数列表
    int argc = 0;//参数个数
    int argcr = 0;//需要参数个数
    bool isNumber(QString input) { //判断字符串是否是纯数字（非负浮点数）
        return (input.contains(QRegularExpression("^-?\\d+(\\.\\d+)?$"))? true : false);
    }
public:
    PrintfStmt() {
        root = "PRINTF";
        form.clear();
        list.clear();
    }
    ~PrintfStmt() {}
public:
    void addArg(expression* arg) {
        list.append(arg);
        argc++;
    }
    expression* getArg(int n) {
        if (n >= argc) {
            qDebug() << "下标越界";
            return nullptr;
        }
        else return list[n];
    }
    void setForm(QString f) {
        form = f;
    }
    QString OP() {
        QString result = "";
        if (argcr == 0) {//没有输出格式限制时
            qDebug()<<"无格式输出";
            if (list[0]->type == "DOUBLE") {
                result = QString::number(*list[0]->value());
            }
            else if(list[0]->type == "STR") {
                result = list[0]->value_str();
            }
        }
        else if (argcr >= 1) {//解析form
            QStringList formList = form.split(" ");//分割
            int size = formList.size();
            int argt = 0;
            for (int i = 0; i < size; i++) {
                if(isNumber(formList[i])) {
                    qDebug()<<"num";
                    result = result + formList[i];
                }
                else if (formList[i] == "{}") {
                    qDebug() << "{}";
                    if(list[argt]->type=="STR"){
                        result = result + list[argt]->value_str();
                        argt++;
                    }
                    else if (list[argt]->type == "DOUBLE") {
                        qDebug()<< "num";
                        result = result + QString::number(*list[argt]->value());
                        argt++;
                    }
                }
                else {//ident
                    qDebug()<<"string";
                    result = result + formList[i];
                }
            }
        }
        result = "\"" + result + "\"";
        return result;
    }

    QString get_form(){
        return form;
    }
    QVector<expression*> get_list(){
        return list;
    }
    bool equal() {
        QStringList formList = form.split(" ");//分割
        int size = formList.size();
        for (int i = 0; i < size; i++) {//计算实际参数个数
            if (formList[i] == "{}")
                argcr++;
        }
        qDebug() << argc << " " << argcr;
        if (argcr != argc && argcr != 0) return false;
        else return true;
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

class InputsStmt: public statement {//左子树是表达式（标识符），右子树是空
private:
    expression* left;
    expression* right;
public:
    InputsStmt() {
        root = "INPUTS";
        left = nullptr;
        right = nullptr;
    }
    void setLeft(expression* le) {
        left = le;
    }
    ~InputsStmt() {}
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
