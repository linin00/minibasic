#include "statement.h"
bool isNumber(QString input) { //判断字符串是否是纯数字（非负浮点数）
    return (input.contains(QRegularExpression("^-?\\d+(\\.\\d+)?$"))? true : false);
}
