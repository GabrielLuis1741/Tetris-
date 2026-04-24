#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_TetrisPlusV2.h"
#include <QKeyEvent>
#include "GameEngine.h"
#include <QPushButton>
#include <QLabel>
#include <QPixmap>
#include <fstream>
#include <vector>
#include <algorithm>

class TetrisPlusV2 : public QMainWindow
{
    Q_OBJECT

public:
    TetrisPlusV2(QWidget *parent = nullptr);
    ~TetrisPlusV2();
    

protected:
    void keyPressEvent(QKeyEvent* event) override;
    void paintEvent(QPaintEvent* event) override;

private:
    Ui::TetrisPlusV2Class ui;
    GameEngine engine;

    bool inMainMenu;
    QLabel* titleLabel;
    QPushButton* endlessButton;
    QPushButton* timedButton;
	QPushButton* controlsButton;
    QPixmap menuBackground;
    QPushButton* backButton;
    QLabel* controlsLabel;

    QPushButton* highscoreButton;
    QLabel* highscoreLabel;

    std::vector<int> topEndlessScores;
    std::vector<int> topTimeTrialScores;

    void loadHighScores();
    void saveHighScore(int newScore, GameMode mode);
    void updateHighScoreDisplay();
};

