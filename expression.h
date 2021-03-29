#ifndef EXPRESSION_H
#define EXPRESSION_H
#include <QString>
#include <QtMath>
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
    QString root;
    double val;
    virtual QString show() = 0;
    virtual double* value(){
        return &val;
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
public:
    QString root;//标识符字符串（名称）
    IdentifierExp() {
        left = nullptr;//左节点为空
        right = nullptr;//右节点为空
        root.clear();
    }
    ~IdentifierExp() {}
    double* value() {
        return &val;
    }
    void setRoot(QString inputStr) {
        root = inputStr;
    }
    QString show() {
        return root;
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
    CompoundExp(QString op, expression* le, expression* ri) {//参数处理顺序为从右到左
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
        else if (root == "**"){//暂不考虑特殊情况
            //if (*left->value() < 0 && *right->value() < 1) abort();
            //if (*left->value() == 0 && *right->value() == 0) abort();
            val = pow(*left->value(), *right->value());
        }
        return &val;
    }
    QString show() {
        return root;
    }
};
#endif // EXPRESSION_H
