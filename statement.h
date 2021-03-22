#ifndef STATEMENT_H
#define STATEMENT_H
#include "expression.h"

template <class K, class V>
class statement
{
public:
    statement(){
        root.clear();
        left = nullptr;
        right = nullptr;
    }
    ~statement(){}
    QString root;
    K* left;
    V* right;
private:
};

class RemStmt: public statement<QString, QString> {
public:
    RemStmt() {
        root = "REM";
        left->clear();
        right = nullptr;
    }
    ~RemStmt() {}
};

class LetStmt: public statement<IdentifierExp, expression> {
public:
    LetStmt() {
        root = "LET =";
        left = nullptr;
        right = nullptr;
    }
    ~LetStmt() {}
};

class PrintStmt: public statement<expression, QString> {
public:
    PrintStmt() {
        root = "PRINT";
        left = nullptr;
        right = nullptr;
    }
    ~PrintStmt() {}
};

class InputStmt: public statement<expression, QString> {
public:
    InputStmt() {
        root = "INPUT";
        left = nullptr;
        right = nullptr;
    }
    ~InputStmt() {}
};

class GotoStmt: public statement<expression, QString> {
public:
    GotoStmt() {
        root = "GOTO";
        left = nullptr;
        right = nullptr;
    }
    ~GotoStmt() {}
};

class IfStmt: public statement<expression, expression> {
public:
    QString op;
    expression* then;
    IfStmt() {
        root = "IF THEN";
        left = nullptr;
        right = nullptr;
        op.clear();
        then = nullptr;
    }
    ~IfStmt() {}
};

class EndStmt: public statement<QString, QString> {
public:
    EndStmt() {
        root = "END";
        left = nullptr;
        right = nullptr;
    }
    ~EndStmt() {}
};

#endif // STATEMENT_H
