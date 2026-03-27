#include "encounter.h"

// BGs

BGData melee = {
    meleeSunsetBrightPal, meleeSunsetBrightPalLen,
    meleeSunsetBrightTiles, meleeSunsetBrightTilesLen,
    meleeSunsetBrightMap, meleeSunsetBrightMapLen
};

// palette sets

PaletteSet defaultPaletteSet = {
    magicBallYellowPal, sprite1Pal
};

// encounters

Encounter firstEncounter = {
    &melee,
    &defaultPaletteSet,
    -1,
    NULL,
    0,
    0
};

// functions

void loadEncounter(Encounter* encounter) {
    // palette
    for (int i = 0; i < 16; i++) {
        memcpy32(&pal_obj_bank[i], (*(encounter->paletteSet))[i], 8);
    }

    // bg
    loadBG(
        MAP_SBB,
        encounter->bgData->pal, encounter->bgData->palLen,
        encounter->bgData->tiles, encounter->bgData->tilesLen,
        encounter->bgData->map, encounter->bgData->mapLen
    );

    // player, ui
    spawnPlayer(120 << 16, 80 << 16);
    initialiseUi();

    // length
    if (encounter->length != -1) {
        s16 entId = reserveEntSlot();
        if (entId != -1)
            addComponentTimer(entId, 0, encounter->length, logSomething);
    }

    // music

    // objective

    addEventListener(COMP_PHYSICS, E_STAR_COLLECTED, logSomething, false);
    if (hasComponent(gPlayerId, COMP_GROUP)) {
        GroupComponent* group = getComponent(gPlayerId, COMP_GROUP);
        for (int i = 0; i < group->numMembers; i++) {
            CounterComponent* counter = getComponent(group->memberIds[i], COMP_COUNTER);
            if (counter && (counter->header.flags & COUNTER_HEALTH_FLAG)) {
                addEventListener(COMP_PHYSICS, E_PHYS_TOUCHED, decrementCounterHealth, false);
                break;
            }
        }
    }
    spawnEnemy(10 << 16, 10 << 16);

    // spawn schedule. what is it?
    // do we define it statically here, or define a spawner object that can refer to a spawn schedule
    // then the object can be changed mid-level if we want
    PositionMini p = { 110, 60 };
    addStarLine(0, p, 60);
}
