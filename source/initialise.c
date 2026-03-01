#include "initialise.h"

void initialiseGame() {
    irq_init(NULL);
    irq_add(II_VBLANK, NULL);
    oam_init(gObjBuffer, 128);
    memset32(&gObjAllocArr, OBJ_SLOT_UNUSED, sizeof(gObjAllocArr) / 4);
    REG_DISPCNT = DCNT_MODE0 | DCNT_BG0 | DCNT_OBJ | DCNT_OBJ_1D;
    REG_BG0CNT = BG_PRIO(3) | BG_CBB(0) | BG_SBB(MAP_SBB) | BG_4BPP | BG_REG_32x32; // map

    // components stuff
    initialiseComponentArrays();
    initialiseArchetypeArrays();

    memset32(gSpriteAllocList, 0, sizeof(gSpriteAllocList) / 4);
    memset32(&gEntFlags, 0, sizeof(gEntFlags) / 4);
    memset16(gCompSetSparse, -1, NUM_COMP_TYPES * MAX_ENTS);
    memset32(gEntsToDelete, 0, sizeof(gEntsToDelete) / 4);
    spawnPlayer(120 << 16, 80 << 16);

    loadEncounter(&firstEncounter);
    // loadBG(MAP_SBB, gradientPal, gradientPalLen, gradientTiles, gradientTilesLen, gradientMap, gradientMapLen);
    // ui:
    // loadBG(UI_SBB, uiLayerPal, uiLayerPalLen, uiLayerTiles, uiLayerTilesLen, uiLayerMap, uiLayerMapLen);
    loadSprites();

    setGameState(NORMAL);

    gGameState.enterFunction();
}

void loadSprites() {
    // memcpy32(&tile_mem_obj[0], magicBallYellowTiles, magicBallYellowTilesLen / sizeof(u32));
    // gPlayer.obj->attr2 &= ~ATTR2_ID_MASK;
    // gPlayer.obj->attr2 |= allocateObj(magicBallYellowTiles, magicBallYellowTilesLen);
    // memcpy32(pal_obj_bank[0], magicBallYellowPal, magicBallYellowPalLen / sizeof(u32));

    // memcpy32(pal_obj_bank[1], sprite1Pal, sprite1PalLen / sizeof(u32));
    // memcpy32(&tile_mem[4][8], sprite2Tiles, sprite2TilesLen / sizeof(u32));
}
