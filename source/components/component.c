#include "component.h"

ComponentHeader* addComponentCustom(int componentType, void* data) {
    if (componentType >= NUM_COMP_TYPES ||
        gNumCompsPerType[componentType] >= gMaxCompsPerType[componentType]) return NULL;
    ComponentHeader* dest = (void*)gDenseSetAddresses[componentType] + (int)gCompSizes[componentType] * gNumCompsPerType[componentType];
    memcpy32((void*)dest, data, gCompSizes[componentType] / 4);
    gCompSetSparse[componentType][((ComponentHeader*)data)->entIndex] = gNumCompsPerType[componentType]++;
    return dest;
}

// moves last component in list to the slot of the one being removed
// updates the corresponding entry in gCompSetSparse[componentType] to -1
// decrements gNumCompsPerType[componentType]
void removeComponent(int index, int componentType) {
    if (gCompSetSparse[componentType][index] == -1) return;
    int denseIndex = gCompSetSparse[componentType][index];
    gCompSetSparse[componentType][index] = -1;
    if (gNumCompsPerType[componentType] == 1) {
        gNumCompsPerType[componentType]--;
    }
    if (gNumCompsPerType[componentType] == 0) return;
    int replacementEntIndex = ((ComponentHeader*)(gDenseSetAddresses[componentType] +
        gCompSizes[componentType] * (gNumCompsPerType[componentType] - 1)))->entIndex;
    void* dst = (void*)(gDenseSetAddresses[componentType] + gCompSizes[componentType] * denseIndex);
    void* src = (void*)(gDenseSetAddresses[componentType] + gCompSizes[componentType] * (gNumCompsPerType[componentType] - 1));
    memcpy32(dst, src, gCompSizes[componentType] / 4);
    // memset32(src, 0, sizeof(ComponentHeader) / 4);
    gCompSetSparse[componentType][replacementEntIndex] = denseIndex;
    gNumCompsPerType[componentType]--;
}

void* getComponent(int index, int componentType) {
    return (void*)gDenseSetAddresses[componentType] + (int)gCompSizes[componentType] * gCompSetSparse[componentType][index];
}

void removeObjComponent(u32 entId) {
    ObjComponent* o = &gObjCompsDense[gCompSetSparse[COMP_OBJ][entId]];
    for (int i = 0; i < gNumSpritesAllocated; i++) {
        if (gSpriteAllocList[i].arrIndex == (o->obj->attr2 & ATTR2_ID_MASK)) {
            gSpriteAllocList[i].numUsing--;
            if (gSpriteAllocList[i].numUsing == 0) {
                deallocateObj(gSpriteAllocList[i].arrIndex);
                gSpriteAllocList[i] = gSpriteAllocList[gNumSpritesAllocated-- - 1];
            }
            break;
        }
    }
    o->obj->attr0 = 512;
    o->obj->attr1 = 0;
    o->obj->attr2 = 0;
    removeComponent(entId, COMP_OBJ);
}

void initialiseComponentArrays() {
    memset32(&gNumCompsPerType, 0, sizeof(gNumCompsPerType) / 4);
    for (int i = 0; i < NUM_COMP_TYPES; i++) {
        if (!gDenseSetAddresses[i]) break;
        memset32((void*)gDenseSetAddresses[i], 0, gCompSizes[i] / 4);
    }
    for (int i = 0; i < NUM_COMP_TYPES; i++) {
        memset32(&gCompSetSparse[i], 0, MAX_ENTS / 4);
    }
}

int reserveEntSlot() {
    if (gNumEnts == MAX_ENTS) return -1;
    gEntFlags[gNumEnts] = true;
    return gNumEnts++;
}

void markEntToBeDeleted(int index) {
    gEntsToDelete[gNumEntsToDelete++] = index;
}

int deleteEnt(int index) {
    if (gEntFlags[index] == 0) return 0;
    removeObjComponent(index);
    for (int i = 1; i < NUM_COMP_TYPES; i++) {
        removeComponent(index, i);
    }
    gNumEnts--;
    gEntFlags[index] = 0;
    return 1;
}

void deleteMarkedEnts() {
    int numToDelete = gNumEntsToDelete;
    for (int i = 0; i < numToDelete; i++) {
        deleteEnt(gEntsToDelete[i]);
    }
    gNumEntsToDelete = 0;
}
