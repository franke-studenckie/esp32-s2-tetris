#ifndef DISPLAY_H
#define DISPLAY_H

#include <memory>
#include <Adafruit_GFX.h>
#include <ESP32-HUB75-MatrixPanel-I2S-DMA.h>
#include <Fonts/TomThumb.h>
#include "Engine.h"

class Display {
private:
    enum HubPins {
        R1_PIN = 13, R2_PIN = 11,
        G1_PIN = 7,  G2_PIN = 6,
        B1_PIN = 12, B2_PIN = 14,
        A_PIN  = 21, B_PIN  = 5,
        C_PIN  = 10, D_PIN  = 4,
        E_PIN  = 33, LAT_PIN = 39,
        OE_PIN = 38, CLK_PIN = 34
    };

    static constexpr int DISP_RES_X = 64;
    static constexpr int DISP_RES_Y = 32;
    static constexpr int DISP_CHAIN = 1;

    // --- UKŁAD EKRANU (zmieniasz tylko te 4 stałe) ---
    // Wariant A (2x2 + HUD):   CELL_SIZE=2, OFFSET_X=6, OFFSET_Y=22, SHOW_HUD=true
    // Wariant B (3x3 full):    CELL_SIZE=3, OFFSET_X=1, OFFSET_Y=2,  SHOW_HUD=false
    static constexpr int  CELL_SIZE = 2;
    static constexpr int  OFFSET_X  = 6;
    static constexpr int  OFFSET_Y  = 22;
    static constexpr bool SHOW_HUD  = true;

    std::unique_ptr<MatrixPanel_I2S_DMA> display_;

    uint16_t colorFromId(int id) const {
        switch (id) {
            case 1: return display_->color565(255, 220, 0);   // square - żółty
            case 2: return display_->color565(170, 0, 255);   // T      - fioletowy
            case 3: return display_->color565(0, 230, 255);   // I      - cyjan
            case 4: return display_->color565(255, 120, 0);   // LR     - pomarańczowy
            case 5: return display_->color565(0, 80, 255);    // LL     - niebieski
            case 6: return display_->color565(0, 220, 60);    // SR     - zielony
            case 7: return display_->color565(255, 30, 30);   // SL     - czerwony
            default: return 0;
        }
    }

    void drawCell(int gridX, int gridY, int colorId) {
        if (colorId == 0) return;
        int px = OFFSET_X + gridX * CELL_SIZE;
        int py = OFFSET_Y + gridY * CELL_SIZE;
        display_->fillRect(px, py, CELL_SIZE, CELL_SIZE, colorFromId(colorId));
    }

    void drawHud(int points, int highScore) {
        display_->setFont(&TomThumb);
        display_->setTextSize(1);
        display_->setTextWrap(false);

        display_->setTextColor(display_->color565(200, 200, 200));
        display_->setCursor(1, 7);
        display_->print("P:");
        display_->print(points);

        display_->setTextColor(display_->color565(255, 200, 0));
        display_->setCursor(1, 15);
        display_->print("H:");
        display_->print(highScore);
    }

    void drawGameOver(int points, int highScore) {
        display_->setFont(&TomThumb);
        display_->setTextSize(1);
        display_->setTextWrap(false);

        display_->setTextColor(display_->color565(255, 40, 40));
        display_->setCursor(8, 16);
        display_->print("GAME");
        display_->setCursor(8, 24);
        display_->print("OVER");

        display_->setTextColor(display_->color565(255, 255, 255));
        display_->setCursor(2, 38);
        display_->print("PTS:");
        display_->setCursor(2, 46);
        display_->print(points);

        display_->setTextColor(display_->color565(255, 200, 0));
        display_->setCursor(2, 56);
        display_->print("HI:");
        display_->print(highScore);
    }

public:
    bool begin(uint8_t brightness = 90) {
        HUB75_I2S_CFG::i2s_pins pins = {
            R1_PIN, G1_PIN, B1_PIN, R2_PIN, G2_PIN, B2_PIN,
            A_PIN, B_PIN, C_PIN, D_PIN, E_PIN, LAT_PIN, OE_PIN, CLK_PIN
        };

        HUB75_I2S_CFG cfg(DISP_RES_X, DISP_RES_Y, DISP_CHAIN, pins);
        cfg.double_buff = true; // Podwójny bufor eliminuje migotanie klatek
        cfg.clkphase = false;     // odwraca zbocze zegara CLK (najczęściej naprawia dół)
        cfg.latch_blanking = 2;

        display_ = std::make_unique<MatrixPanel_I2S_DMA>(cfg);
        if (!display_->begin()) {
            return false;
        }

        display_->setBrightness8(brightness);
        display_->setRotation(3); // Pionowo: 32x64 px
        display_->clearScreen();
        display_->flipDMABuffer();
        return true;
    }

    void render(const Engine& engine) {
        if (!display_) return;

        display_->clearScreen();

        if (engine.isGameOver()) {
            drawGameOver(engine.getPoints(), engine.getHighScore());
            display_->flipDMABuffer();
            return;
        }

        if (SHOW_HUD) {
            drawHud(engine.getPoints(), engine.getHighScore());
        }

        const Board& board = engine.getBoard();
        int boardPxW = board.getWidth() * CELL_SIZE;
        int boardPxH = board.getHeight() * CELL_SIZE;

        // Ramka 1 px wokół planszy
        display_->drawRect(
            OFFSET_X - 1, OFFSET_Y - 1,
            boardPxW + 2, boardPxH + 2,
            display_->color565(70, 70, 70)
        );

        // 1. Zamrożone klocki z planszy
        for (int y = 0; y < board.getHeight(); ++y) {
            for (int x = 0; x < board.getWidth(); ++x) {
                drawCell(x, y, board.getCell(x, y));
            }
        }

        // 2. Aktualnie spadający klocek
        const Block* active = engine.getBlock();
        if (active) {
            int colorId = active->getColor();
            for (const auto& p : active->getBlocks()) {
                drawCell(p.x, p.y, colorId);
            }
        }

        display_->flipDMABuffer();
    }
};

#endif