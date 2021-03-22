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
    virtual double value() = 0;
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
    ~ConstantExp() {}
    double value();
};

class IdentifierExp: public expression {
public:
    QString root;
    IdentifierExp() {
        left = nullptr;
        right = nullptr;
        root.clear();
    }
    ~IdentifierExp() {}
    double value();
};

class CompoundExp: public expression {
public:
    QString root;
    CompoundExp() {
        left = nullptr;
        right = nullptr;
        root.clear();
    }
    ~CompoundExp() {}
    double value();
};
#endif // EXPRESSION_H
