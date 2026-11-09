#ifndef BOARD_H
#define BOARD_H

class Board {
private:
    int width_;
    int height_;

public:
    Board(int w = 10, int h = 20);
    int getWidth() const;
    int getHeight() const;
    
    bool isInside(int x, int y) const;
};

#endif