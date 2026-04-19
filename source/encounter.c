#include "encounter.h"

// BGs

BGData melee = {
    meleeSunsetBrightPal, meleeSunsetBrightPalLen,
    meleeSunsetBrightTiles, meleeSunsetBrightTilesLen,
    meleeSunsetBrightMap, meleeSunsetBrightMapLen
};

BGData gradient = {
    gradientPal, gradientPalLen,
    gradientTiles, gradientTilesLen,
    gradientMap, gradientMapLen
};

// palette sets

PaletteSet defaultPaletteSet = {
    magicBallYellowPal, sprite1Pal
};

// encounters

Encounter firstEncounter = {
    &gradient,
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

    // centre point
    int centreEntId = reserveEntSlot();
    addComponentSimplePhysics(centreEntId, 0, 120 << 16, 80 << 16, ARCHETYPE_NON_INTERACTABLE, 0, 0);
    ObjComponent* centreObj = addComponentObj(centreEntId, 0, COMP_PHYSICS_SIMPLE);
    getObj(centreObj)->attr2 = ATTR2_PALBANK(1) | fetchSprite(plusTiles, plusTilesLen) | ATTR2_PRIO(3);

    // player
    spawnPlayer(120 << 16, 80 << 16);

    // length
    if (encounter->length != -1) {
        s16 entId = reserveEntSlot();
        if (entId != -1)
            addComponentTimer(entId, 0, encounter->length, logSomething);
    }

    addEventListener(COMP_PHYSICS, E_STAR_COLLECTED, incrementPower, false);
    addEventListener(COMP_PHYSICS, E_STAR_COLLECTED, incrementHealthConditional, false);
    addEventListener(COMP_COUNTER, E_PLAYER_DIED, handlePlayerDied, false);
    addEventListener(COMP_COUNTER, E_POWER_FULL, handlePowerFull, false);

    if (hasComponent(gPlayerId, COMP_GROUP)) {
        GroupComponent* group = getComponent(gPlayerId, COMP_GROUP);
        for (int i = 0; i < group->numMembers; i++) {
            CounterComponent* counter = getComponent(group->memberIds[i], COMP_COUNTER);
            if (counter && (counter->header.flags & COUNTER_HEALTH_FLAG)) {
                addEventListener(COMP_PHYSICS, E_PHYS_TOUCHED, handlePlayerToPhysCollision, false);
                break;
            }
        }
    }
    spawnEnemy(10 << 16, 10 << 16);

    // spawn schedule. what is it?
    // do we define it statically here, or define a spawner object that can refer to a spawn schedule
    // then the object can be changed mid-level if we want
    PositionMini p = { 128, 70 };
    spawnStarLine(0, p, 60);
}
