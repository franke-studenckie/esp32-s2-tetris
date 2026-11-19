#ifndef I_H
#define I_H

#include "Block.h"

class I : public Block {
public:
    I(int startX, int startY) : Block(startX, startY, 3) {}

    std::vector<Point> getBlocks() const override {
        if (rotation_ % 2 == 0) {
            // Poziomo
            return { {x_ - 1, y_}, {x_, y_}, {x_ + 1, y_}, {x_ + 2, y_} };
        } else {
            // Pionowo
            return { {x_, y_ - 1}, {x_, y_}, {x_, y_ + 1}, {x_, y_ + 2} };
        }
    }
};

#endif