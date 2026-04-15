#include "main.h"

int main() {
    initialiseGame();
    while (1) {
        key_poll();
        qran();
        __qran_seed += key_curr_state();
        if (gGameState.justChanged) {
            gGameState.enterFunction();
            gGameState.justChanged = false;
        }
        gGameState.updateFunction();
        oam_copy(oam_mem, gObjBuffer, 128);
    }

    return 0;
}
