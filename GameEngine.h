#pragma once
#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include "blocks.h"

class GameEngine {
private:
    Block* activeBlock;
    int score;
    const int rows = 20;
    const int columns = 10;

    std::vector<std::vector<int>> grid;

    int dropIntervalMs = 1000;
    int dropAccumulatorMs = 0;
    bool gameOver = false;
	int level = 1;
	int linesCleared = 0;

    std::vector<int> nextPiecesQueue;

public:
    GameEngine();
    ~GameEngine();

    bool canPlace(Block* b, int x, int y, bool rotated = false);
    void checkLines();
    void lockActiveBlock();
    void SpawnNextBlock();
    void reset();
    void update(int deltaMs);

    void moveLeft();
    void moveRight();
    void moveDown();
    void rotateActiveBlock();
    void hardDrop();

    void PrintGrid();

    int getScore() const;
    std::vector<std::vector<int>> getGrid() const;
    bool isGameOver() const;
	Block* getActiveBlock() const;

    int getLevel() const;

    std::vector<int> getNextPieces() const;
};