#include "archetypes.h"

int addPhysArchetypeCustom(void* data) {
    if (gNumCustomPhysArchetypes == MAX_PHYS_ARCHETYPES_CUSTOM) return 0;
    memcpy32(&gPhysArchetypesCustom[gNumCustomPhysArchetypes++], data, sizeof(PhysArchetype) / 4);
    return 1;
}

void initialiseArchetypeArrays() {
    memset32(&gPhysArchetypesCustom, 0, sizeof(gPhysArchetypesCustom) / 4);
}
