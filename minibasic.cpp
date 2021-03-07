#include "minibasic.h"
#include "ui_minibasic.h"

minibasic::minibasic(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::minibasic)
{
    ui->setupUi(this);
}

minibasic::~minibasic()
{
    delete ui;
}

