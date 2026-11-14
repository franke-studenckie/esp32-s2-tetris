#ifndef SL_H
#define SL_H

#include "Block.h"

class SL : public Block {
public:
    SL(int startX, int startY) : Block(startX, startY) {}

    std::vector<Point> getBlocks() const override {
        if (rotation_ % 2 == 0) {
            // Poziomo
            return { {x_ - 1, y_}, {x_, y_}, {x_, y_ + 1}, {x_ + 1, y_ + 1} };
        } else {
            // Pionowo
            return { {x_ + 1, y_ - 1}, {x_ + 1, y_}, {x_, y_}, {x_, y_ + 1} };
        }
    }
};

#endif