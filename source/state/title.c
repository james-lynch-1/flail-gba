#include "state.h"

// title state functions

void displayLevel(int x, int y) {
    CounterComponent* numDefeated = getCounterByFlags(gPlayerId, COUNTER_NUM_DEFEATED_FLAG);
    tte_set_pos(x, y);
    tte_erase_line();
    tte_write("LEVEL ");
    if (!numDefeated) {
        tte_write("error");
        return;
    }
    char numDefStr[7];
    sprintf(numDefStr, "%d", numDefeated->curr);
    tte_write(numDefStr);
}

void enterTitle() {
    REG_DISPCNT &= ~(DCNT_BG1 | DCNT_OBJ);
    tte_erase_screen();
    tte_set_pos(8, 8);
    tte_write("PRESS START");
    displayLevel(8, 16);
}

void updateTitle() {
    CounterComponent* numDefeated = getCounterByFlags(gPlayerId, COUNTER_NUM_DEFEATED_FLAG);
    if (key_hit(KEY_LEFT | KEY_RIGHT)) {
        if (numDefeated) numDefeated->curr += key_tri_horz();
        displayLevel(8, 16);
    }
    if (key_hit(KEY_START)) {
        setGameState(NORMAL);
    }
}

void exitTitle(enum GameState state) {
    tte_erase_screen();
}
