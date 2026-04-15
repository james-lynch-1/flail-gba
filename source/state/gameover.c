#include "state.h"

void enterGameOver() {
    REG_BG1CNT &= ~BG_PRIO_MASK;
    REG_BG2CNT &= ~BG_PRIO_MASK;
    tte_set_color(0, CLR_RED);
    tte_set_pos(0, 24);
    tte_write(" GAME OVER\n Press START to retry.\n");
}

void updateGameOver() {
    if (key_hit(KEY_START)) {
        setGameState(NORMAL);
        return;
    }
    VBlankIntrWait();
}

void exitGameOver(enum GameState state) {
    REG_BG1CNT |= BG_PRIO(PRIO_UI);
    REG_BG2CNT |= BG_PRIO(PRIO_TEXT);
    tte_erase_screen();
    reset();
}
