#include "entity.h"

int spawnEntity(int entityKind, int x, int y) {
    return gEntSpawners[entityKind](x, y);
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
    // ZZZ have to update this for every new component you add
    if (gEntFlags[index] == 0) return 0;
    for (int i = 0; i < NUM_COMP_TYPES; i++) {
        void(*delFn)(int);
        delFn = (void(*)(int))gCompTable[i][COMP_REMOVERS];
        delFn(index);
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

