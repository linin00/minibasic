#ifndef EXECUTIONTREE_H
#define EXECUTIONTREE_H
#include <QString>
#define EXCUTE_STAGE false;
#define INPUT_STAGE true;
class Program {
private:
    void build();
    bool state;

public:
    QString input;//输入内容
    QString program;//程序
    Program() {//初始化
      state = INPUT_STAGE;
    }
    ~Program() {}//析构

    void read_from_input(QString inputStr);//从输入中读取内容
    void read_from_files(QString inputStr);//从文件中读取内容
    void clear();//清除
};
#endif // EXECUTIONTREE_H
