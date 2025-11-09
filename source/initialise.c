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
    initialisePlayerComps();

    // initialisePlayer();
    // initialiseEntities();
    loadEncounter(&firstEncounter);
    // loadBG(MAP_SBB, gradientPal, gradientPalLen, gradientTiles, gradientTilesLen, gradientMap, gradientMapLen);
    // ui:
    // loadBG(UI_SBB, uiLayerPal, uiLayerPalLen, uiLayerTiles, uiLayerTilesLen, uiLayerMap, uiLayerMapLen);
    loadSprites();

    setGameState(NORMAL);

    gGameState.enterFunction();
}

void initialisePlayer() {
    PlayerEnt p = {
        &gObjBuffer[127], &gObjAffBuffer[0],
        {{120 << 16}, {80 << 16}},
        {12, 12, 0, 0},
        20,
        5, // radius
        {0x5000}, // accel
        {{0}, {0}}, // vector
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
    };
    gPlayer = p;
    // initialise player obj
    gObjBuffer[127].attr0 = ATTR0_REG;
    gObjBuffer[127].attr1 = ATTR1_SIZE_16x16;
    gObjBuffer[127].attr2 = ATTR2_ID(0);
}

void initialiseEntities() {
    gEntities = allocEntities();
    gFirstAvailableEntSlot = gEntities;

    Entity enemy = {
        {{{0}, {0}}},
        {8, 8, 0, 0},
        &gObjBuffer[1], NULL,
        WEAK_TURN_FREQ,
        1, // health
        1, // radius
        {0x1000}, // accel
        {{0}, {0}}, // vector
        0,
        0,
        0,
        0,
        0, // flags
        ILK_ENEMY,
        0, // animationState
        0, // animFrames
    };

    for (int i = 0; i < MAX_ENTS; i++) {
        memcpy32(&gEntities[i], &enemy, sizeof(Entity) / 4);
        gEntities[i].state.next = &gEntities[i + 1];
        gEntities[i].obj = &gObjBuffer[i];
        gEntities[i].obj->attr2 = ATTR2_ID(4);
    }
    gEntities[MAX_ENTS - 1].state.next = NULL;
}

Entity* allocEntities() {
    Entity* ents = malloc(sizeof(Entity) * MAX_ENTS);
    return ents;
}

void loadSprites() {
    // memcpy32(&tile_mem_obj[0], magicBallYellowTiles, magicBallYellowTilesLen / sizeof(u32));
    // gPlayer.obj->attr2 &= ~ATTR2_ID_MASK;
    // gPlayer.obj->attr2 |= allocateObj(magicBallYellowTiles, magicBallYellowTilesLen);
    // memcpy32(pal_obj_bank[0], magicBallYellowPal, magicBallYellowPalLen / sizeof(u32));

    // memcpy32(pal_obj_bank[1], sprite1Pal, sprite1PalLen / sizeof(u32));
    // memcpy32(&tile_mem[4][8], sprite2Tiles, sprite2TilesLen / sizeof(u32));
}
