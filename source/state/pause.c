#include "state.h"

// pause state functions

void enterPause() {
    REG_DISPCNT &= ~(DCNT_OBJ | DCNT_BG1);
}

void updatePause() {
    key_poll();
    if (key_hit(KEY_START)) {
        exitPause(NORMAL);
        return;
    }
    VBlankIntrWait();
}

void exitPause(enum GameState state) {
    REG_DISPCNT |= DCNT_OBJ | DCNT_BG1;
    setGameState(state);
}
