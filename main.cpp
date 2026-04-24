#include <QApplication>
#include "TetrisPlusV2.h"

int main(int argc, char* argv[]) {
    QApplication a(argc, argv);

    a.setStyleSheet("QMainWindow { background-color: #1e1e1e; }"
        "QLabel { color: #ffffff; }"
        "QPushButton { background-color: #333333; color: white; border: 1px solid #555; padding: 5px; }");

    TetrisPlusV2 w;
    w.show();
    return a.exec();
}