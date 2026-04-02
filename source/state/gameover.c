#include "state.h"

void enterGameOver() {
    REG_BG2CNT |= BG_PRIO(0);
}

void updateGameOver() {
    if (key_hit(KEY_START)) {
        exitGameOver(NORMAL);
        return;
    }
    tte_erase_screen();
    tte_set_color(0, CLR_RED);
    tte_set_pos(0, 8);
    tte_write(" GAME OVER\n Press START to retry.\n");
    VBlankIntrWait();
}

void exitGameOver(enum GameState state) {
    tte_erase_screen();
    REG_BG2CNT |= BG_PRIO(2);
    reset();
}
