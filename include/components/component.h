#ifndef COMPONENT
#define COMPONENT

#include "global.h"
#include "mem.h"

ComponentHeader* addComponentCustom(int componentType, void* data);

void removeComponent(int index, int componentType);

void* getComponent(int index, int componentType);

void removeObjComponent(u32 entId);

void initialiseComponentArrays();

int reserveEntSlot();

void markEntToBeDeleted(int index);

int deleteEnt(int index);

void deleteMarkedEnts();

#endif
