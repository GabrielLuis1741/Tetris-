#pragma once
#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include "blocks.h"

enum class GameMode{ Endless, TimeTrial};

class GameEngine {
private:
    Block* activeBlock;
    int score;     //Using encapsulation here for the player's score
    const int rows = 20;
    const int columns = 10;

    std::vector<std::vector<int>> grid;     //makes the grid for the game 

    int dropIntervalMs = 1000;
    int dropAccumulatorMs = 0;
    bool gameOver = false;    //this will be useful for keeping track of when to end game functions
	int level = 1;
	int linesCleared = 0;

    std::vector<int> nextPiecesQueue;

    GameMode mode = GameMode::Endless;
    int timeRemainingMs = 180000;
    int activeType = -1; // type index of current active block
    int heldPiece = -1; // -1 means empty
    bool canHold = true; // whether hold is allowed for current active piece
    bool paused = false;

public:
    GameEngine();
    ~GameEngine();
    //the following functions will be used polymorphically, adjusting for the type of block or other differing inputs:
    bool canPlace(Block* b, int x, int y, bool rotated = false);
    void checkLines();   
    void lockActiveBlock();
    void SpawnNextBlock();
    void reset();
    void update(int deltaMs); 

    void moveLeft();     //Move/rotate functions, will each behave slightly differently depending on the block
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

    void setMode(GameMode newMode); //has an input of the class GameMode - currently there's only two - Endless and time trial, but this could easily be scaled to implent other since we used polymorphism
    GameMode getMode() const;
    int getTimeRemainingMs() const;
    void pause();
    bool isPaused() const;
    void hold();
    int getHeldPiece() const;   
};