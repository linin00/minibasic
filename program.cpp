#include "program.h"
#include <QStringList>
#include <iostream>
#include <QStack>
void Program::read_from_input(QString inputStr) {
    input = inputStr;
}

void Program::read_from_files(QString inputStr) {
    input = inputStr;
}
expression* Program::buildExp(QStringList inputList) {
    QStack<int> opOder;//优先级：加减：1；乘除：2；幂运算：3；左括号：0；右括号：4；
    QStack<expression*> stack;//符号栈
    QStack<QString> OP;//运算符栈，什么鬼乱七八糟

    /*开始build*/
    expression* result = nullptr;
    while (inputList.size()) {
        if (*inputList.begin() == "(") {
            OP.push(inputList.first());
            opOder.push(0);
            inputList.removeFirst();
        }
        else if (*inputList.begin() == "+") {
            if (opOder.top() < 1) {//如果前一个运算符的优先级低于 +
                OP.push(inputList.first());
                opOder.push(1);
                inputList.removeFirst();
                continue;
            }
            /*否则*/
            while (opOder.top() >= 1) {
                CompoundExp* temp = new CompoundExp(OP.pop(), stack.pop(), stack.pop());//运算符，右节点，左节点
                stack.push(temp);//压入构建的节点
                opOder.pop();//弹出一个运算符的同时弹出其优先级
            }//预示下面会出现大量重复代码。。。
            continue;
        }
        else if (*inputList.begin() == "-") {
            if (opOder.top() < 1) {//如果前一个运算符的优先级低于 -
                OP.push(inputList.first());
                opOder.push(1);
                inputList.removeFirst();
                continue;
            }
            /*否则*/
            while (opOder.top() >= 1) {
                CompoundExp* temp = new CompoundExp(OP.pop(), stack.pop(), stack.pop());//运算符，右节点，左节点
                stack.push(temp);//压入构建的节点
                opOder.pop();//弹出一个运算符的同时弹出其优先级
            }
            continue;
        }
        else if (*inputList.begin() == "*") {
            if (opOder.top() < 2) {//如果前一个运算符的优先级低于 *
                OP.push(inputList.first());
                opOder.push(2);
                inputList.removeFirst();
                continue;
            }
            /*否则*/
            while (opOder.top() >= 2) {
                CompoundExp* temp = new CompoundExp(OP.pop(), stack.pop(), stack.pop());//运算符，右节点，左节点
                stack.push(temp);//压入构建的节点
                opOder.pop();//弹出一个运算符的同时弹出其优先级
            }
            continue;
        }
        else if (*inputList.begin() == "/") {
            if (opOder.top() < 2) {//如果前一个运算符的优先级低于 /
                OP.push(inputList.first());
                opOder.push(2);
                inputList.removeFirst();
                continue;
            }
            /*否则*/
            while (opOder.top() >= 2) {
                CompoundExp* temp = new CompoundExp(OP.pop(), stack.pop(), stack.pop());//运算符，右节点，左节点
                stack.push(temp);//压入构建的节点
                opOder.pop();//弹出一个运算符的同时弹出其优先级
            }
            continue;
        }
        else if (*inputList.begin() == "**") {
            if (opOder.top() < 3) {//如果前一个运算符的优先级低于 **
                OP.push(inputList.first());
                opOder.push(3);
                inputList.removeFirst();
                continue;
            }
            /*否则*/
            while (opOder.top() >= 3) {
                CompoundExp* temp = new CompoundExp(OP.pop(), stack.pop(), stack.pop());//运算符，右节点，左节点
                stack.push(temp);//压入构建的节点
                opOder.pop();//弹出一个运算符的同时弹出其优先级
            }
            continue;
        }
        else if (*inputList.begin() == ")") {//如果即将压入的符号为右括号
            while (OP.top() != "(") {
                CompoundExp* temp = new CompoundExp(OP.pop(), stack.pop(), stack.pop());//运算符，右节点，左节点
                stack.push(temp);//压入构建的节点
                opOder.pop();//弹出一个运算符的同时弹出其优先级
            }
            //弹出右括号
            inputList.removeFirst();
            //将左括号弹出
            OP.pop();
            opOder.pop();
            continue;
        }
        else {
            QString temp = inputList.first();
            double val = temp.toDouble();

            if (QString::number(val) == temp) {//temp是常数
                ConstantExp* constant = new ConstantExp(val);
                stack.push(constant);
                inputList.removeFirst();
                continue;
            }
            /*如果temp不是常数*/
            int size = identifier.size();
            for (int i = 0; i < size; i++) {
                if (temp == identifier[i]->root) {
                    stack.push(identifier[i]);
                    break;
                }
                else if (i == size) { //不存在这个标识符，程序报错
                    abort();
                }
            }
        }
    }
    while (stack.size() != 1) {
        CompoundExp* temp = new CompoundExp(OP.pop(), stack.pop(), stack.pop());//运算符，右节点，左节点
        stack.push(temp);//压入构建的节点
        opOder.pop();//弹出一个运算符的同时弹出其优先级
    }
    result = stack.pop();
    return result;
}
void Program::build() {
    QStringList strList = input.split("/n");//以回车分割字符串，一行就是条指令
    int num = strList.size();
    statement* temp;
    for (int i = 0; i < num; i++) {//逐行生成语法树并插入语法树向量
        temp = build(strList[i]);//生成
        int size = program.size();
        for (int j = 0; j < size; j++) {//插入语法树
            if (temp->lineNum < program[j]->lineNum) continue;
            else if (temp->lineNum > program[j]->lineNum) {
                program.insert(j + 1, temp);
                break;
            }
            else if (temp->lineNum == program[j]->lineNum) {
                statement* temp2 = program[j];
                program[j] = temp;
                delete temp2;
                break;
            }
        }
        if (size == 0) {//插入语法树
            program.push_back(temp);
        }
    }
}
statement* Program::build(QString inputStr) {
    QStringList strList = inputStr.split(" ");
    int lineNum = -1;
    lineNum = strList[0].toDouble()? strList[0].toDouble() : -1;//如果是0，说明不是数字，行号记为-1，否则记录行号；
    if (lineNum != -1) {//如果有行号
        strList.removeFirst();//删除行号的字符串
    }
    /*根据指令生成特定的语法树，递归实现，太难了*/
    if (strList[0] == "REM") {
        RemStmt* result = new RemStmt;
        //设置left
        int size = strList.size();
        QString temp;
        for (int i = 1; i < size; i++) {//将REM后面的字符串记录到temp中
            temp = temp + " ";
            temp = temp + strList[i];
        }
        result->setLeft(temp);//设置节点中的left字符串
        result->lineNum = lineNum;//记录行号；

        return result;
    }
    else if (strList[0] == "LET") {//赋值语句
        LetStmt* result = new LetStmt;
        IdentifierExp* iden = nullptr;//处理左节点
        int size = identifier.size();
        for (int i = 0; i < size; i++) {
            if (identifier[i]->root == strList[1]) {//如果标识符已存在
                iden = identifier[i];//取其指针
                break;
            }
            if (i == size - 1) {//如果不存在，申请新的标识符，存入向量中
                iden = new IdentifierExp;
                iden->setRoot(strList[1]);
                identifier.push_back(iden);//存入向量中
            }
        }
        result->setLeft(iden);//记录赋值的对象

        expression* right = nullptr;
        strList.removeFirst();//删除LET
        strList.removeFirst();//删除标识符名称

        /*处理右节点，有可能是组合式或常数*/
        right = buildExp(strList);
        result->setRight(right);
        result->lineNum = lineNum;//记录行号；
        return result;

    }
    else if (strList[0] == "INPUT") {
        InputStmt* result = new InputStmt;
        if (strList.size() != 2) abort();
        //设置left
        int size = identifier.size();
        for (int i = 0; i < size; i++) {
            if (strList[1] == identifier[i]->root) {
                result->setLeft(identifier[i]);
                break;
            }
            if (i == size - 1) {
                IdentifierExp* temp = new IdentifierExp;
                temp->root = strList[1];
                result->setLeft(temp);
            }
        }

        result->lineNum = lineNum;//记录行号；
        return result;
    }
    else if (strList[0] == "PRINT") {
        PrintStmt* result = new PrintStmt;
        /*设置左节点*/
        strList.removeFirst();//删除PRINT
        result->setLeft(buildExp(strList));
        result->lineNum = lineNum;//记录行号；
        return result;
    }
    else if (strList[0] == "GOTO") {
        GotoStmt* result = new GotoStmt;
        /*设置左节点*/
        strList.removeFirst();//删除GOTO
        result->setLeft(buildExp(strList));
        result->lineNum = lineNum;//记录行号；
        return result;
    }
    else if (strList[0] == "IF") {
        IfStmt* result = new IfStmt;
        strList.removeFirst();//删除IF
        QString temp = strList.join(" ");//先合并~~~
        QStringList list = temp.split("THEN");//再分割。。
        if (list.size() != 2) abort();
        expression* tar = buildExp(list[1].split(" "));//目标表达式
        /*构建条件表达式*/
        if (list[1].contains(">")) {
            QStringList exp = list[1].split(">");
            if (exp.size() != 2) abort();//如果不能分成两个部分，报错
            result->set(">", tar, buildExp(exp[0].split(" ")), buildExp(exp[1].split(" ")));
        }
        else if (list[1].contains(">=")) {
            QStringList exp = list[1].split(">=");
            if (exp.size() != 2) abort();//如果不能分成两个部分，报错
            result->set(">=", tar, buildExp(exp[0].split(" ")), buildExp(exp[1].split(" ")));
        }
        else if (list[1].contains("<")) {
            QStringList exp = list[1].split("<");
            if (exp.size() != 2) abort();//如果不能分成两个部分，报错
            result->set("<", tar, buildExp(exp[0].split(" ")), buildExp(exp[1].split(" ")));
        }
        else if (list[1].contains("<=")) {
            QStringList exp = list[1].split("<=");
            if (exp.size() != 2) abort();//如果不能分成两个部分，报错
            result->set("<=", tar, buildExp(exp[0].split(" ")), buildExp(exp[1].split(" ")));
        }
        else if (list[1].contains("==")) {
            QStringList exp = list[1].split("==");
            if (exp.size() != 2) abort();//如果不能分成两个部分，报错
            result->set("==", tar, buildExp(exp[0].split(" ")), buildExp(exp[1].split(" ")));
        }

        result->lineNum = lineNum;//记录行号；
        return result;
    }
    else if (strList[0] == "END") {
        EndStmt* result = new EndStmt;
        result->lineNum = lineNum;//记录行号；
        return result;
    }
    else abort();
}
void Program::clear() {
    program.clear();
}
