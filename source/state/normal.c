#include "state.h"

// normal state functions

void enterNormal() {
    REG_DISPCNT |= DCNT_BG1;
    REG_BG1CNT = BG_PRIO(3) | BG_CBB(0) | BG_SBB(UI_SBB) | BG_4BPP | BG_REG_32x32;
}

void updateNormal() {
    key_poll();

    updateInputComps();
    updateAiRand();
    updatePhysics();
    updatePhysicsSimple();
    updateObjs();
    deleteMarkedEnts();

    VBlankIntrWait();

    gFrameCount++;
}

void exitNormal(GameStateEnum state) {
    setGameState(state);
}
