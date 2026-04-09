#include "initialise.h"

void initialiseGame() {
    irq_init(NULL);
    irq_add(II_VBLANK, NULL);
    oam_init(gObjBuffer, 128);
    memset32(&gObjAllocArr, OBJ_SLOT_UNUSED, sizeof(gObjAllocArr) / 4);
    REG_DISPCNT = DCNT_MODE0 | DCNT_BG0 | DCNT_BG1 | DCNT_BG2 | DCNT_OBJ | DCNT_OBJ_1D;
    REG_BG0CNT = BG_PRIO(3) | BG_CBB(0) | BG_SBB(MAP_SBB) | BG_4BPP | BG_REG_32x32; // map

    memset32(&gNumListenersPerType, 0, sizeof(gNumListenersPerType) / 4);

    memset32(gTasks, 0, sizeof(gTasks) / 4);

    // components stuff
    initialiseComponentArrays();
    initialiseArchetypeArrays();

    memset32(gSpriteAllocList, 0, sizeof(gSpriteAllocList) / 4);
    memset32(&gEntFlags, 0, sizeof(gEntFlags) / 4);
    memset16(gCompSetSparse, -1, NUM_COMP_TYPES * MAX_ENTS);
    memset32(gEntsToDelete, 0, sizeof(gEntsToDelete) / 4);

    initialiseUi();
    loadEncounter(&firstEncounter);

    setGameState(NORMAL);

    gGameState.enterFunction();
}
