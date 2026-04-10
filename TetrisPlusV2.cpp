#include "TetrisPlusV2.h"
#include "GameEngine.h"
#include "blocks.h"
#include <qpainter.h>
#include <QKeyEvent>
#include <cstdlib>
#include <ctime>
#include <QTimer>

TetrisPlusV2::TetrisPlusV2(QWidget* parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);

    centralWidget()->setAttribute(Qt::WA_TransparentForMouseEvents);
    centralWidget()->setStyleSheet("background:transparent;");

    setFixedSize(450, 610);
    srand(static_cast<unsigned>(time(0)));  
    engine.SpawnNextBlock();

	QTimer* timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, [=]() {
        engine.update(50);
        update();
        });
    timer->start(50);
}

TetrisPlusV2::~TetrisPlusV2() {}


void TetrisPlusV2::keyPressEvent(QKeyEvent* event) {
    if (engine.isGameOver()) return;
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
                    Qt::white);
            }
            else {
                int colorId = grid[row][col] - 1;
                QColor qtColor;

                switch (colorId) {
                case static_cast<int>(TetrisColor::Yellow): qtColor = Qt::yellow; break;
                case static_cast<int>(TetrisColor::Purple): qtColor = Qt::magenta; break;
                case static_cast<int>(TetrisColor::Green): qtColor = Qt::green; break;
                case static_cast<int>(TetrisColor::Red): qtColor = Qt::red; break;
                case static_cast<int>(TetrisColor::Blue): qtColor = Qt::blue; break;
                case static_cast<int>(TetrisColor::Orange): qtColor = QColor(255, 165, 0); break; // Qt doesn't have a predefined orange color, so we create one using RGB values
                case static_cast<int>(TetrisColor::Cyan): qtColor = Qt::cyan; break;
                default: qtColor = Qt::darkGray; break;
                }

                painter.fillRect(col * cellSize, row * cellSize, cellSize - 1, cellSize - 1, qtColor);
            }
        }
    }
    // Paint blocks
    Block* active = engine.getActiveBlock();
    if (active != nullptr) {
        QColor qtColor;
        switch (active->getColor()) {
        case TetrisColor::Yellow: qtColor = Qt::yellow; break;
        case TetrisColor::Purple: qtColor = Qt::magenta; break;
        case TetrisColor::Green: qtColor = Qt::green; break;
        case TetrisColor::Red: qtColor = Qt::red; break;
        case TetrisColor::Blue: qtColor = Qt::blue; break;
        case TetrisColor::Orange: qtColor = QColor(255, 165, 0); break; // Qt doesn't have a predefined orange color, so we create one using RGB values
        case TetrisColor::Cyan: qtColor = Qt::cyan; break;

        }

        int xOffset = active->getX();
        int yOffset = active->getY();

        for (auto& cell : active->getCells()) {
            int drawX = xOffset + cell.first;
            int drawY = yOffset + cell.second;

            painter.fillRect(drawX * cellSize, drawY * cellSize, cellSize - 1, cellSize - 1, qtColor);
        }
    }
    painter.setPen(Qt::white);
    QFont uiFont("Arial", 16, QFont::Bold);
    painter.setFont(uiFont);
    painter.drawText(330, 40, "Next");

	auto nextPieces = engine.getNextPieces();
    for (int i = 0; i < nextPieces.size(); i++) {
		int type = nextPieces[i];

        Block* tempBlock = nullptr;
        switch (type) {
		case 0: tempBlock = new TBlock(0, 0); break;
        case 1: tempBlock = new OBlock(0, 0); break;
        case 2: tempBlock = new IBlock(0, 0); break;
        case 3: tempBlock = new LBlock(0, 0); break;
        case 4: tempBlock = new JBlock(0, 0); break;
        case 5: tempBlock = new SBlock(0, 0); break;
        case 6: tempBlock = new ZBlock(0, 0); break;
        }

        if (tempBlock) {
            QColor qtColor;
            switch (tempBlock->getColor()) {
			case TetrisColor::Yellow: qtColor = Qt::yellow; break;
			case TetrisColor::Purple: qtColor = Qt::magenta; break;
			case TetrisColor::Green: qtColor = Qt::green; break;
			case TetrisColor::Red: qtColor = Qt::red; break;
			case TetrisColor::Blue: qtColor = Qt::blue; break;
			case TetrisColor::Orange: qtColor = QColor(255, 165, 0); break; // Qt doesn't have a predefined orange color, so we create one using RGB values
			case TetrisColor::Cyan: qtColor = Qt::cyan; break;
            }

            int offsetX = 330;
            int offsetY = 80 + (i * 120);

            for (auto& cell : tempBlock->getCells()) {
                painter.fillRect(offsetX + (cell.first * cellSize), offsetY + (cell.second * cellSize), cellSize - 1, cellSize - 1, qtColor);
            }
            delete tempBlock;
        }
    }

    if (engine.isGameOver()) {
        painter.fillRect(0, 0, width(), height(), QColor(0, 0, 0, 180));
        QFont font("Arial", 30, QFont::Bold);
        painter.setFont(font);
        painter.drawText(rect(), Qt::AlignCenter, "Game Over");
    }
}






// main menu appearance/buttons