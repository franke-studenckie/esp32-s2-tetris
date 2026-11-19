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
    Engine game(false); 
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
    Engine game(false);
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
    
    EXPECT_TRUE(game.getBoard().isOccupied(4, 18));
    EXPECT_TRUE(game.getBoard().isOccupied(5, 19));
    EXPECT_EQ(game.getBlock()->getY(), 0);
}

TEST(BoardTest, ClearLinesAndGravity) {
    Board board(10, 20);

    // Wypełniamy dwa dolne wiersze (y = 18 i y = 19)
    for (int x = 0; x < 10; ++x) {
        board.setCell(x, 19);
        board.setCell(x, 18);
    }
    // Stawiamy pojedynczy "klocek kontrolny" nad nimi (x = 3, y = 17)
    board.setCell(3, 17);

    // Sprawdzamy, czy pętla z y++ poprawnie wytnie obie linie naraz
    EXPECT_EQ(board.clearFullLines(), 2);

    // Klocek z (3, 17) powinien spaść o 2 pola w dół, na (3, 19)
    EXPECT_TRUE(board.isOccupied(3, 19));
    EXPECT_FALSE(board.isOccupied(3, 17));
    // Sąsiednie pola na dole powinny być już wyczyszczone
    EXPECT_FALSE(board.isOccupied(0, 19));
    EXPECT_FALSE(board.isOccupied(0, 18));
}

//Test piętrowania: drugi klocek zatrzymuje się na pierwszym, nie na podłodze
TEST(EngineTest, BlockStacking) {
    Engine game(false);

    // Pętla while zrzuca klocek w dół aż do zamrożenia (zwrócenia false)
    while (game.moveBlock(0, 1)) {}

    // Pierwszy kwadrat leży na y=18 i y=19.
    // Drugi startuje na y=0, więc może spaść dokładnie 16 razy (do y=16, zajmując 16 i 17)
    for (int i = 0; i < 16; ++i) {
        EXPECT_TRUE(game.moveBlock(0, 1));
    }

    // 17. ruch uderza w pierwszy kwadrat i zamraża drugi klocek
    EXPECT_FALSE(game.moveBlock(0, 1));

    EXPECT_TRUE(game.getBoard().isOccupied(4, 16));
    EXPECT_TRUE(game.getBoard().isOccupied(5, 17));
    EXPECT_EQ(game.getBlock()->getY(), 0); // Trzeci klocek już czeka na górze
}

//Test pełnej rozgrywki: 5 kwadratów obok siebie czyści 2 linie i daje 200 punktów
TEST(EngineTest, ClearLinesAndScoring) {
    Engine game(false);

    // Przesunięcia w poziomie od pozycji startowej (x = 4), żeby zapełnić kolumny 0..9:
    // -4 -> x=0, -2 -> x=2, 0 -> x=4, +2 -> x=6, +4 -> x=8
    int offsets[] = { -4, -2, 0, 2, 4 };

    for (int dx : offsets) {
        int step = (dx > 0) ? 1 : -1;
        for (int i = 0; i < std::abs(dx); ++i) {
            EXPECT_TRUE(game.moveBlock(step, 0));
        }
        // Zrzucamy klocek aż do zamrożenia
        while (game.moveBlock(0, 1)) {}
    }

    // 5 kwadratów 2x2 zapełniło naraz wiersze 18 i 19 -> 2 linie * 100 pkt = 200 pkt
    EXPECT_EQ(game.getPoints(), 200);

    // Dolne wiersze powinny być znów całkowicie puste
    for (int x = 0; x < 10; ++x) {
        EXPECT_FALSE(game.getBoard().isOccupied(x, 19));
        EXPECT_FALSE(game.getBoard().isOccupied(x, 18));
    }
}

TEST(EngineTest, GameOverWhenStackReachesTop) {
    Engine game(false); 

    // Plansza ma wysokość 20 -> 9 kwadratów zajmuje y = 2..19.
    // 10. kwadrat pojawia się jeszcze legalnie na y = 0..1.
    for (int i = 0; i < 9; ++i) {
        while (game.moveBlock(0, 1)) {}
        EXPECT_FALSE(game.isGameOver());
    }

    // 10. kwadrat uderza w wieżę i zamraża się na samej górze (y = 0..1).
    // Próba spawnu 11. kwadratu na (4, 0) wywołuje kolizję -> Game Over.
    while (game.moveBlock(0, 1)) {}

    EXPECT_TRUE(game.isGameOver());
    // Po Game Over żaden ruch nie może już przejść
    EXPECT_FALSE(game.moveBlock(-1, 0));
    EXPECT_FALSE(game.moveBlock(0, 1));
}

TEST(EngineTest, RandomSpawningFitsInsideBoard) {
    Engine game(true); // tryb losowy

    // Losujemy 50 klocków na pustej planszy — żaden nie może wystawać poza y = 0
    for (int i = 0; i < 50; ++i) {
        game.SpawnBlock();
        ASSERT_NE(game.getBlock(), nullptr);
        EXPECT_FALSE(game.isGameOver());
    }
}

TEST(EngineTest, RotationBlockedByWall) {
    Engine game(false);
    game.SpawnBlock(1); // Klocek T na (4, 1), stan 0 (poziomy)

    // Obrót na środku planszy (stan 0 -> stan 1, pionowy z wypustką w prawo)
    EXPECT_TRUE(game.rotateBlock());

    // W stanie 1 klocek T zajmuje x_ oraz x_+1, więc może dojechać 4 razy w lewo do x_ = 0
    for (int i = 0; i < 4; ++i) {
        EXPECT_TRUE(game.moveBlock(-1, 0));
    }

    // Przy ścianie (x_ = 0) obrót do stanu 2 wymagałby pola x = -1 -> musi zostać zablokowany
    EXPECT_FALSE(game.rotateBlock());

    // Po odsunięciu się o 1 pole w prawo (x_ = 1) obrót znowu jest legalny
    EXPECT_TRUE(game.moveBlock(1, 0));
    EXPECT_TRUE(game.rotateBlock());
}

TEST(EngineTest, ColorsPreservedAfterLock) {
    Engine game(false);
    game.SpawnBlock(1); // Klocek T (kolor = 2) na (4, 1)

    EXPECT_EQ(game.getBlock()->getColor(), 2);

    while (game.moveBlock(0, 1)) {} // Zrzucamy T na samo dno (y = 18 i 19)

    // Zamrożone komórki powinny pamiętać ID koloru klocka T (2)
    EXPECT_EQ(game.getBoard().getCell(3, 19), 2);
    EXPECT_EQ(game.getBoard().getCell(4, 19), 2);
    EXPECT_EQ(game.getBoard().getCell(5, 19), 2);
    EXPECT_EQ(game.getBoard().getCell(4, 18), 2);
    // Puste pole obok nadal ma 0
    EXPECT_EQ(game.getBoard().getCell(0, 19), 0);
}

TEST(EngineTest, ResetRestoresInitialState) {
    Engine game(false);

    // 1. Najpierw zdobywamy 200 punktów (5 kwadratów w dolnym rzędzie)
    int offsets[] = { -4, -2, 0, 2, 4 };
    for (int dx : offsets) {
        int step = (dx > 0) ? 1 : -1;
        for (int i = 0; i < std::abs(dx); ++i) {
            game.moveBlock(step, 0);
        }
        while (game.moveBlock(0, 1)) {}
    }
    ASSERT_EQ(game.getPoints(), 200);

    // 2. Następnie doprowadzamy grę do Game Over (10 kwadratów jeden na drugim)
    for (int i = 0; i < 10; ++i) {
        while (game.moveBlock(0, 1)) {}
    }
    ASSERT_TRUE(game.isGameOver());

    // 3. Resetujemy i sprawdzamy, czy wszystko wróciło do stanu początkowego
    game.reset();

    EXPECT_FALSE(game.isGameOver());
    EXPECT_EQ(game.getPoints(), 0);
    EXPECT_FALSE(game.getBoard().isOccupied(4, 19));
    EXPECT_EQ(game.getBoard().getCell(4, 19), 0);
    ASSERT_NE(game.getBlock(), nullptr);
    EXPECT_EQ(game.getBlock()->getY(), 0);
    EXPECT_TRUE(game.moveBlock(0, 1)); // Gra znów przyjmuje ruchy
}