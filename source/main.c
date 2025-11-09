#include "main.h"

int main() {
    initialiseGame();
    while (1) {
        gGameState.updateFunction();
        oam_copy(oam_mem, gObjBuffer, 128);
    }

    return 0;
}
