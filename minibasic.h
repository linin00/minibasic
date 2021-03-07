#ifndef MINIBASIC_H
#define MINIBASIC_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class minibasic; }
QT_END_NAMESPACE

class minibasic : public QMainWindow
{
    Q_OBJECT

public:
    minibasic(QWidget *parent = nullptr);
    ~minibasic();

private:
    Ui::minibasic *ui;
};
#endif // MINIBASIC_H
