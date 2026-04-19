#include "component.h"

// physics component archetypes
PhysArchetype gPhysArchetypesStatic[MAX_PHYS_ARCHETYPES_STATIC] = {
    { // ARCHETYPE_PLAYER
        {10, 10, 0, 0},
        5,
        {0x00001000},
        SPEED_DECAY,
        true
    },
    { // ARCHETYPE_ENEMY_WEAK
        {8, 8, 0, 0},
        1,
        {0x00002000},
        0,
        true
    },
    { // ARCHETYPE_ENEMY
        {16, 16, 0, 0},
        2,
        {0x00002000},
        0,
        true
    },
    { // ARCHETYPE_ENEMY_DEAD
        {16, 16, 0, 0},
        64,
        {0x00002000},
        0,
        true
    },
    { // ARCHETYPE_ITEM
        {12, 16, 0, 0},
        0,
        {0},
        0xFFFF,
        true
    },
    { // ARCHETYPE_NON_INTERACTABLE
        {0, 0, 0, 0},
        0,
        {0},
        0xFFFF,
        true
    },
    { // ARCHETYPE_PARTICLE
        {2, 2, 0, 0},
        1,
        {0x2000},
        0x400,
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
