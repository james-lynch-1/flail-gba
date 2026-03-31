#include "component.h"

// physics component archetypes
PhysArchetype gPhysArchetypesStatic[MAX_PHYS_ARCHETYPES_STATIC] = {
    { // player
    {14, 14, 0, 0},
    5,
    {0x00001000},
    SPEED_DECAY,
    true
    },
    { // weak enemy
    {8, 8, 0, 0},
    1,
    {0x00002000},
    0,
    true
    },
    { // enemy
    {16, 16, 0, 0},
    2,
    {0x00002000},
    0,
    true
    },
    { // item
    {8, 8, 0, 0},
    0,
    {0},
    0,
    true
    }
};
PhysArchetype gPhysArchetypesCustom[MAX_PHYS_ARCHETYPES_CUSTOM];

int addPhysArchetypeCustom(void* data) {
    if (gNumCustomPhysArchetypes == MAX_PHYS_ARCHETYPES_CUSTOM) return 0;
    memcpy32(&gPhysArchetypesCustom[gNumCustomPhysArchetypes++], data, sizeof(PhysArchetype) / 4);
    return 1;
}

void initialiseArchetypeArrays() {
    memset32(&gPhysArchetypesCustom, 0, sizeof(gPhysArchetypesCustom) / 4);
}
