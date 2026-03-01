#include "state.h"

// normal state functions

void enterNormal() {
    REG_DISPCNT |= DCNT_BG1;
    REG_BG1CNT = BG_PRIO(3) | BG_CBB(0) | BG_SBB(UI_SBB) | BG_4BPP | BG_REG_32x32;
    // ZZZ initialise stars
        // need to select randomly from a list of star arrangements
        // or select from a specific order
        // have a queue of star lines to spawn? star data and timer
    // ZZZ start star timers
    // 
}

void updateNormal() {
    key_poll();

    updateInputComps();
    updateAiRand();
    updatePhysics();
    updatePhysicsSimple();
    updateObjs();
    deleteMarkedEnts();
    updateTimers();
    // do queued events (e.g. a timer has run out). A timer should have a callback fn ptr

    VBlankIntrWait();

    gFrameCount++;
}

void exitNormal(GameStateEnum state) {
    setGameState(state);
}
