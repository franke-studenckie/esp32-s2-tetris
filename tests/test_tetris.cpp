#include <gtest/gtest.h>
#include "Board.h"
#include "square.h"
#include "T.h"
#include "Block.h"
#include "Engine.h"

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

TEST(EngineTest, WallCollisionDetection) {
    Engine game; 
    // Przesuwamy kwadrat w prawo krok po kroku:
    EXPECT_TRUE(game.moveBlock(1, 0)); // Ruch 1: x = 5 (zajmuje 5 i 6). Ruch legalny.
    EXPECT_TRUE(game.moveBlock(1, 0)); // Ruch 2: x = 6 (zajmuje 6 i 7). Ruch legalny.
    EXPECT_TRUE(game.moveBlock(1, 0)); // Ruch 3: x = 7 (zajmuje 7 i 8). Ruch legalny.
    EXPECT_TRUE(game.moveBlock(1, 0)); // Ruch 4: x = 8 (zajmuje 8 i 9). Opiera się o prawą ścianę!
    
    // Ruch 5 w prawo powinien zostać zablokowany, bo prawy bok klocka wyleciałby na pole nr 10!
    EXPECT_FALSE(game.moveBlock(1, 0)); 
    
    // Upewniamy się, że pomimo nieudanej próby ruchu, nasz klocek faktycznie został na swoim bezpiecznym miejscu
    EXPECT_EQ(game.getBlock()->getX(), 8);
}

TEST(EngineTest, FloorCollisionDetection) {
    Engine game;
    
    // Kwadrat startuje na Y = 0 i zajmuje dwa pola w pionie (Y=0 oraz Y=1).
    // Plansza ma wysokość 20 (legalne pola to od 0 do 19).
    // Chcemy opuścić go na samo dno. Skoro zajmuje 2 pola, 
    // może zjechać maksymalnie na pole Y = 18 (zajmie wtedy 18 i 19).
    
    for (int i = 0; i < 18; ++i) {
        EXPECT_TRUE(game.moveBlock(0, 1)); 
    }
    
    // Teraz klocek opiera się o podłogę. 
    // Próba wykonania 19-tego ruchu powinna zostać zablokowana!
    EXPECT_FALSE(game.moveBlock(0, 1)); 
    
    // Sprawdzamy, czy klocek nie przebił planszy i faktycznie zatrzymał się na Y = 18
    EXPECT_EQ(game.getBlock()->getY(), 18);
}