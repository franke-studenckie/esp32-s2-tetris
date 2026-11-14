#ifndef LR_H
#define LR_H

#include "Block.h"

class LR : public Block {
public:
    LR(int startX, int startY) : Block(startX, startY) {}

    std::vector<Point> getBlocks() const override {
        switch (rotation_) {
            case 0: return { {x_ - 1, y_}, {x_, y_}, {x_ + 1, y_}, {x_ + 1, y_ - 1} };
            case 1: return { {x_, y_ - 1}, {x_, y_}, {x_, y_ + 1}, {x_ + 1, y_ + 1} };
            case 2: return { {x_ - 1, y_}, {x_, y_}, {x_ + 1, y_}, {x_ - 1, y_ + 1} };
            case 3: return { {x_, y_ - 1}, {x_, y_}, {x_, y_ + 1}, {x_ - 1, y_ - 1} };
            default: return {};
        }
    }
};

#endif