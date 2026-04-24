#include "TetrisPlusV2.h"
#include "GameEngine.h"
#include "blocks.h"
#include <qpainter.h>
#include <QKeyEvent>
#include <cstdlib>
#include <ctime>
#include <QTimer>
#include <algorithm>

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

    timedButton = new QPushButton("Time Trial", this);
    timedButton->setGeometry(150, 350, 200, 50);
    timedButton->setStyleSheet("QPushButton { background-color: #333; color: white; font-size: 20px; font-weight: bold; border-radius: 10px; }"
        "QPushButton:hover { background-color: #555; }"
    );


    connect(endlessButton, &QPushButton::clicked, this, [=]() {
        inMainMenu = false;          
        titleLabel->hide();           
        endlessButton->hide();
        timedButton->hide();

		engine.setMode(GameMode::Endless);
        engine.reset();               
        this->setFocus();             
        });

    // Load menu background from resources. The project's .qrc was changed — try several common paths
    const QString candidates[] = {
        ":/tetrisbg.jpg",
        ":/Tetris/tetrisbg.jpg",
        ":/images/tetrisbg.jpg",
        ":/assets/images/tetrisbg.jpg",
        ":/tetris/tetrisbg.jpg",
        ":/Tetris/assets/images/menu_bg.png",
        "tetrisbg.jpg",
        "assets/images/tetrisbg.jpg",
        "assets/images/menu_bg.png"
    };
    for (const QString &p : candidates) {
        if (!p.isEmpty()) {
            menuBackground = QPixmap(p);
            if (!menuBackground.isNull()) break;
        }
    }

    connect(timedButton, &QPushButton::clicked, this, [=]() {
        inMainMenu = false;
        titleLabel->hide();
        endlessButton->hide();
        timedButton->hide();

        engine.setMode(GameMode::TimeTrial);
        engine.reset();
        this->setFocus();
        });

    controlsButton = new QPushButton("Controls", this);
    controlsButton->setGeometry(150, 420, 200, 50);
    controlsButton->setStyleSheet(
        "QPushButton { background-color: #333; color: white; font-size: 20px; font-weight: bold; border-radius: 10px; }"
        "QPushButton:hover { background-color: #555; }"
    );

    controlsLabel = new QLabel(
        "Left / Right Arrows - Move Block\n\n"
        "Down Arrow - Soft Drop\n\n"
        "Up Arrow - Rotate Block\n\n"
        "Spacebar - Hard Drop\n\n"
        "C - Hold Piece\n\n"
        "Esc - Pause Game", this);
    controlsLabel->setGeometry(50, 180, 400, 280);
    controlsLabel->setAlignment(Qt::AlignCenter);
    controlsLabel->setStyleSheet("color: white; font-size: 18px; font-weight: bold; background: rgba(0,0,0,180); border-radius: 10px;");
    controlsLabel->hide();

    backButton = new QPushButton("Back to Menu", this);
    backButton->setGeometry(150, 480, 200, 50);
    backButton->setStyleSheet(
        "QPushButton { background-color: #333; color: white; font-size: 20px; font-weight: bold; border-radius: 10px; }"
        "QPushButton:hover { background-color: #555; }"
    );
    backButton->hide();

    connect(controlsButton, &QPushButton::clicked, this, [=]() {
        endlessButton->hide();
        timedButton->hide();
        controlsButton->hide();

        controlsLabel->show();
        backButton->show();
        });

    connect(backButton, &QPushButton::clicked, this, [=]() {
        controlsLabel->hide();
        backButton->hide();

        endlessButton->show();
        timedButton->show();
        controlsButton->show();
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
        else if (event->key() == Qt::Key_M) {
            inMainMenu = true;
            titleLabel->show();      
            endlessButton->show();    
            timedButton->show(); 
            controlsButton->show();
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
    case Qt::Key_Escape: engine.pause();            break;
    case Qt::Key_C: engine.hold();                  break;
    default: QMainWindow::keyPressEvent(event);     break;
    }
    update();
}

void TetrisPlusV2::paintEvent(QPaintEvent*) {
    QPainter painter(this);

    if (inMainMenu) {
        if (!menuBackground.isNull()) {
            painter.drawPixmap(rect(), menuBackground);
            // dark grid
            painter.fillRect(0, 0, width(), height(), QColor(0, 0, 0, 120));
        }
        else {
            painter.fillRect(0, 0, width(), height(), QColor(20, 20, 20));
        }
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

        // Preview current block
        if (!engine.isGameOver() && !engine.isPaused()) {
            int gx = active->getX();
            int gy = active->getY();
            // find lowest y where the block can be placed
            while (engine.canPlace(active, gx, gy + 1)) {
                gy++;
            }
            
            if (gy != active->getY()) {
                QColor ghostColor = qtColor;
                ghostColor.setAlpha(90);
                for (auto& cell : active->getCells()) {
                    int drawX = gx + cell.first;
                    int drawY = gy + cell.second;
                    painter.fillRect(drawX * cellSize, drawY * cellSize, cellSize - 1, cellSize - 1, ghostColor);
                }
            }
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

    // Top: Score and Level
    int topY = 20;
    painter.drawText(QRect(sidePanelX, topY, sidePanelWidth, 30), Qt::AlignCenter, "Score");
    painter.drawText(QRect(sidePanelX, topY + 30, sidePanelWidth, 30), Qt::AlignCenter, QString::number(engine.getScore()));

    painter.drawText(QRect(sidePanelX, topY + 70, sidePanelWidth, 30), Qt::AlignCenter, "Level");
    painter.drawText(QRect(sidePanelX, topY + 100, sidePanelWidth, 30), Qt::AlignCenter, QString::number(engine.getLevel()));

    // Next previews
    int nextLabelY = topY + 140;
    painter.drawText(QRect(sidePanelX, nextLabelY, sidePanelWidth, 30), Qt::AlignCenter, "Next");

    int previewStartY = nextLabelY + 30;
    int previewGap = 80;

    auto nextPieces = engine.getNextPieces();
    int maxPreviews = 3; // show at most this many next-piece previews to avoid overlap
    int previewsToShow = std::min(static_cast<int>(nextPieces.size()), maxPreviews);
    for (int i = 0; i < previewsToShow; i++) {
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
            int drawOffsetY = previewStartY + (i * previewGap);

            for (auto& cell : tempBlock->getCells()) {
                painter.fillRect(drawOffsetX + (cell.first * cellSize), drawOffsetY + (cell.second * cellSize), cellSize - 1, cellSize - 1, qtColor);
            }
            delete tempBlock;
        }
    }

    // After next previews, render Hold area
    int holdLabelY = previewStartY + (previewsToShow * previewGap) + 10;
    painter.drawText(QRect(sidePanelX, holdLabelY, sidePanelWidth, 30), Qt::AlignCenter, "Hold");
    int holdDrawY = holdLabelY + 30;
    int held = engine.getHeldPiece();
    if (held != -1) {
        Block* tempHold = nullptr;
        switch (held) {
        case 0: tempHold = new TBlock(0, 0); break;
        case 1: tempHold = new OBlock(0, 0); break;
        case 2: tempHold = new IBlock(0, 0); break;
        case 3: tempHold = new LBlock(0, 0); break;
        case 4: tempHold = new JBlock(0, 0); break;
        case 5: tempHold = new ZBlock(0, 0); break;
        case 6: tempHold = new SBlock(0, 0); break;
        }
        if (tempHold) {
            QColor qtColor;
            switch (tempHold->getColor()) {
            case TetrisColor::Yellow: qtColor = Qt::yellow; break;
            case TetrisColor::Purple: qtColor = Qt::magenta; break;
            case TetrisColor::Green: qtColor = Qt::green; break;
            case TetrisColor::Red: qtColor = Qt::red; break;
            case TetrisColor::Blue: qtColor = Qt::blue; break;
            case TetrisColor::Orange: qtColor = QColor(255, 165, 0); break;
            case TetrisColor::Cyan: qtColor = Qt::cyan; break;
            }
            int minX = 4, maxX = 0;
            for (auto& cell : tempHold->getCells()) {
                if (cell.first < minX) minX = cell.first;
                if (cell.first > maxX) maxX = cell.first;
            }
            int blockPixelWidth = (maxX - minX + 1) * cellSize;
            int drawOffsetX = sidePanelX + ((sidePanelWidth - blockPixelWidth) / 2) - (minX * cellSize);
            for (auto& cell : tempHold->getCells()) {
                painter.fillRect(drawOffsetX + (cell.first * cellSize), holdDrawY + (cell.second * cellSize), cellSize - 1, cellSize - 1, qtColor);
            }
            delete tempHold;
        }
    }

    // Time label at bottom
    if (engine.getMode() == GameMode::TimeTrial) {
        painter.setPen(Qt::black);
        painter.drawText(QRect(sidePanelX, 520, sidePanelWidth, 30), Qt::AlignCenter, "Time");
        int totalSeconds = engine.getTimeRemainingMs() / 1000;
        int mins = totalSeconds / 60;
        int secs = totalSeconds % 60;
        QString timeStr = QString("%1:%2").arg(mins).arg(secs, 2, 10, QChar('0'));
        painter.drawText(QRect(sidePanelX, 550, sidePanelWidth, 30), Qt::AlignCenter, timeStr);
    }




    // Paused overlay
    if (engine.isPaused()) {
        painter.fillRect(0, 0, width(), height(), QColor(0, 0, 0, 160));
        painter.setPen(Qt::yellow);
        QFont pausedFont("Arial", 30, QFont::Bold);
        painter.setFont(pausedFont);
        painter.drawText(QRect(0, height() / 2 - 40, width(), 50), Qt::AlignCenter, "Paused");

        painter.setPen(Qt::white);
        QFont smallFont("Arial", 14, QFont::Bold);
        painter.setFont(smallFont);
        painter.drawText(QRect(0, height() / 2 + 20, width(), 30), Qt::AlignCenter, "Press Esc to Resume");
        return;
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
        painter.drawText(QRect(0, height() / 2 + 40, width(), 50), Qt::AlignCenter, "Press M for Main Menu");

    }
}






// main menu appearance/buttons