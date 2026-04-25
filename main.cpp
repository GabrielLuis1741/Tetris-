#include <QApplication>
#include "TetrisPlusV2.h"

int main(int argc, char* argv[]) {
    QApplication a(argc, argv); // constructs the Qt app object

    a.setStyleSheet("QMainWindow { background-color: #1e1e1e; }" //This is a stylesheet for the whole app, sets default colors, etc
        "QLabel { color: #ffffff; }" //Labels are white by default 
        "QPushButton { background-color: #333333; color: white; border: 1px solid #555; padding: 5px; }"); //Default button look

    TetrisPlusV2 w; //creates an object, initializing the UI through its constructor
    w.show();    //Makes the window visible
    return a.exec();     //initializes Qt loop
}