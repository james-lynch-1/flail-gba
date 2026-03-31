#include "state.h"

// normal state functions

void enterNormal() {
    REG_DISPCNT |= DCNT_BG1;
    REG_BG1CNT = BG_PRIO(0) | BG_CBB(0) | BG_SBB(UI_SBB) | BG_4BPP | BG_REG_32x32; // ui
}

void updateNormal() {
    key_poll();
    if (gFrameCount & 1) {
        CounterComponent* power = getCounterByFlags(gPlayerId, COUNTER_POWER_FLAG);
        if (power->curr < power->max) {
            incDecCounter(power, -1);
            power->curr = clamp(power->curr, 0, power->max);
        }
    }

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

    deleteMarkedEnts();

    VBlankIntrWait();

    gFrameCount++;
}

void exitNormal(enum GameState state) {
    setGameState(state);
}
