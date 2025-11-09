#include "state.h"
int palOffset = 0;

void setGameState(GameStateEnum state) {
    switch (state) {
        case NORMAL:
            gGameState.enterFunction = enterNormal;
            gGameState.updateFunction = updateNormal;
            gGameState.exitFunction = exitNormal;
            break;
        case PAUSE:
            gGameState.enterFunction = enterPause;
            gGameState.updateFunction = updatePause;
            gGameState.exitFunction = exitPause;
            break;
        case TITLE:
            gGameState.enterFunction = enterTitle;
            gGameState.updateFunction = updateTitle;
            gGameState.exitFunction = exitTitle;
            break;
    }
    gGameState.enterFunction();
}

// normal state functions

void enterNormal() {
    REG_DISPCNT |= DCNT_BG1 | DCNT_WINOBJ;
    se_mem[UI_SBB][244] |= SE_HFLIP;
    // outside the obj, bg0 is enabled. inside, other objs are enabled.
    REG_WINOUT = (WIN_BG0 | WIN_BG1 | WIN_OBJ) | ((WIN_OBJ | WIN_BG0) << 8);

    REG_BG1CNT = BG_PRIO(3) | BG_CBB(0) | BG_SBB(UI_SBB) | BG_4BPP | BG_REG_32x32;
}

void updateNormal() {
    key_poll();
    // old:----------------------------------------------------------------------------------
    // if (key_hit(KEY_START)) {
    //     exitNormal(PAUSE);
    //     return;
    // }
    // if (key_hit(KEY_SELECT)) {
    //     gFlags ^= GFLAG_GRAVITY;
    //     gPlayer.obj->attr2 ^= ATTR2_PALBANK(1);
    // }
    // if (key_hit(KEY_A))
    //     for (int i = 0; i < 20; i++)
    //         spawnEnemy(gPlayer.position.x.HALF.HI, gPlayer.position.y.HALF.HI, ILK_ENEMY);
    // if (key_hit(KEY_B))
    //     spawnEnemy(gPlayer.position.x.HALF.HI, gPlayer.position.y.HALF.HI, ILK_ENEMY);
    // if (key_is_down(KEY_L | KEY_R)) {
    //     // palOffset += key_tri_shoulder();
    //     // memcpy16((void*)MEM_PAL_BG, &gradientPal[palOffset], gradientPalLen / 2);
    //     healthBallRot -= 0x100 * key_tri_shoulder();
    //     // obj_aff_rotate(&gObjAffBuffer[31], healthBallRot);
    //     obj_aff_rotscale(&gObjAffBuffer[31], 0xE8, 0xE8, healthBallRot);
    // }
    // // updatePlayerPos(&gPlayer);
    // updateEntsNormal();
    // updateUINormal();

    updateInputComps();
    VBlankIntrWait();
    updateAiRand();
    updatePhysics();
    updatePhysicsSimple();
    updateObjs();
    deleteMarkedEnts();
    gFrameCount++;
}

void exitNormal(GameStateEnum state) {
    setGameState(state);
}

// pause state functions

void enterPause() {
    REG_DISPCNT &= ~(DCNT_OBJ | DCNT_BG1);
}

void updatePause() {
    key_poll();
    if (key_hit(KEY_START)) {
        exitPause(NORMAL);
        return;
    }
    VBlankIntrWait();
}

void exitPause(GameStateEnum state) {
    REG_DISPCNT |= DCNT_OBJ | DCNT_BG1;
    setGameState(state);
}

// title state functions

void enterTitle() {

}

void updateTitle() {

}

void exitTitle(GameStateEnum state) {
    setGameState(state);
}
