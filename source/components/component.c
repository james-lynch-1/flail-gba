#include "component.h"

ComponentHeader* addComponentCustom(void* data, ComponentType componentType) {
    if (componentType >= NUM_COMP_TYPES ||
        gNumCompsPerType[componentType] >= maxComps(componentType))
        return NULL;
    ComponentHeader* dest = (void*)denseSetAddr(componentType) + compSize(componentType) * gNumCompsPerType[componentType];
    memcpy32((void*)dest, data, compSize(componentType) / 4);
    gCompSetSparse[componentType][((ComponentHeader*)data)->entId] = gNumCompsPerType[componentType]++;
    return dest;
}

void removeComponent(int index, int componentType) {
    if (gCompSetSparse[componentType][index] == -1) return; // in case component doesn't exist
    int denseIndex = gCompSetSparse[componentType][index];
    gCompSetSparse[componentType][index] = -1;
    if (gNumCompsPerType[componentType] == 1) {
        gNumCompsPerType[componentType]--;
    }
    if (gNumCompsPerType[componentType] == 0) return;
    int replacementEntId = ((ComponentHeader*)(denseSetAddr(componentType) +
        compSize(componentType) * (gNumCompsPerType[componentType] - 1)))->entId;
    void* dst = (void*)(denseSetAddr(componentType) + compSize(componentType) * denseIndex);
    // void* dst = (void*)(gCompTable[componentType][COMP_DSET_ADDRESSES] + compSize[componentType] * denseIndex);
    void* src = (void*)(denseSetAddr(componentType) + compSize(componentType) * (gNumCompsPerType[componentType] - 1));
    memcpy32(dst, src, compSize(componentType) / 4);
    // memset32(src, 0, sizeof(ComponentHeader) / 4);
    gCompSetSparse[componentType][replacementEntId] = denseIndex;
    gNumCompsPerType[componentType]--;
}

bool hasComponent(s16 entId, int componentType) {
    int index = gCompSetSparse[componentType][entId];
    return index != -1;
}

void* getComponent(s16 entId, int componentType) {
    if (hasComponent(entId, componentType))
        return denseSetAddr(componentType) + compSize(componentType) * gCompSetSparse[componentType][entId];
    return NULL;
}

void* getComponentFromDenseIndex(int denseIndex, int componentType) {
    return denseSetAddr(componentType) + compSize(componentType) * denseIndex;
}

void removeObjComponent(s16 entId) {
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

ComponentType getComponentType(ComponentHeader* compPtr) {
    for (int i = 0; i < NUM_COMP_TYPES; i++)
        if ((uint32_t)compPtr >= (uint32_t)denseSetAddr(i))
            return i;
    return -1;
}

void initialiseComponentArrays() {
    memset32(&gNumCompsPerType, 0, sizeof(gNumCompsPerType) / 4);
    for (int i = 0; i < NUM_COMP_TYPES; i++) {
        if (!denseSetAddr(i)) break;
        // memset32((void*)denseSetAddr[i], 0, compSize[i] / 4);
        // make the index from the dense set comp to the 
        for (int j = 0; j < maxComps(i); j++) {
            ComponentHeader* compHeader = denseSetAddr(i) + compSize(i) * j;
            compHeader->entId = -1;
        }
        // -1 means there is no comp of this type for this ent
        for (int j = 0; j < MAX_ENTS; j++)
            gCompSetSparse[i][j] = -1;
    }
}

int reserveEntSlot() {
    if (gNumEnts == MAX_ENTS) return -1;
    int i = 0;
    while (gEntFlags[i] != 0) i++;
    gEntFlags[i] = true;
    gNumEnts++;
    return i;
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
