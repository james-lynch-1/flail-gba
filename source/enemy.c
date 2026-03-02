#include "enemy.h"

int spawnEnemyWeak(int x, int y) {
    if ((numComps(COMP_OBJ) >= MAX_OBJ_COMPONENTS) ||
        (numComps(COMP_PHYSICS_SIMPLE) >= MAX_SIMPLE_PHYSICS_COMPONENTS) ||
        (numComps(COMP_AI_RAND) >= MAX_AI_RAND_COMPONENTS)) {
        return -1;
    }
    s16 entId = reserveEntSlot();
    if (entId == -1) return -1;
    ObjComponent* objComp = addComponentObj(entId, 0, COMP_PHYSICS_SIMPLE);
    objComp->obj->attr0 |= ATTR0_4BPP;
    objComp->obj->attr2 |= ATTR2_ID(fetchSprite(sprite1Tiles, sprite1TilesLen)) | ATTR2_PALBANK(0);
    SimplePhysicsComponent phys = {
        {entId, 0},
        { (SWord)x, (SWord)y },
        (PhysArchetype*)&gPhysArchetypesStatic[ARCHETYPE_WEAK_ENEMY],
        {{0}, {0}},
        0xF000
    };
    AiRandComponent ai = { {entId, 0} };

    if (!objComp) return -1;
    if (!addComponentCustom(&phys, COMP_PHYSICS_SIMPLE)) return -1;
    if (!addComponentCustom(&ai, COMP_AI_RAND)) return -1;
    return entId;
}
