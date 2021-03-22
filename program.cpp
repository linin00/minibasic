#include "program.h"

void Program::read_from_input(QString inputStr) {
    program = inputStr;
    build();
}

void Program::read_from_files(QString inputStr) {
    program = inputStr;
    build();
}
void Program::build() {

}
void Program::clear() {
    program.clear();
}
