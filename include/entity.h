#ifndef ENTITY
#define ENTITY

#include "global.h"

int spawnEntity(int entityKind, int x, int y);

/** Returns -1 if no slot available */
int reserveEntSlot();

void markEntToBeDeleted(int index);

int deleteEnt(int index);

void deleteMarkedEnts();

#endif
