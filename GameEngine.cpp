#include "GameEngine.h"
#include <algorithm>

GameEngine::GameEngine() {  // Create the grid and set up the score and blocks
    score = 0; 
    activeBlock = nullptr;
    grid = std::vector<std::vector<int>>(rows, std::vector<int>(columns, 0));
}

GameEngine::~GameEngine() { // destructor for each block after being placed
    if (activeBlock) {
        delete activeBlock;
        activeBlock = nullptr;
    }
}

bool GameEngine::canPlace(Block* b, int x, int y, bool rotated) {   //
    if (!b) return false;   // boolean to check if the block can be added to the cells its going to
    std::vector<std::pair<int, int>> cells; // actually create the cells (components of the block)
    if (rotated) cells = b->getRotatedCells();  // check if blocks are rotated 
    else cells = b->getCells();                 // and see if collisions apply
    for (auto& c : cells) {     // loop through every cell in the created block
        int worldX = x + c.first;   // checks for both grid offset via movemement
        int worldY = y + c.second;  // and grid offset due to block shape
        if (worldX < 0 || worldX >= columns || worldY < 0 || worldY >= rows) return false;  // check if cells are out of bounds
        if (grid[worldY][worldX] != 0) return false;    // actual collision check with previously placed blocks
    }
    return true;    // if every condition above is met, then block is moved to desired location
}

void GameEngine::checkLines() {     // line checking for scoring full rows
    for (int i = rows - 1; i >= 0; i--) {   
        bool fullRow = true;            // start with a true for the full row and try to disprove
        for (int j = 0; j < columns; j++) {
            if (grid[i][j] == 0) { fullRow = false; break; }    // if any point in the row is empty, does not qualify for a full row
        }
        if (fullRow) {  // give points after determining that it is indeed a full row
            score += 100;
			linesCleared++;
			level = (linesCleared / 10) + 1;   // increase level every 10 lines cleared
			dropIntervalMs = std::max(100, 1000 - (level - 1) * 100);   // increase drop speed with level, capped at 100ms
            grid.erase(grid.begin() + i);   // delete all blocks in the row after scoring
            grid.insert(grid.begin(),std::vector<int>(columns, 0));    // insert empty spaces in the grid after deletion 
            i++;    // move to next row
        }
    }
}

void GameEngine::SpawnNextBlock() {     
    if (activeBlock) { delete activeBlock; activeBlock = nullptr; }

    if (nextPiecesQueue.empty()) {
        for (int i = 0; i < 4; i++) {
            nextPiecesQueue.push_back(rand() % 7);
        }
    }

	int nextType = nextPiecesQueue.front();
	nextPiecesQueue.erase(nextPiecesQueue.begin());
	nextPiecesQueue.push_back(rand() % 7);
    
    int startX = 3, startY = 0;     
    switch (nextType) {          
    case 0: activeBlock = new TBlock(startX, startY); break;
    case 1: activeBlock = new OBlock(startX, startY); break;
    case 2: activeBlock = new IBlock(startX, startY); break;
    case 3: activeBlock = new LBlock(startX, startY); break;
    case 4: activeBlock = new JBlock(startX, startY); break;
    case 5: activeBlock = new ZBlock(startX, startY); break;
    case 6: activeBlock = new SBlock(startX, startY); break;
    }
    if (!canPlace(activeBlock, activeBlock->getX(), activeBlock->getY()))   // if a block obstructs the spawn location then end the game as a loss condition
        gameOver = true;
}

void GameEngine::lockActiveBlock() {
    if (!activeBlock) return;
    for (auto& c : activeBlock->getCells()) {
        int worldX = activeBlock->getX() + c.first;
        int worldY = activeBlock->getY() + c.second;
        if (worldY >= 0 && worldY < rows && worldX >= 0 && worldX < columns)
			grid[worldY][worldX] = static_cast<int>(activeBlock->getColor()) + 1; // Store color as non-zero value for grid representation
    }
    delete activeBlock;
    activeBlock = nullptr;
    checkLines();
}

void GameEngine::update(int deltaMs) {
    if (gameOver) return;
    dropAccumulatorMs += deltaMs;
    if (!activeBlock) SpawnNextBlock();
    while (dropAccumulatorMs >= dropIntervalMs && !gameOver) {
        dropAccumulatorMs -= dropIntervalMs;
        int nx = activeBlock->getX();
        int ny = activeBlock->getY() + 1;
        if (canPlace(activeBlock, nx, ny)) activeBlock->move(0, 1);
        else { lockActiveBlock(); SpawnNextBlock(); }
    }
}

void GameEngine::moveLeft() {
    if (activeBlock && canPlace(activeBlock, activeBlock->getX() - 1, activeBlock->getY()))
        activeBlock->move(-1, 0);
}

void GameEngine::moveRight() {
    if (activeBlock && canPlace(activeBlock, activeBlock->getX() + 1, activeBlock->getY()))
        activeBlock->move(1, 0);
}

void GameEngine::moveDown() {
    if (!activeBlock) return;
    if (canPlace(activeBlock, activeBlock->getX(), activeBlock->getY() + 1))
        activeBlock->move(0, 1);
    else { lockActiveBlock(); SpawnNextBlock(); }
}

void GameEngine::hardDrop() {
    if (!activeBlock || gameOver) return;
    int dropDistance = 0;

    while (canPlace(activeBlock, activeBlock->getX(), activeBlock->getY() + 1)) {
        activeBlock->move(0, 1);
		dropDistance++;
    }
    score += (dropDistance * 2);
    lockActiveBlock();
    SpawnNextBlock();
}

void GameEngine::rotateActiveBlock() {
    if (!activeBlock) return;

    // Try rotation with a series of small offsets (wall kicks).
    // We attempt the rotation in-place first, then try common horizontal shifts
    // and a small upward shift to allow rotation near walls/obstacles.
    const std::vector<std::pair<int,int>> kicks = {
        {0, 0}, {-1, 0}, {1, 0}, {-2, 0}, {2, 0}, {0, -1}, {-1, -1}, {1, -1}
    };

    int baseX = activeBlock->getX();
    int baseY = activeBlock->getY();
    for (auto &k : kicks) {
        int nx = baseX + k.first;
        int ny = baseY + k.second;
        if (canPlace(activeBlock, nx, ny, true)) {
            activeBlock->rotate();
            // move by the kick offset
            activeBlock->move(k.first, k.second);
            break;
        }
    }
}

int GameEngine::getScore() const { return score; }
std::vector<std::vector<int>> GameEngine::getGrid() const { return grid; }
bool GameEngine::isGameOver() const { return gameOver; }

Block* GameEngine::getActiveBlock() const { return activeBlock; }

std::vector<int> GameEngine::getNextPieces() const { return nextPiecesQueue; }

int GameEngine::getLevel() const { return level; }

void GameEngine::reset() {
    score = 0;
    gameOver = false;
    dropAccumulatorMs = 0;

    if (activeBlock) {
        delete activeBlock;
        activeBlock = nullptr;
    }

    for (int row = 0; row < rows; row++) {
        for (int col =0; col < columns; col++) {
            grid[row][col] = 0;
		}
    }

    nextPiecesQueue.clear();
    SpawnNextBlock();
}