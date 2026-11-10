#include "Block.h"
#ifndef SQUARE_H
#define SQUARE_H

class square : public Block {
    public:
        square(int startX, int startY) : Block(startX, startY) {}

        std::vector<Point> getBlocks() const override {
            return{ 
                {x_, y_}, {x_ + 1, y_},
                {x_, y_ + 1}, {x_ + 1, y_ + 1}
            };
        }
};

#endif