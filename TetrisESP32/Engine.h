#ifndef ENGINE_H
#define ENGINE_H

#include <memory>
#include "Board.h"
#include "Block.h"
#include "square.h"

class Engine {
    private:
        Board board_;
        std::unique_ptr<Block> curr_block_;
        int points_;

    public:
        Engine() : board_(10, 20), points_(0) {
            SpawnBlock();
        }

        void SpawnBlock() {
            curr_block_ = std::make_unique<square>(4, 0); 
        }

        Block* getBlock() const {
        return curr_block_.get();
    }

    bool checkCollision(int dx, int dy) const {
        if (!curr_block_) return true;

        auto blocks = curr_block_->getBlocks();
        
        for (const auto& point : blocks) {
            int newX = point.x + dx;
            int newY = point.y + dy;

            if (!board_.isInside(newX, newY)) {
                return true; 
            }
        }
        return false;
    }

    bool moveBlock(int dx, int dy) {
        if (!checkCollision(dx, dy)) {
            curr_block_->move(dx, dy);
            return true;
        }
        return false; 
    }
};

#endif