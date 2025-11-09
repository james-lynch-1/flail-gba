#ifndef MEM
#define MEM

#include "global.h"

int fetchSprite(const u16* tiles, int tilesLen);

int allocateObj(const u16 * tiles, int tilesLen);

void deallocateObj(int index);

void loadBG(int sbbIndex, const u16 * pal, int palLen, const u16 * tiles, int tilesLen, const u16 * map, int mapLen);

#endif
