#ifndef EXPRESSION_H
#define EXPRESSION_H
#include <QString>
class expression
{
public:
    expression(){
        left = nullptr;
        right = nullptr;
    }
    ~expression(){}
    expression* left;
    expression* right;
    QString root;
    double value(){
        return 0;
    }
    virtual QString show() = 0;
private:
};

class ConstantExp: public expression {
public:
    double rootVal;
    QString root;
    ConstantExp() {
        left = nullptr;
        right = nullptr;
        rootVal = 0;
        root.clear();
    }
    ConstantExp(double val) {
        left = nullptr;
        right = nullptr;
        rootVal = val;
        root = QString::number(val);
    }
    ~ConstantExp() {}
    double value();
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
    double value();
    void setRoot(QString inputStr) {
        root = inputStr;
    }
    QString show() {
        return root;
    }
};

class CompoundExp: public expression {
public:
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
    double value();
    QString show() {
        return root;
    }
};
#endif // EXPRESSION_H
