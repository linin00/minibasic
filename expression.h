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
    double value(){
        return 0;
    }
private:
};

class ConstantExp: public expression {
public:
    double root;
    ConstantExp() {
        left = nullptr;
        right = nullptr;
        root = 0;
    }
    ConstantExp(double val) {
        left = nullptr;
        right = nullptr;
        root = val;
    }
    ~ConstantExp() {}
    double value();
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
};

class CompoundExp: public expression {
public:
    QString root;
    CompoundExp() {
        left = nullptr;
        right = nullptr;
        root.clear();
    }
    CompoundExp(QString op, expression* ri, expression* le) {
        left = le;
        right = ri;
        root = op;
    }
    ~CompoundExp() {}
    double value();
};
#endif // EXPRESSION_H
