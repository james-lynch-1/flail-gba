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
    // length
    if (encounter->length != -1) {
        s16 entId = reserveEntSlot();
        if (entId != -1) {
            TimerComponent t = { {entId, 0}, encounter->length * 60, logSomething };
            addComponentCustom(&t, COMP_TIMER);
        }
    }
    // music

    // objective

    // spawn schedule. what is it?
    // do we define it statically here, or define a spawner object that can refer to a spawn schedule
    // then the object can be changed mid-level if we want
    PositionMini p = { 0, 0 };
    addStarLine(0, p, 60);
}
