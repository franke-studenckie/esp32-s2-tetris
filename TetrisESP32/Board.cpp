#include "Board.h"

Board::Board(int w, int h) : width_(w), height_(h) {}

int Board::getWidth() const { return width_; }
int Board::getHeight() const { return height_; }

bool Board::isInside(int x, int y) const {
    return x >= 0 && x < width_ && y >= 0 && y < height_;
}