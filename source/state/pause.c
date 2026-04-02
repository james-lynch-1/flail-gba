#include "state.h"

// pause state functions

void enterPause() {
}

void updatePause() {
    if (key_hit(KEY_START)) {
        exitPause(NORMAL);
        return;
    }
    if (key_hit(KEY_SELECT)) reset();
    tte_set_pos(SCREEN_WIDTH / 2 - 3 * TILE_WIDTH, SCREEN_HEIGHT - TILE_WIDTH * 3);
    tte_write("PAUSED\n    Press SELECT to retry.");
    VBlankIntrWait();
}

void exitPause(enum GameState state) {
    setGameState(state);
    tte_erase_screen();
}
