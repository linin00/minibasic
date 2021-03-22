#include "minibasic.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);//处理命令行参数
    minibasic w;//申请对象
    w.show();//显示窗口
    return a.exec();//移交程序控制权
}
