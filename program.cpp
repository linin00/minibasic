#include "program.h"
#include <QStringList>
#include <iostream>
#include <QStack>
#include <QRegularExpression>
#include <QDebug>
#include <QTextCursor>
#include <QList>
#include <QPair>
#include <QColor>
bool isNumber(QString input) { //判断字符串是否是纯数字（非负浮点数）
    return (input.contains(QRegularExpression("^\\d+(\\.\\d+)?$"))? true : false);
}
void replace(QString& str) {//补空格
    if (str == "" || str == nullptr) return;
    //运算符替换
    str.replace("+", " + ");
    //str.replace("-", " - ");
    str.replace("*", " * ");
    str.replace("/", " / ");
    str.replace(" *  * ", " ** ");

    //括号替换
    str.replace(")", " ) ");
    str.replace("(", " ( ");

    //逻辑运算符替换
    str.replace("=", " = ");
    str.replace(">", " > ");
    str.replace("<", " < ");
    str.replace(" >  = ", " >= ");
    str.replace(" <  = ", " <= ");
}
void Program::read_from_input(QString inputStr) {
    numOfError = 0;
    highlight_pos_err.clear();

    int p = 0;
    if (inputStr == nullptr || inputStr == "LIST")  return;//空行不处理
    replace(inputStr);
    inputStr.replace(QRegularExpression("[\\s]+"), " ");//合并空格
    inputStr.remove(QRegularExpression("^ +\\s*"));//去头
    inputStr.remove(QRegularExpression("\\s* +$"));//去尾
    QStringList temp = inputStr.split(" ");//空格分割

    int length = code.size();
    if (isNumber(temp[0]) && temp.size() == 1){//如果只有行号，删除指令
        if (length == 0) {
            QMessageBox::warning(NULL, "Warning!", inputStr + "\n删除失败\n程序中没有代码");
            return;
        }
        double del = temp[0].toDouble();
        for (int i = 0; i < length; i++) {
            if (isNumber(code[i].split(" ")[0]) && del == code[i].split(" ")[0].toDouble()) {//删除对应行
                code.erase(code.begin()+i);
                break;
            }
            if (i == length - 1) {
                QMessageBox::warning(NULL, "Warning!", inputStr + "\n在程序中不存在该行代码");
                return;
            }
        }
    }

    else if (!isNumber(temp[0]) &&
             ((temp[0] == "PRINT") ||
             (temp[0] == "LET") ||
             (temp[0] == "INPUT"))
             ) {//如果没有行号，直接插入并运行

        code.push_back(inputStr);
        p = code.size() - 1;
        build();//构建
        if (error) {
            code.erase(code.begin() + p);
            error = false;
            return;
        }
        int line_old = line;
        line = program.size() -1;
        RUN();//运行,由于line已经设置为最后一行，所以只执行一句就停下来了
        line = line_old;
        code.erase(code.begin() + p);//从语法树中剔除
        return;
    }
    else if (!isNumber(temp[0]) &&
             !((temp[0] == "PRINT") ||
             (temp[0] == "LET") ||
             (temp[0] == "INPUT"))
             ) {//报错
        QMessageBox::warning(NULL, "Warning!", inputStr + "\n缺行号或指令不存在");
        return;
    }

    else if (length == 0) {//如果事先没有代码，直接插入
        code.push_back(inputStr);
        p = code.size() - 1;
    }
    else for (int i = length - 1; i >= 0; i--) {//插入,从后往回找，快一点
        QStringList l1 = code[i].split(" ");
        if (l1[0].toDouble() < temp[0].toDouble()) {//插在比他小的后面
            code.insert(code.begin()+i+1, inputStr);
            p = i + 1;
            break;
        }
        if (l1[0].toDouble() == temp[0].toDouble()) {//行号相同时，直接替换
            code[i] = inputStr;
            p = i;
            break;
        }
        if (i == 0) {//所有的行号比他大时，在头部插入
            code.insert(code.begin(), inputStr);
            p = 0;
            break;
        }
    }

    build();//构建
    if (error) {
        error = false;
        qDebug() << "发现错误\n";
    }
    length = code.size();
    input.clear();
    for (int i = 0; i < length; i++) {
        input = input + code[i] + "\n";
    }
}
void Program::read_from_files(QString Str) {
    file = Str.split("\n");
    Input -> setText(file[0]);
    file.pop_front();
    inputFF = true;
}
void Program::build() {//构建语句树，存入语句树向量
    program.clear();//构建前先清空可能已有的语法树
    identifier.clear();
    //构建语句树
    int num = code.size();
    statement* temp = nullptr;
    for (int i = 0; i < num; i++) {//逐行生成语法树并插入语法树向量
        temp = build(code[i]);//生成
        if (temp == nullptr) {//发生了错误
            numOfError++;//错误个数统计
            int pos = 0;
            for (int j = 0; j < i; j++) {
                pos+=code[j].size();
                pos++;
            }
            pos+=code[i].size();
            highlight_pos_err.append(pos);//记录错误位置
        }
        //插入语法树
        program.push_back(temp);
    }
}
statement* Program::build(QString inputStr) {//由一条语句生成语句树，并返回

    /*第一步，拆分语句项*/
    QStringList strList = inputStr.split(" ");//用空格划分

    /*第二步，行号处理*/
    int lineNum = -1;
    if (isNumber(strList[0])) {//如果是非负数，记录行号，否则行号记为-1；
        lineNum = strList[0].toDouble();
    }

    if (lineNum != -1) {//如果有行号
        strList.removeFirst();//删除字符串的行号
    }

    /*第三步，根据指令生成特定的语法树，太难了*/
    /*REM*/
    if (strList[0] == "REM") {//头节点
        RemStmt* result = new RemStmt;

        //设置left
        strList.removeFirst();
        QString temp = strList.join(" ");//将REM后面的字符串记录到temp中
        result->setLeft(temp);//设置节点中的left节点
        result->lineNum = lineNum;//记录行号；
        return result;
    }
    /*LET*/
    else if (strList[0] == "LET") {//赋值语句
        if (strList[2] != "=") {
            //QMessageBox::warning(NULL, "Warning!", inputStr + "\n赋值语句格式错误");
            error = true;
            return nullptr;
        }
        LetStmt* result = new LetStmt;

        //处理赋值目标
        IdentifierExp* iden = nullptr;//处理左节点

        if (strList[1].contains(QRegularExpression("^[\\-\\+]?\\d*[0-9](|.\\d*[0-9]|,\\d*[0-9])?$"))||
                strList[1].contains(QRegularExpression("[\\+\\-\\*\\/\\>\\<\\?\\.||,\\=]"))) {
            //QMessageBox::warning(NULL, "Warning!", inputStr + "\n赋值语句格式错误,不能给常数或特殊符号赋值");
            error = true;
            return nullptr;
        }

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

        /*处理右节点，有可能是组合式或常数*/
        expression* right = nullptr;
        strList.removeFirst();//删除LET
        strList.removeFirst();//删除标识符名称
        strList.removeFirst();//删除=

        right = buildExp(strList);
        if (right == nullptr) return nullptr;
        result->setRight(right);
        result->lineNum = lineNum;//记录行号；
        return result;

    }
    /*INPUT*/
    else if (strList[0] == "INPUT") {
        InputStmt* result = new InputStmt;
        if (strList.size() != 2) {
            //QMessageBox::warning(NULL, "Warning!", inputStr + "\n输入语句格式错误");
            error = true;
            return nullptr;
        }

        //设置left
        if (strList[1].contains(QRegularExpression("^[\\-\\+]?\\d*[0-9](|.\\d*[0-9]|,\\d*[0-9])?$"))||
                strList[1].contains(QRegularExpression("[\\+\\-\\*\\/\\>\\<\\?\\.||,\\=]"))) {
        //QMessageBox::warning(NULL, "Warning!", inputStr + "\n输入语句格式错误,不能给常数或特殊符号赋值");
        error = true;
        return nullptr;
    }
        int size = identifier.size();
        for (int i = 0; i < size; i++) {
            if (strList[1] == identifier[i]->root) {//存在标识符
                result->setLeft(identifier[i]);
                break;
            }
            if (i == size - 1) {//不存在
                IdentifierExp* temp = new IdentifierExp;
                temp->root = strList[1];
                result->setLeft(temp);
                identifier.push_back(temp);
            }
        }

        if (size == 0) {//不存在
            IdentifierExp* temp = new IdentifierExp;
            temp->root = strList[1];
            result->setLeft(temp);
            identifier.push_back(temp);
        }

        result->lineNum = lineNum;//记录行号；
        return result;
    }
    /*PRINT*/
    else if (strList[0] == "PRINT") {
        PrintStmt* result = new PrintStmt;
        if (strList.size() < 2) {
            //QMessageBox::warning(NULL, "Warning!", inputStr + "\n打印语句格式错误");
            error = true;
            return nullptr;
        }
        /*设置左节点*/
        strList.removeFirst();//删除PRINT
        expression* temp = buildExp(strList);
        if (temp == nullptr) return nullptr;
        result->setLeft(temp);
        result->lineNum = lineNum;//记录行号；
        return result;
    }
    /*GOTO*/
    else if (strList[0] == "GOTO") {
        if (lineNum == -1) {
            //QMessageBox::warning(NULL, "Warning!", inputStr + "\n必须有行号");
            error = true;
            return nullptr;
        }
        GotoStmt* result = new GotoStmt;
        /*设置左节点*/
        strList.removeFirst();//删除GOTO
        expression* temp = buildExp(strList);
        if (temp == nullptr) return nullptr;
        result->setLeft(temp);
        result->lineNum = lineNum;//记录行号；
        return result;
    }
    /*IF*/
    else if (strList[0] == "IF") {
        if (lineNum == -1) {
            //QMessageBox::warning(NULL, "Warning!", inputStr + "\n必须有行号");
            error = true;
            return nullptr;
        }
        IfStmt* result = new IfStmt;

        strList.removeFirst();//删除IF
        QString temp = strList.join(" ");//先合并~~~
        QStringList list = temp.split(" THEN ");//再分割。。
        if (list.size() != 2) {//如果不能分成两部分，报错
            //QMessageBox::warning(NULL, "Warning!", inputStr + "\n条件语句格式错误");
            error = true;
            return nullptr;
        }
        expression* tar = buildExp(list[1].split(" "));//生成目标表达式
        //构建条件表达式
        if (list[0].contains(" > ")) {//判断是否含有逻辑运算符
            QStringList exp = list[0].split(" > ");
            if (exp.size() != 2) {//如果不能分成两个部分，报错
                //QMessageBox::warning(NULL, "Warning!", list[0] + "\n条件表达式格式错误");
                error = true;
                return nullptr;
            }
            result->set(">", tar, buildExp(exp[0].split(" ")), buildExp(exp[1].split(" ")));
        }
        else if (list[0].contains(" >= ")) {
            QStringList exp = list[0].split(" >= ");
            if (exp.size() != 2) {//如果不能分成两个部分，报错
                //QMessageBox::warning(NULL, "Warning!", list[0] + "\n条件表达式格式错误");
                error = true;
                return nullptr;
            }
            result->set(">=", tar, buildExp(exp[0].split(" ")), buildExp(exp[1].split(" ")));
        }
        else if (list[0].contains(" < ")) {
            QStringList exp = list[0].split(" < ");
            if (exp.size() != 2) {//如果不能分成两个部分，报错
                //QMessageBox::warning(NULL, "Warning!", list[0] + "\n条件表达式格式错误");
                error = true;
                return nullptr;
            }
            result->set("<", tar, buildExp(exp[0].split(" ")), buildExp(exp[1].split(" ")));
        }
        else if (list[0].contains(" <= ")) {
            QStringList exp = list[0].split(" <= ");
            if (exp.size() != 2) {//如果不能分成两个部分，报错
                //QMessageBox::warning(NULL, "Warning!", list[0] + "\n条件表达式格式错误");
                error = true;
                return nullptr;
            }
            result->set("<=", tar, buildExp(exp[0].split(" ")), buildExp(exp[1].split(" ")));
        }
        else if (list[0].contains(" = ")) {
            QStringList exp = list[0].split(" = ");
            if (exp.size() != 2) {//如果不能分成两个部分，报错
                //QMessageBox::warning(NULL, "Warning!", list[0] + "\n条件表达式格式错误");
                error = true;
                return nullptr;
            }
            result->set("=", tar, buildExp(exp[0].split(" ")), buildExp(exp[1].split(" ")));
        }
        else if (list[0].contains(" != ")) {
            QStringList exp = list[0].split(" != ");
            if (exp.size() != 2) {//如果不能分成两个部分，报错
                //QMessageBox::warning(NULL, "Warning!", list[0] + "\n条件表达式格式错误");
                error = true;
                return nullptr;
            }
            result->set("!=", tar, buildExp(exp[0].split(" ")), buildExp(exp[1].split(" ")));
        }
        else {
            //QMessageBox::warning(NULL, "Warning!", list[0] + "\n条件表达式格式错误");
            error = true;
            return nullptr;
        };
        if (result -> Left() == nullptr || result -> Right() == nullptr ) return nullptr;
        result->lineNum = lineNum;//记录行号；
        return result;
    }
    /*END*/
    else if (strList[0] == "END") {
        if (lineNum == -1) {
            //QMessageBox::warning(NULL, "Warning!", inputStr + "\n必须有行号");
            error = true;
            return nullptr;
        }
        EndStmt* result = new EndStmt;
        result->lineNum = lineNum;//记录行号；
        return result;
    }
    /*不存在*/
    else {//不存在的语句
        //QMessageBox::warning(NULL, "Warning!", inputStr + "\n不存在的语句");
        error = true;
        return nullptr;
    }
}
expression* Program::buildExp(QStringList inputList) {//生成表达式树

    QStack<int> opOder;//优先级：加减：1；乘除：2；幂运算：3；左括号：0；右括号：4；
    QStack<expression*> stack;//符号栈
    QStack<QString> OP;//运算符栈，什么鬼乱七八糟

    opOder.push(0);//先压一个最低优先级，避免特殊情况出现错误
    QStringList inputList_cp = inputList;


    /*开始build*/
    expression* result = nullptr;

    while (inputList.size()) {//直到表达式元素全部读取完毕

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
        else if (inputList.first() == "**") {//右结合，需特殊处理
            if (opOder.top() < 3) {//如果前一个运算符的优先级低于 **
                OP.push(inputList.first());
                //opOder.push(3);
                opOder.push(1);
                inputList.removeFirst();
                continue;
            }
            /*否则,但不可能*/
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

            //如果temp不是常数,那就是标识符
            int size = identifier.size();

            if (size == 0) { //不存在这个标识符，先放一个在列表里
                IdentifierExp* iden = new IdentifierExp;
                iden->root = temp;
                identifier.push_back(iden);
                stack.push(iden);
                inputList.removeFirst();
                qDebug() << "没找到标识符，但新建了一个";
            }
            else for (int i = 0; i < size; i++) {
                if (temp == identifier[i]->show()) {//找到了相同的标识符
                    stack.push(identifier[i]);
                    inputList.removeFirst();
                    qDebug() << "成功找到标识符";
                    break;
                }
                else if (i == size - 1) { //不存在这个标识符，先放一个在列表
                    IdentifierExp* iden = new IdentifierExp;
                    iden->root = temp;
                    identifier.push_back(iden);
                    stack.push(iden);
                    inputList.removeFirst();
                    qDebug() << "没找到标识符，但新建了一个";
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
    if (opOder.size() != 1) {
        //QMessageBox::warning(NULL, "Warning!", inputList_cp.join(" ") + "\n表达式错误");
        error = true;
        return nullptr;
    }
    return result;
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
QString Program::buildtree(int i) {//构建单行可打印语句树，i：第i行语句
    int size = program.size();
    if (i > size - 1) abort();
    QString result;
    //处理指令树后返回
    if (program[i]->lineNum != -1)
        result = result + QString::number(program[i]->lineNum);//行号
    result = result + " ";
    result = result + program[i]->root;////////
    result = result + "\n";
    if (program[i]->root=="IF THEN") {
        result = result + "     " + program[i]->OP() + "\n";
    }
    /*子节点*/
    result = result + buildtree(1, program[i]->Left());
    result = result + buildtree(1, program[i]->Right());

    if (program[i]->root=="IF THEN") {
        result = result + buildtree(1, program[i]->TAR());
    }
    return result;
}
void Program::showIdent(){
    int t = identifier.size();
    QString iden;
    for (int i = 0; i < t; i++) {
        if (identifier[i]->DONE())
            iden = iden + identifier[i]->root + ": " + identifier[i]->type + " = " + QString::number(*identifier[i]->setvalue()) + "\n";
    }
    Ident->setText(iden);
}
void Program::run() {
    if (debug) {
        highlight_pos_now = -1;
        highlight();
        debug = false;
    }
    if (numOfError != 0) {//如果有错，不运行
        if (!debug){
            Load->setEnabled(true);
            Clear->setEnabled(true);
            Input->clear();//把输入窗口的东东清掉
            state = 1;//归位是个好习惯
            line = 0;
            showIdent();
        }
        return;
    }
    RESULT.clear();//运行前清空运算结果
    int size = program.size();
    if (size == 0) return;//如果语句树为空，直接返回

    for (; line < size ; line++) {//先打印到当前执行的命令
        if (program[line]->lineNum != -1){//如果没有行号，不增加语法树
            TREE = TREE + buildtree(line);
        }
        Tree->setText(TREE);

        statement* sta = program[line];

        if (sta->root == "REM") {
            continue;//不做任何事
        }
        else if (sta->root == "LET =") {//赋值

            sta->Left()->turn_on();//声明变量

            *sta->Left()->setvalue() = *sta->Right()->value();
            qDebug() << sta->Left();
        }
        else if (sta->root == "INPUT") {//输入，从输入框获取信息
            sta->Left()->turn_on();//声明变量
            state = false;
            Input->setText("? ");
            line++;
            idenNow = sta->Left();
            return;
        }
        else if (sta->root == "PRINT") {
            RESULT = RESULT + QString::number(*sta->Left()->value()) + "\n";//将输出内容存入RESULT
            qDebug() << sta->Left();
            Result->setText(RESULT);//打印
        }
        else if (sta->root == "IF THEN") {
            double L = *sta->TAR()->value();
            if (L - int(L) != 0) {
                QMessageBox::warning(NULL, "Warning!", "THEN " + QString::number(L) + "\n不存在目标行号");
                return;//停止运行
            }

            //判断条件是否成立
            bool jmp = false;
            if (sta->OP() == ">"){
                if (*sta->Left()->value() > *sta->Right()->value()) jmp = true;
            }
            else if (sta->OP() == ">="){
                if (*sta->Left()->value() >= *sta->Right()->value()) jmp = true;
            }
            else if (sta->OP() == "<"){
                if (*sta->Left()->value() < *sta->Right()->value()) jmp = true;
            }
            else if (sta->OP() == "<="){
                if (*sta->Left()->value() <= *sta->Right()->value()) jmp = true;
            }
            else if (sta->OP() == "="){
                if (*sta->Left()->value() == *sta->Right()->value()) jmp = true;
            }
            else if (sta->OP() == "!="){
                if (*sta->Left()->value() != *sta->Right()->value()) jmp = true;
            }


            int _size = program.size();//设置执行行号
            if (!jmp) continue;//不跳转
            for (int i = 0; i < _size; i++) {
                if (program[i]->lineNum == L) {
                    line = i - 1;//减一，因为等下会加一
                    break;
                }
                if (i == _size - 1) {
                    QMessageBox::warning(NULL, "Warning!", "THEN " + QString::number(L) + "\n不存在目标行号");
                    return;
                }
            }

        }
        else if (sta->root == "GOTO") {
            int L = *sta->Left()->value();//读取goto目标

            int _size = program.size();//设置执行行号
            for (int i = 0; i < _size; i++) {
                if (program[i]->lineNum == L) {
                    line = i - 1;//减一，因为等下会加一
                    break;
                }
                if (i == _size - 1) {
                    QMessageBox::warning(NULL, "Warning!", "GOTO " + QString::number(L) + "\n不存在目标行号");
                    return;
                }
            }
        }
        else if (sta->root == "END") {
            break;
        }
    }

    Input->clear();//把输入窗口的东东清掉
    state = 1;//归位是个好习惯
    line = 0;
    showIdent();
    if (!debug){
        Load->setEnabled(true);
        Clear->setEnabled(true);
    }
    identityoff();
}
void Program::Debug() {
    int size = program.size();
    if (size == 0) return;//如果语句树为空，直接返回
    highlight_pos_now = 0;
    for (int j = 0; j < line; j++) {
        highlight_pos_now += code[j].size() + 1;
    }
    highlight_pos_now += code[line].size();
    highlight();
    //先打印到当前执行的命令
    if (program[line]->lineNum != -1){//如果没有行号，不增加语法树
        //TREE = TREE + buildtree(line);
        TREE = buildtree(line);
    }
    Tree->setText(TREE);

    statement* sta = program[line];

    if (sta->root == "REM") {
        //不做任何事
    }
    else if (sta->root == "LET =") {//赋值

        sta->Left()->turn_on();//声明变量

        *sta->Left()->setvalue() = *sta->Right()->value();
        qDebug() << sta->Left();
    }
    else if (sta->root == "INPUT") {//输入，从输入框获取信息
        sta->Left()->turn_on();//声明变量
        state = false;
        Input->setText("? ");
        line++;
        idenNow = sta->Left();
        return;
    }
    else if (sta->root == "PRINT") {

        RESULT = RESULT + QString::number(*sta->Left()->value()) + "\n";//将输出内容存入RESULT
        qDebug() << sta->Left();
        Result->setText(RESULT);//打印
    }
    else if (sta->root == "IF THEN") {
        double L = *sta->TAR()->value();
        if (L - int(L) != 0) {
            QMessageBox::warning(NULL, "Warning!", "THEN " + QString::number(L) + "\n不存在目标行号");
            return;//停止运行
        }

        //判断条件是否成立
        bool jmp = false;
        if (sta->OP() == ">"){
            if (*sta->Left()->value() > *sta->Right()->value()) jmp = true;
        }
        else if (sta->OP() == ">="){
            if (*sta->Left()->value() >= *sta->Right()->value()) jmp = true;
        }
        else if (sta->OP() == "<"){
            if (*sta->Left()->value() < *sta->Right()->value()) jmp = true;
        }
        else if (sta->OP() == "<="){
            if (*sta->Left()->value() <= *sta->Right()->value()) jmp = true;
        }
        else if (sta->OP() == "="){
            if (*sta->Left()->value() == *sta->Right()->value()) jmp = true;
        }
        else if (sta->OP() == "!="){
            if (*sta->Left()->value() != *sta->Right()->value()) jmp = true;
        }


        int _size = program.size();//设置执行行号
        if (jmp)//不跳转
        for (int i = 0; i < _size; i++) {
            if (program[i]->lineNum == L) {
                line = i - 1;//减一，因为等下会加一
                break;
            }
            if (i == _size - 1) {
                QMessageBox::warning(NULL, "Warning!", "THEN " + QString::number(L) + "\n不存在目标行号");
                return;
            }
        }

    }
    else if (sta->root == "GOTO") {
        int L = *sta->Left()->value();//读取goto目标

        int _size = program.size();//设置执行行号
        for (int i = 0; i < _size; i++) {
            if (program[i]->lineNum == L) {
                line = i - 1;
                break;
            }
            if (i == _size - 1) {
                QMessageBox::warning(NULL, "Warning!", "GOTO " + QString::number(L) + "\n不存在目标行号");
                return;
            }
        }
    }
    else if (sta->root == "END") {
        line = 0 -1;
    }
    line++;
    showIdent();
    if (line > size || line == 0) {
        state = 1;//归位是个好习惯
        line = 0;
    }
    Input->clear();//把输入窗口的东东清掉
}
void Program::RUN() {
    if (numOfError != 0) return;//如果有错，不运行
    if (program[line]->lineNum != -1){//如果没有行号，不增加语法树
        TREE = TREE + buildtree(line);
    }
    Tree->setText(TREE);

    statement* sta = program[line];
    if (sta->root == "LET =") {//赋值

        sta->Left()->turn_on();//声明变量
        *sta->Left()->setvalue() = *sta->Right()->value();
        qDebug() << sta->Left();
    }
    else if (sta->root == "INPUT") {//输入，从输入框获取信息
        sta->Left()->turn_on();//声明变量
        state = false;
        Input->setText("? ");
        line++;
        idenNow = sta->Left();
        return;
    }
    else if (sta->root == "PRINT") {
        RESULT = RESULT + QString::number(*sta->Left()->value()) + "\n";//将输出内容存入RESULT
        qDebug() << sta->Left();
        Result->setText(RESULT);//打印
    }
    Input->clear();//把输入窗口的东东清掉
    state = 1;//归位是个好习惯
    line = 0;
    showIdent();
    identityoff();
}
void Program::clear() {//完全初始化，但不更新窗口显示内容
    input.clear();//清空从输入窗口读取的内容
    code.clear();//清空从输入窗口读取的内容
    program.clear();//清空语句树向量
    identifier.clear();//清空变量储存区
    highlight_pos_err.clear();
    highlight_pos_now = -1;
    TREE.clear();//清空语句树打印串
    RESULT.clear();//清空结果打印串
    state = 1;
    inputFF = false;
    error = false;
    numOfError = 0;
    line = 0;//重置执行序号
    file.clear();
}
void Program::highlight() {
    QTextCursor cursor(Code->document());
    QList<QTextEdit::ExtraSelection> extras;
    QList<QPair<int, QColor>> highlights;

    for (auto &line : highlight_pos_err) {
        QPair<int, QColor> temp(line, QColor(255,100,100));
        highlights.append(temp);
    }
    if (highlight_pos_now != -1) {
        QPair<int, QColor> temp(highlight_pos_now, QColor(100,255,100));
        highlights.append(temp);
    }
    for (auto &line : highlights) {
        QTextEdit::ExtraSelection h;
        h.cursor = cursor;// 下面这些的功能，请大家自行查看文档
        h.cursor.setPosition(line.first);
        h.cursor.movePosition(QTextCursor::StartOfLine);
        h.cursor.movePosition(QTextCursor::EndOfLine);
        h.format.setProperty(QTextFormat::FullWidthSelection, true);
        h.format.setBackground(line.second);
        extras.append(h);
    }
    qDebug() << extras.size();
    Code->setExtraSelections(extras);
}
void Program::identityoff(){
    int size = identifier.size();
    for (int i = 0; i < size; i++) {
        identifier[i]->turn_off();
    }
}
