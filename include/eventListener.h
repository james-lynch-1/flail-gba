#ifndef EVENTLISTENER
#define EVENTLISTENER

#include "global.h"

#define E_PHYS_TOUCHED          1

bool addEventListener(enum ComponentType eventType, u32 flags, void(*callback)(int), bool removeAfterCallback);

void removeEventListener(enum ComponentType compType, int listenerIndex);

void notify(int entId, enum ComponentType compType, u32 flags);

#endif
