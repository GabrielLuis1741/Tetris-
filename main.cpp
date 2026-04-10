#include <QApplication>
#include "TetrisPlusV2.h"

int main(int argc, char* argv[]) {
    QApplication a(argc, argv);
    TetrisPlusV2 w;
    w.show();
    return a.exec();
}