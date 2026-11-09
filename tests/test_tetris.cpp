#include <gtest/gtest.h>
#include "Board.h"

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