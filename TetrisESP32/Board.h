#ifndef BOARD_H
#define BOARD_H

#include <vector>

class Board {
private:
    int width_;
    int height_;
    std::vector<std::vector<int>> grid_; 

public:
    Board(int w = 10, int h = 20) : width_(w), height_(h) {
        grid_.resize(height_, std::vector<int>(width_, 0));
    }

    int getWidth() const { return width_; }
    int getHeight() const { return height_; }

    bool isInside(int x, int y) const {
        return x >= 0 && x < width_ && y >= 0 && y < height_;
    }

    //Sprawdza, czy pole jest już zajęte przez zamrożony klocek
    bool isOccupied(int x, int y) const {
        if (!isInside(x, y)) return true; // Poza planszą traktujemy jako zajęte ściany
        return grid_[y][x] != 0;
    }

    //Zamraża klocek na planszy (zmienia 0 na 1)
    void setCell(int x, int y, int color = 1) {
        if (isInside(x, y)) {
            grid_[y][x] = color; 
        }
    }

    int getCell(int x, int y) const {
        if (!isInside(x, y)) return 0;
        return grid_[y][x];
    }

    void clear() {
        grid_.assign(height_, std::vector<int>(width_, 0));
    }

    int clearFullLines() {
        int linesCleared = 0;
        
        for (int y = height_ - 1; y >= 0; --y) {
            bool isFull = true;
            for (int x = 0; x < width_; ++x) {
                if (grid_[y][x] == 0) {
                    isFull = false;
                    break;
                }
            }

            if (isFull) {
                // Usuwamy pełną linię (cały wiersz z wektora)
                grid_.erase(grid_.begin() + y);
                // Wstawiamy nową, pustą linię na samej górze
                grid_.insert(grid_.begin(), std::vector<int>(width_, 0));
                
                linesCleared++;
                y++; // Po usunięciu linii wszystko spadło o 1, więc musimy sprawdzić ten sam indeks y ponownie
            }
        }
        return linesCleared;
    }
};

#endif