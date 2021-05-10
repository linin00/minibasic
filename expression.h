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
    expression(){
        left = nullptr;
        right = nullptr;
        val = -1;
    }
    ~expression(){}
    expression* left;
    expression* right;
    QString root = "testing";
    double val;
    virtual QString show() = 0;
    virtual double* value(){
        return &val;
    }
    virtual void turn_on() {
    }
    virtual void turn_off() {
    }
    virtual bool DONE(QString str) {
        return true;
    }
    virtual double* setvalue() {
        return nullptr;
    }
private:
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
    ConstantExp(double Val) {
        left = nullptr;
        right = nullptr;
        val = Val;
        root = QString::number(val);
    }
    ~ConstantExp() {}
    double* value() {
        return &val;
    }
    QString show() {
        return root;
    }
};

class IdentifierExp: public expression {
private:
    double val = 0;//标识符的值
    double done = false;//是否已启用
public:
    QString root;//标识符字符串（名称）
    IdentifierExp() {
        left = nullptr;//左节点为空
        right = nullptr;//右节点为空
        root.clear();
    }
    ~IdentifierExp() {}

    double* value() {
        if (!done) {//如果还没有声明
            QMessageBox::warning(NULL, "Warning!", root + "\n标识符不存在");
            //abort();
        }
        return &val;
    }
    double* setvalue() {
        return &val;
    }

    void setRoot(QString inputStr) {
        root = inputStr;
    }
    QString show() {
        return root;
    }
    void turn_on() {
        done = true;
        qDebug() << "声明成功";
    }
    void turn_off() {
        done = false;
    }
    bool DONE(QString str) {
        if (done && str == root) return true;
        return false;
    }
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
#endif // EXPRESSION_H
