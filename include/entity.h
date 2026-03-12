#ifndef ENTITY
#define ENTITY

#include "global.h"

int spawnEntity(int entityKind, int x, int y);

/** Returns -1 if no slot available */
int reserveEntSlot();

/** Marks an ent to be deleted at the end of the frame.
 * CAUTION: Don't do anything with an ent that has been deleted!
 * If necessary, check if it's been marked first!
 * ZZZ will be fixed
 */
void markEntToBeDeleted(int index);

int deleteEnt(int index);

void deleteMarkedEnts();

#endif
