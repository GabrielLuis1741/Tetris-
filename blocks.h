#pragma once
#include <vector>
#include <utility>
#include <string>

enum class TetrisColor{ Yellow, Purple, Green, Red, Blue, Orange, Cyan };

class Block {
protected:
	TetrisColor color;
	int x_pos, y_pos;

	int gridSize;
	std::vector<std::pair<int, int>> cells;

public:
	Block(TetrisColor c, int x, int y, int size) : color(c), x_pos(x), y_pos(y), gridSize(size) {}
	virtual ~Block() = default;

	virtual void rotate() {
		for (auto& cell : cells) {
			int old_x = cell.first;
			int old_y = cell.second;
			cell.first = (gridSize - 1) - old_y;
			cell.second = old_x;
		}
	}

	virtual std::string getName() const = 0;

	std::vector<std::pair<int, int>> getCells() const { return cells;}
	TetrisColor getColor() const { return color; }
	int getX() const { return x_pos; }
	int getY() const { return y_pos; }

	void move(int dx, int dy) { x_pos += dx; y_pos += dy; }

	void setPosition(int x, int y) { x_pos = x; y_pos = y; }

	std::vector<std::pair<int,int>> getRotatedCells() const {
		std::vector<std::pair<int,int>> out;
		out.reserve(cells.size());
		for (auto &cell : cells) {
			int old_x = cell.first;
			int old_y = cell.second;
			int new_x = (gridSize - 1) - old_y;
			int new_y = old_x;
			out.emplace_back(new_x, new_y);
		}
		return out;
	}
};

class TBlock : public Block {
public:
	TBlock(int x, int y) : Block(TetrisColor::Yellow, x, y, 3) {
		cells = { {1,0}, {0,1}, {1,1}, {2,1} };
	}
	std::string getName() const override { return "TBlock"; }
};

class OBlock : public Block {
public:
	OBlock(int x, int y) : Block(TetrisColor::Purple, x, y, 2) {
		cells = { {0,0}, {1,0}, {0,1}, {1,1} };
	}
	std::string getName() const override { return "OBlock"; }
};

class LBlock : public Block {
public:
	LBlock(int x, int y) : Block(TetrisColor::Green, x, y, 3) {
		cells = { {2,0}, {0,1}, {1,1}, {2,1} };
	}
	std::string getName() const override { return "LBlock"; }
};

class JBlock : public Block {
public:
	JBlock(int x, int y) : Block(TetrisColor::Red, x, y, 3) {
		cells = { {0,0}, {0,1}, {1,1}, {2,1} };
	}
	std::string getName() const override { return "JBlock"; }
};

class ZBlock : public Block {
public:
	ZBlock(int x, int y) : Block(TetrisColor::Blue, x, y, 3) {
		cells = { {0,0}, {1,0}, {1,1}, {2,1} };
	}
	std::string getName() const override { return "ZBlock"; }
};
class SBlock : public Block {
public:
	SBlock(int x, int y) : Block(TetrisColor::Orange, x, y, 3) {
		cells = { {1,0}, {2,0}, {0,1}, {1,1} };
	}
	std::string getName() const override { return "SBlock"; }
};

class IBlock : public Block {
public:
	IBlock(int x, int y) : Block(TetrisColor::Cyan, x, y, 4) {
		cells = { {0,1}, {1,1}, {2,1}, {3,1} };
	}
	std::string getName() const override { return "IBlock"; }
};
