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
    //if (inputList.size() == 0) abort();
    QStack<int> opOder;//优先级：加减：1；乘除：2；幂运算：3；左括号：0；右括号：4；
    QStack<expression*> stack;//符号栈
    QStack<QString> OP;//运算符栈，什么鬼乱七八糟
    opOder.push(0);
    /*开始build*/
    expression* result = nullptr;
    while (inputList.size()) {
        if (inputList.first() == "(") {
            OP.push(inputList.first());
            opOder.push(0);
            inputList.removeFirst();
        }
        else if (inputList.first() == "+") {
            if (opOder.top() < 1) {//如果前一个运算符的优先级低于 +
                OP.push(inputList.first());//将+加到栈中
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
        }
        else if (inputList.first() == "-") {
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
        else if (inputList.first() == "*") {
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
        else if (inputList.first() == "/") {
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
        else if (inputList.first() == "**") {
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
        else if (inputList.first() == ")") {//如果即将压入的符号为右括号，避免类似1 + ( n ) * 3的情况发生错误
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
            /*如果temp不是常数,那就是标识符*/
            int size = identifier.size();
            if (size == 0) abort();
            for (int i = 0; i < size; i++) {
                if (temp == identifier[i]->root) {//找到了相同的标识符
                    stack.push(identifier[i]);
                    inputList.removeFirst();
                    break;
                }
                else if (i == size - 1) { //不存在这个标识符，程序报错
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
    QStringList strList = input.split("\n");//以回车分割字符串，一行就是条指令
    int num = strList.size();
    statement* temp;
    for (int i = 0; i < num; i++) {//逐行生成语法树并插入语法树向量
        if (strList[i] == "") continue;//空行跳过
        temp = build(strList[i]);//生成
        int size = program.size();
        if (size == 0) {//插入语法树
            program.push_back(temp);
            return;
        }
        for (int j = 0; j < size; j++) {//插入语法树，如果顺序发生变化，有可能出现变量未声明就使用的情况，暂不考虑
            if (temp->lineNum > program[j]->lineNum) {
                if (j == size - 1) {
                    program.push_back(temp);
                    break;
                }
                continue;
            }
            else if (temp->lineNum < program[j]->lineNum) {
                program.insert(j, temp);
                break;
            }
            else if (temp->lineNum == program[j]->lineNum) {//命令替换，需要将这条命令声明的对象一并清除，太难了太难了，先不实现这个
                //statement* temp2 = program[j];//指令指针
                program[j] = temp;//替换
                break;
            }
        }
    }
    buildtree();
    run();
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
        strList.removeFirst();
        QString temp = strList.join(" ");//将REM后面的字符串记录到temp中
        result->setLeft(temp);//设置节点中的left节点
        result->lineNum = lineNum;//记录行号；
        return result;
    }
    else if (strList[0] == "LET") {//赋值语句
        if (strList[2] != "=") abort();
        LetStmt* result = new LetStmt;
        IdentifierExp* iden = nullptr;//处理左节点
        int size = identifier.size();
        for (int i = 0; i < size; i++) {
            if (identifier[i]->root == strList[1]) {//如果标识符已存在
                iden = identifier[i];//取其指针
                break;
            }
            if (i == size - 1) {//如果不存在，申请新的标识符，并存入向量中
                iden = new IdentifierExp;
                iden->setRoot(strList[1]);
                identifier.push_back(iden);//存入向量中
            }
        }
        if (size == 0) {
            iden = new IdentifierExp;
            iden->setRoot(strList[1]);
            identifier.push_back(iden);//存入向量中
        }
        result->setLeft(iden);//记录赋值的对象

        expression* right = nullptr;
        strList.removeFirst();//删除LET
        strList.removeFirst();//删除标识符名称
        strList.removeFirst();//删除=

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
                identifier.push_back(temp);
            }
        }

        if (size == 0) {
            IdentifierExp* temp = new IdentifierExp;
            temp->root = strList[1];
            result->setLeft(temp);
            identifier.push_back(temp);
        }

        result->lineNum = lineNum;//记录行号；
        return result;
    }
    else if (strList[0] == "PRINT") {
        PrintStmt* result = new PrintStmt;
        if (strList.size() != 2) abort();
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
    TREE.clear();
    RESULT.clear();
    input.clear();
    identifier.clear();
}

void Program::buildtree() {
    int size = program.size();
    for (int i = 0; i < size; i++) {//逐个处理指令树后合并
        TREE = TREE + QString::number(program[i]->lineNum);//行号
        TREE = TREE + " ";
        TREE = TREE + program[i]->root;////////
        TREE = TREE + "\n";
        /*子节点*/
        int level = 1;
        TREE = TREE + buildtree(level, program[i]->Left());
        TREE = TREE + buildtree(level, program[i]->Right());
    }
}

QString Program::buildtree(int level, expression* exp) {
    if (exp == nullptr) return "";
    QString result;
    for (int i = 0; i < level; i++) {
        result = result + "     ";
    }
    result = result + exp->show();
    result = result + "\n";

    level++;
    result = result + buildtree(level, exp->left);
    result = result + buildtree(level, exp->right);

    return result;
}
void Program::run() {}
