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
    NULL
};

// functions

void loadEncounter(Encounter* encounter) {
    for (int i = 0; i < 16; i++) {
        memcpy32(&pal_obj_bank[i], (*(encounter->paletteSet))[i], 8);
    }
    loadBG(
        MAP_SBB,
        encounter->bgData->pal, encounter->bgData->palLen,
        encounter->bgData->tiles, encounter->bgData->tilesLen,
        encounter->bgData->map, encounter->bgData->mapLen
    );
}
