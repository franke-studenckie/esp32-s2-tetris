#ifndef BLOCK_H
#define BLOCK_H

#include <vector>

struct Point{
    int x;
    int y;
};

class Block{
    protected:
        int x_;
        int y_;
        int rotation_;
        int color_;
    
    public:
        Block(int startX, int startY, int color = 1) : x_(startX), y_(startY), rotation_(0), color_(color) {}
        virtual ~Block() = default;

        virtual std::vector<Point> getBlocks() const = 0;

        void move(int dx, int dy){
            x_ += dx;
            y_ += dy;
        }

        void rotate(){
            rotation_ = (rotation_ + 1) % 4;        
        }

        void undoRotate(){
            rotation_ = (rotation_ + 3) % 4;
        }

        int getX() { return x_; }
        int getY() { return y_; }

        int getColor() const { return color_; }

};



#endif