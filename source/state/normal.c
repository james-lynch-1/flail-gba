#include "state.h"

// normal state functions

void enterNormal() {
    REG_DISPCNT |= DCNT_BG1;
    REG_BG1CNT = BG_PRIO(0) | BG_CBB(0) | BG_SBB(UI_SBB) | BG_4BPP | BG_REG_32x32; // ui
}

void updateNormal() {
    key_poll();

    updateInputComps();
    updateInputCheckers();
    updateAi();
    updateAiRand();
    updatePlayerPhysics();
    updatePhysics();
    updateHitboxes();
    updatePhysicsSimple();
    updateObjs();
    updateDebugBlobs();
    updateSpawners();
    updateTimers();

    updateUINormal();

    deleteMarkedEnts();

    VBlankIntrWait();

    gFrameCount++;
}

void exitNormal(enum GameState state) {
    setGameState(state);
}
