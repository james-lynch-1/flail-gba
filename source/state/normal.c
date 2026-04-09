#include "state.h"

// normal state functions

void enterNormal() {
    REG_DISPCNT |= DCNT_BG1 | DCNT_OBJ;
    REG_BG1CNT = BG_PRIO(1) | BG_CBB(0) | BG_SBB(UI_SBB) | BG_4BPP | BG_REG_32x32; // ui
}

void updateNormal() {
    if (gHitstunFrameCount) {
        gHitstunFrameCount--;
        VBlankIntrWait();
        gFrameCount--;
        return;
    }
    updatePlayerStuff();
    updateInputComps();
    updateInputCheckers();
    updateAi();
    updateAiRand();
    updatePlayerPhysics();
    updatePhysics();
    updateHitboxes();
    updatePhysicsSimple();
    updateObjs();
#ifdef DEBUG
    updateDebugBlobs();
#endif
    updateSpawners();
    updateTimers();

    updateUINormal();

    doTasks();
    deleteMarkedEnts();

    VBlankIntrWait();

    gFrameCount++;
}

void exitNormal(enum GameState state) {
    setGameState(state);
}
