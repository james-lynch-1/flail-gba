#include "mem.h"

// returns index of sprite data in gObjAllocArr, allocating a new one if not found
int fetchSprite(const u16* tiles, int tilesLen) {
    for (int i = 0; i < gNumSpritesAllocated; i++) {
        if (gSpriteAllocList[i].tileData == tiles) {
            gSpriteAllocList[i].numUsing++;
            return gSpriteAllocList[i].arrIndex;
        }
    }
    int index = allocateObj(tiles, tilesLen);
    if (index == -1) return -1;
    SpriteAllocList alloc = { tiles, index, 1 };
    gSpriteAllocList[gNumSpritesAllocated++] = alloc;
    return index;
}

// https://www.gamedeveloper.com/programming/gameboy-advance-resource-management
// 'OBJ Sprite Memory Management'
// returns -1 if unsuccessful, returns index in allocation array if successful
int allocateObj(const u16* tiles, int tilesLen) {
    if (tilesLen < 32 || tilesLen > 2048 || (tilesLen & (tilesLen - 1)) != 0) return -1;
    int numBlocksReqd = tilesLen / 32;
    bool found = false;
    u32 index = 0;
    for (int i = 0; i < 1024; i += numBlocksReqd) {
        for (int j = i; j < i + numBlocksReqd; j++) {
            if (gObjAllocArr[j] != OBJ_SLOT_UNUSED) {
                found = false;
                break;
            }
            found = true;
        }
        if (found) {
            index = i;
            gObjAllocArr[i] = OBJ_SLOT_USED;
            for (i++; i < numBlocksReqd; i++)
                gObjAllocArr[i] = OBJ_SLOT_CONTINUE;
            memcpy32(MEM_VRAM_OBJ + (void*)(index * 32), tiles, tilesLen / 4);
            return index;
        }
    }
    return -1;
}

void deallocateObj(int index) {
    int i = index;
    gObjAllocArr[i++] = OBJ_SLOT_UNUSED;
    while (gObjAllocArr[i] == OBJ_SLOT_CONTINUE)
        gObjAllocArr[i++] = OBJ_SLOT_UNUSED;
}

void loadBG(int sbbIndex, const u16* pal, int palLen, const u16* tiles, int tilesLen, const u16* map, int mapLen) {
    memcpy16(pal_bg_mem, pal, palLen / sizeof(u16));
    memcpy32(&tile_mem[0][0], tiles, tilesLen / sizeof(u32));
    int srcWInTiles = 32;
    for (int row = 0; row < 20; row++)
        memcpy32(&se_mem[sbbIndex][row * SBB_WIDTH_T], &map[row * srcWInTiles], SBB_WIDTH_T / 2);
}
