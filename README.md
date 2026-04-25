# Tetris Plus

A recreation of the classic game Tetris, built in Qt. Designed to provide an entertaining experience with different game modes and difficulties

---
## Team members

Jackson Read

Patrick Riley

Gabriel Luis

---
## About

This project brings the timeless gameplay loop of Tetris, and adds modern features like score tracking, profile creation, multiple game modes and varying difficulties

---
## All Planned Features
### Point Gain system

Players will earn points by filling and clearing full horizontal rows, with the score increasing for every row cleared at once.

### Randomized Block Spawns

Tetris blocks are randomly selected from a predetermined set of shapes and spawned at timed intervals throughout gameplay.

### 3. Game Modes
- **Endless Mode** – Play until the blocks fill the screen. Survive as long as possible and accumulate points.
- **Time Trial Mode** – Race against the clock to earn as many points as possible before time runs out.

### 4. Difficulty Settings
Multiple difficulty levels are available, adjusting block fall speed and spawn rate. Difficulty settings are compatible with all game modes.

### 5. High Score Tracking
High scores are automatically recorded per game mode:
- **Endless Mode** – Tracks highest points earned.
- **Time Trial Mode** – Tracks best time or highest points within the time limit.

### 6. Local High Score
Added a local scoring feature. Separate scores for both game modes are saved locally, allowing for scores to be saved between gaming sessions.

---

## Built With

- **Qt** – Used for the main menu, UI, and overall application framework
- **C++** – Core game logic

---

## Game Conditions

The game follows classic Tetris rules:
- Blocks fall from the top of the game window.
- Filling a complete horizontal row clears it and awards points.
- The game ends when there is no longer space for new blocks to spawn.

---

## OOP Concepts and usage

### Classes and Objects

The project is built around multiple interacting classes. GameEngine manages the boolean grid and all game logic. MenuManager tracks which screen is currently active. TetrisPlusV2 is the Qt window that reads from both and renders the result. Each class is instantiated as an object and communicates through public method calls only.

### Encapsulation

Every class keeps its internal data private. GameEngine holds the grid, score, and active block as private members. Controlled access is provided through public methods like getGrid(), getScore(), and isGameOver(). 

### Inheritance

Block serves as the abstract base class for all seven Tetris piece types. Each piece inherits from Block and provides its own implementation of the shape and color. This means all seven pieces can be handled through a single Block* pointer without the engine needing to know which specific piece it is working with.

### Polymorphism

Runtime polymorphism is demonstrated through virtual functions on the Block base class. When GameEngine calls activeBlock->getCells() or activeBlock->getRotatedCells(), the call is resolved at runtime to whichever derived piece type the pointer actually holds. This means the collision detection, locking, and rendering code works identically for all seven piece types without a single if/else or switch to check the piece type.

### Abstract Classes

Block is an abstract base class containing pure virtual functions for getCells() and getRotatedCells(). Due to this, Block cannot be instantiated directly, intead  one of the concrete derived classes must be used.

### Composition

The project demonstrates multiple levels of has-a relationships. TetrisPlusV2 has-a GameEngine and has-a MenuManager. GameEngine has-a Block* for the active piece. This means the window owns the engine, the engine owns the active block, and destroying the window cascades cleanup through all owned objects automatically.

### Templates

Grid<T> is a template class that provides a generic two-dimensional data structure. GameEngine uses it as Grid<bool> for the boolean collision grid. Using a template here means the same grid implementation could store any data type, demonstrating reusable generic programming.

---

## Getting Started

To run TetrisPlus, just download the required files from the repository, open Microsoft Visual Studio, with Qt Visual Studio Tools installed. Then run and build the program to play!

Visual Studio download link:

https://visualstudio.microsoft.com/downloads/

Tutorial on Qt for Visual Studio download:

https://youtu.be/lXsJeFQRZH0?si=pYfTuiJDSkXxMcko

## AI Disclosure

Portions of this project were developed with the assistance of AI tools, including GitHub Copilot and/or Gemini. All AI-generated content was reviewed, tested, and integrated by team members. The use of AI was intended to support and accelerate development, not replace the understanding and decision-making of the development team.
