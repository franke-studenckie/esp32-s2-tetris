#include <gtest/gtest.h>
#include "Board.h"
#include "square.h"
#include "T.h"
#include "Block.h"

TEST(BoardTest, DefaultDimensions) {
    Board board;
    EXPECT_EQ(board.getWidth(), 10);
    EXPECT_EQ(board.getHeight(), 20);
}

TEST(BoardTest, ChecksBoundaries) {
    Board board(10, 20);
    EXPECT_TRUE(board.isInside(5, 10));  
    EXPECT_FALSE(board.isInside(-1, 5)); 
    EXPECT_FALSE(board.isInside(10, 5)); 
    EXPECT_FALSE(board.isInside(5, 20)); 
}

TEST(BlockTest, Trotation) {
    std::unique_ptr<Block> klocek = std::make_unique<T>(5, 5);
    
    auto bloki_0 = klocek->getBlocks();
    // Sprawdzamy, czy czwarty blok (wypustka) ma współrzędną Y o 1 mniejszą (wyżej na planszy)
    EXPECT_EQ(bloki_0[3].y, 4); 

    klocek->rotate();
    
    auto bloki_1 = klocek->getBlocks();
    // Sprawdzamy, czy czwarty blok (wypustka) ma teraz współrzędną X o 1 większą
    EXPECT_EQ(bloki_1[3].x, 6);
}

TEST(BlockTest, SquareBlocks) {
    // Tworzymy kwadrat na pozycji początkowej x=5, y=5
    std::unique_ptr<Block> klocek = std::make_unique<square>(5, 5);

    // Pobieramy wygenerowane współrzędne 4 małych kwadracików
    auto bloki = klocek->getBlocks();

    // 1. Sprawdzamy, czy w ogóle zwróciło 4 punkty
    ASSERT_EQ(bloki.size(), 4);

    // 2. Sprawdzamy dokładne współrzędne każdego z nich po Twoich poprawkach:
    
    // bloki[0] to {x_, y_} -> lewy górny
    EXPECT_EQ(bloki[0].x, 5);
    EXPECT_EQ(bloki[0].y, 5);

    // bloki[1] to {x_ + 1, y_} -> prawy górny
    EXPECT_EQ(bloki[1].x, 6);
    EXPECT_EQ(bloki[1].y, 5);

    // bloki[2] to {x_, y_ + 1} -> lewy dolny
    EXPECT_EQ(bloki[2].x, 5);
    EXPECT_EQ(bloki[2].y, 6);

    // bloki[3] to {x_ + 1, y_ + 1} -> prawy dolny
    EXPECT_EQ(bloki[3].x, 6);
    EXPECT_EQ(bloki[3].y, 6);
}