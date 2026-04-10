#include "TetrisPlusV2.h"
#include "GameEngine.h"
#include "blocks.h"
#include <qpainter.h>
#include <QKeyEvent>
#include <cstdlib>
#include <ctime>

TetrisPlusV2::TetrisPlusV2(QWidget* parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    srand(static_cast<unsigned>(time(0)));  
    engine.SpawnNextBlock();                
}

TetrisPlusV2::~TetrisPlusV2() {}


void TetrisPlusV2::keyPressEvent(QKeyEvent* event) {
    switch (event->key()) {
    case Qt::Key_Left:  engine.moveLeft();          break;
    case Qt::Key_Right: engine.moveRight();         break;
    case Qt::Key_Down:  engine.moveDown();          break;
    case Qt::Key_Up:    engine.rotateActiveBlock(); break;
    default: QMainWindow::keyPressEvent(event);     break;
    }
    update();
}

void TetrisPlusV2::paintEvent(QPaintEvent*) {
    QPainter painter(this);
    int cellSize = 30;

    auto grid = engine.getGrid();

    for (int row = 0; row < 20; row++) {
        for (int col = 0; col < 10; col++) {
            if (grid[row][col] == 0) {
                painter.fillRect(col * cellSize, row * cellSize,
                    cellSize - 1, cellSize - 1,
                    QColor(30, 30, 30));
            }
            else {
                painter.fillRect(col * cellSize, row * cellSize,
                    cellSize - 1, cellSize - 1,
                    Qt::cyan);
            }
        }
    }
}

// Paint blocks






// main menu appearance/buttons