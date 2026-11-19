#ifndef ENGINE_H
#define ENGINE_H

#include <memory>
#include "Board.h"
#include "Block.h"
#include "square.h"
#include <random>
#include "T.h"
#include "I.h"
#include "LR.h"
#include "LL.h"
#include "SR.h"
#include "SL.h"

class Engine {
    private:
        Board board_;
        std::unique_ptr<Block> curr_block_;
        int points_;
        bool game_over_;
        bool random_mode_;
        std::mt19937 rng_;

    public:
        Engine(bool randomMode = true) : board_(10, 20), points_(0), game_over_(false), random_mode_(randomMode) , rng_(std::random_device{}()){
            SpawnBlock();
        }

        void SpawnBlock() {
            int type = 0;
            if (random_mode_) {
                std::uniform_int_distribution<int> dist(0, 6);
                type = dist(rng_);
            }

            switch (type) {
                case 0: curr_block_ = std::make_unique<square>(4, 0); break;
                case 1: curr_block_ = std::make_unique<T>(4, 1);      break;
                case 2: curr_block_ = std::make_unique<I>(4, 0);      break;
                case 3: curr_block_ = std::make_unique<LR>(4, 1);     break;
                case 4: curr_block_ = std::make_unique<LL>(4, 1);     break;
                case 5: curr_block_ = std::make_unique<SR>(4, 0);     break;
                case 6: curr_block_ = std::make_unique<SL>(4, 0);     break;
            }

            if (checkCollision(0, 0)) {
                game_over_ = true;
            } 
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

            if (board_.isOccupied(newX, newY)) {
                return true; 
            }
        }
        return false;
    }

    void lockBlock(){
        if (game_over_ || !curr_block_) return;
        
        auto blocks = curr_block_ -> getBlocks();

        for(const auto& point : blocks){
            board_.setCell(point.x, point.y);
        }
        points_ += 100 * board_.clearFullLines();
        SpawnBlock();
    }

    bool moveBlock(int dx, int dy) {
        if(game_over_) return false;
        if (!checkCollision(dx, dy)) {
            curr_block_->move(dx, dy);
            return true;
        }
        else{
            if(dx == 0 && dy == 1){
                lockBlock();
            }
        }
        return false; 
    }

    int getPoints() const { return points_; }
    const Board& getBoard() const { return board_; }

    bool isGameOver() const { return game_over_; }
};

#endif