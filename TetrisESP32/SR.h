#ifndef SR_H
#define SR_H

#include "Block.h"

class SR : public Block {
public:
    SR(int startX, int startY) : Block(startX, startY) {}

    std::vector<Point> getBlocks() const override {
        if (rotation_ % 2 == 0) {
            // Poziomo
            return { {x_, y_}, {x_ + 1, y_}, {x_ - 1, y_ + 1}, {x_, y_ + 1} };
        } else {
            // Pionowo
            return { {x_, y_ - 1}, {x_, y_}, {x_ + 1, y_}, {x_ + 1, y_ + 1} };
        }
    }
};

#endif