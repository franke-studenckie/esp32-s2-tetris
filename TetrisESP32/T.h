#include "Block.h"


class T : public Block {
    public:
        T(int startX, int startY) : Block(startX, startY) {}

        std::vector<Point> getBlocks() const override {
            switch (rotation_) {
                case 0:
                    return { {x_ - 1, y_}, {x_, y_}, {x_ + 1, y_}, {x_, y_ - 1} };
            
            case 1: 
                // Obrót o 90 stopni w prawo: wypustka po PRAWEJ
                return { {x_, y_ - 1}, {x_, y_}, {x_, y_ + 1}, {x_ + 1, y_} };
            
            case 2: 
                // Obrót o 180 stopni: wypustka na DOLE
            
            case 3: 
                return { {x_, y_ - 1}, {x_, y_}, {x_, y_ + 1}, {x_ - 1, y_} };
            
            default:
                // Fallback (choć teoretycznie nigdy tu nie wejdzie dzięki modulo)
                return { {x_ - 1, y_}, {x_, y_}, {x_ + 1, y_}, {x_, y_ - 1} };
            }
        }
};