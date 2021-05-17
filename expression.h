#ifndef EXPRESSION_H
#define EXPRESSION_H
#include <QString>
#include <QtMath>
#include <QMessageBox>
#include <QProcess>
#include <QDebug>
class expression
{
public:
    double val;
    QString val_str;
    expression(){
        left = nullptr;
        right = nullptr;
        val = -1;
    }
    ~expression(){}
    expression* left;
    expression* right;
    QString root = "testing";
    QString type = "DOUBLE";
    virtual QString show() = 0;
    virtual double* value(){
        return &val;
    }
    virtual void turn_on() {
    }
    virtual void turn_off() {
    }
    virtual bool DONE(QString) {
        return true;
    }
    virtual double* setvalue() {
        return nullptr;
    }
    virtual QString value_str() {
        return nullptr;
    }
    virtual void setvalue_str(QString) {
        return;
    }
    virtual void setType(QString) {

    }
private:
};

class CompoundExp: public expression {
public:
    double val;
    QString root;
    CompoundExp() {
        left = nullptr;
        right = nullptr;
        root.clear();
    }
    CompoundExp(QString op, expression* ri, expression* le) {//参数处理顺序为从右到左
        left = le;
        right = ri;
        root = op;
    }
    ~CompoundExp() {}
    double* value() {
        if (root == "+") {
            val = *left->value() + *right->value();
        }
        else if (root == "-"){
            val = *left->value() - *right->value();
        }
        else if (root == "*"){
            val = *left->value() * *right->value();
        }
        else if (root == "/"){
            if (*right->value() == 0) abort();
            val = *left->value() / *right->value();
        }
        else if (root == "**"){
            val = pow(*left->value(), *right->value());
        }
        return &val;
    }
    QString show() {
        return root;
    }
};

class ConstantExp: public expression {
public:
    double val;
    QString root;
    ConstantExp() {
        left = nullptr;
        right = nullptr;
        val = 0;
        root.clear();
    }
    ConstantExp(double Val) {//实数常量
        type = "DOUBLE";
        left = nullptr;
        right = nullptr;
        val = Val;
        root = QString::number(val);
    }
    ConstantExp(QString Val) {//字符串常量
        type = "STR";
        left = nullptr;
        right = nullptr;
        val = -1;
        root = Val;
    }
    ~ConstantExp() {}
    double* value() {
        return &val;
    }
    QString value_str() {
        return root;
    }
    QString show() {
        return root;
    }
};

class IdentifierExp: public expression {
private:
    double done = false;//是否已启用
public:
    QString root;//标识符字符串（名称）
    IdentifierExp() {
        left = nullptr;
        right = nullptr;//右节点为空
        root.clear();
    }
    ~IdentifierExp() {}

    double* value() {
        if (!done) {//如果还没有声明
            QMessageBox::warning(NULL, "Warning!", root + "\n标识符不存在");
            qDebug()<<root + "\nDOUBLE标识符不存在";
        }
        return &val;
    }
    double* setvalue() {
        return &val;
    }
    QString value_str() {
        if (!done) {//如果还没有声明
            QMessageBox::warning(NULL, "Warning!", root + "\n标识符不存在");
            qDebug()<<root + "\nSTR标识符不存在";
        }
        return val_str;
    }
    void setvalue_str(QString val) {
        val_str = val;
    }

    void setRoot(QString inputStr) {
        root = inputStr;
    }
    QString show() {
        return root;
    }
    void turn_on() {
        done = true;
        qDebug() << "声明变量" << root;
    }
    void turn_off() {
        done = false;
    }
    bool DONE(QString str) {
        if (done && str == root) return true;
        return false;
    }
    bool DONE() {
        return done;
    }
    void setType(QString str) {
        type = str;
    }
};
#endif // EXPRESSION_H
