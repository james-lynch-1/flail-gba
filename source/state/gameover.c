#include "state.h"

void enterGameOver() {
    REG_BG1CNT &= ~BG_PRIO_MASK;
    REG_BG2CNT &= ~BG_PRIO_MASK;
}

void updateGameOver() {
    if (key_hit(KEY_START)) {
        setGameState(NORMAL);
        return;
    }
    tte_erase_screen();
    tte_set_color(0, CLR_RED);
    tte_set_pos(0, 8);
    tte_write(" GAME OVER\n Press START to retry.\n");
    VBlankIntrWait();
}

void exitGameOver(enum GameState state) {
    REG_BG1CNT |= BG_PRIO(2);
    REG_BG2CNT |= BG_PRIO(2);
    tte_erase_screen();
    reset();
}
