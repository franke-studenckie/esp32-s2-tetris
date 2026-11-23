#include "Engine.h"
#include "Display.h"

Engine game(true);
Display screen;

const unsigned long DROP_INTERVAL_MS = 400;
const unsigned long GAME_OVER_DELAY_MS = 4000;

unsigned long lastDropTime = 0;
unsigned long gameOverTime = 0;
bool wasGameOver = false;

void setup() {
    Serial.begin(115200);

    if (!screen.begin(90)) {
        Serial.println("Blad inicjalizacji matrycy HUB75!");
        while (true) { delay(1000); }
    }

    screen.render(game);
    lastDropTime = millis();
}

void loop() {
    unsigned long now = millis();

    if (game.isGameOver()) {
        if (!wasGameOver) {
            wasGameOver = true;
            gameOverTime = now;
            screen.render(game);
        } else if (now - gameOverTime >= GAME_OVER_DELAY_MS) {
            game.reset();
            wasGameOver = false;
            lastDropTime = now;
            screen.render(game);
        }
        return;
    }

    if (now - lastDropTime >= DROP_INTERVAL_MS) {
        lastDropTime = now;
        game.moveBlock(0, 1);
        screen.render(game);
    }
}