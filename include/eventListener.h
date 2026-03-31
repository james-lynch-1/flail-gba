#ifndef EVENTLISTENER
#define EVENTLISTENER

#include "global.h"

enum __attribute__ ((__packed__)) EventFlags {
    E_NO_EVENT,
    E_PHYS_TOUCHED,
    E_STAR_COLLECTED
};

bool addEventListener(enum ComponentType eventType, u32 flags, void(*callback)(int entId), bool removeAfterCallback);

void removeEventListener(enum ComponentType compType, int listenerIndex);

void notify(int entId, enum ComponentType compType, u32 flags);

#endif
