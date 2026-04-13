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

    setFixedSize(500, 610);
    srand(static_cast<unsigned>(time(0)));
    inMainMenu = true;

    titleLabel = new QLabel("Tetris+", this);
    titleLabel->setGeometry(0, 100, 500, 100);
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet("color: cyan; font-size: 65px; font-weight: bold; font-family: Arial;");

    endlessButton = new QPushButton("Endless Mode", this);
    endlessButton->setGeometry(150, 280, 200, 50);
    endlessButton->setStyleSheet(
        "QPushButton { background-color: #333; color: white; font-size: 20px; font-weight: bold; border-radius: 10px; }"
        "QPushButton:hover { background-color: #555; }" 
    );

    timedButton = new QPushButton("Time Trial (Soon)", this);
    timedButton->setGeometry(150, 350, 200, 50);
    timedButton->setStyleSheet("QPushButton { background-color: #222; color: gray; font-size: 16px; font-weight: bold; border-radius: 10px; }");
    timedButton->setEnabled(false);

    connect(endlessButton, &QPushButton::clicked, this, [=]() {
        inMainMenu = false;          
        titleLabel->hide();           
        endlessButton->hide();
        timedButton->hide();

        engine.reset();               
        this->setFocus();             
        });

	QTimer* timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, [=]() {
        engine.update(50);
        update();
        });
    timer->start(50);
}

TetrisPlusV2::~TetrisPlusV2() {}


void TetrisPlusV2::keyPressEvent(QKeyEvent* event) {
    if (inMainMenu) return;

    if (engine.isGameOver()) {
        if (event->key() == Qt::Key_R) {
            engine.reset();
            update();
        }
        return;
    }
    switch (event->key()) {
    case Qt::Key_Left:  engine.moveLeft();          break;
    case Qt::Key_Right: engine.moveRight();         break;
    case Qt::Key_Down:  engine.moveDown();          break;
    case Qt::Key_Up:    engine.rotateActiveBlock(); break;
    case Qt::Key_Space: engine.hardDrop();          break;
    default: QMainWindow::keyPressEvent(event);     break;
    }
    update();
}

void TetrisPlusV2::paintEvent(QPaintEvent*) {
    QPainter painter(this);

    if (inMainMenu) {
        painter.fillRect(0, 0, width(), height(), QColor(20, 20, 20));
        return;
    }
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

    int sidePanelX = 320;
    int sidePanelWidth = 160;

    painter.setPen(Qt::white);
    QFont uiFont("Arial", 16, QFont::Bold);
    painter.setFont(uiFont);

    painter.drawText(QRect(sidePanelX, 30, sidePanelWidth, 30), Qt::AlignCenter, "Score");
    painter.drawText(QRect(sidePanelX, 60, sidePanelWidth, 30), Qt::AlignCenter, QString::number(engine.getScore()));

    painter.drawText(QRect(sidePanelX, 95, sidePanelWidth, 30), Qt::AlignCenter, "Level");
    painter.drawText(QRect(sidePanelX, 125, sidePanelWidth, 30), Qt::AlignCenter, QString::number(engine.getLevel()));

    painter.drawText(QRect(sidePanelX, 160, sidePanelWidth, 30), Qt::AlignCenter, "Next");

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
        case 5: tempBlock = new ZBlock(0, 0); break;
        case 6: tempBlock = new SBlock(0, 0); break;
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

            int minX = 4, maxX = 0;
            for (auto& cell : tempBlock->getCells()) {
                if (cell.first < minX) minX = cell.first;
                if (cell.first > maxX) maxX = cell.first;
            }
            int blockPixelWidth = (maxX - minX + 1) * cellSize;

            int drawOffsetX = sidePanelX + ((sidePanelWidth - blockPixelWidth) / 2) - (minX * cellSize);
            // Move the preview images further down the side panel
            int drawOffsetY = 200 + (i * 90);

            for (auto& cell : tempBlock->getCells()) {
                painter.fillRect(drawOffsetX + (cell.first * cellSize), drawOffsetY + (cell.second * cellSize), cellSize - 1, cellSize - 1, qtColor);
            }
            delete tempBlock;
        }
    }

    if (engine.isGameOver()) {
        painter.fillRect(0, 0, width(), height(), QColor(0, 0, 0, 180));

        painter.setPen(Qt::red);
        QFont bigFont("Arial", 30, QFont::Bold);
        painter.setFont(bigFont);
        painter.drawText(QRect(0, height() / 2 - 60, width(), 50), Qt::AlignCenter, "Game Over");

		painter.setPen(Qt::white);
        QFont smallFont("Arial", 16, QFont::Bold);
        painter.setFont(smallFont);
		painter.drawText(QRect(0, height() / 2 + 10, width(), 50), Qt::AlignCenter, "Press R to Restart");

    }
}






// main menu appearance/buttons