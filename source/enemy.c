#include "enemy.h"

int spawnEnemyWeak(int x, int y) {
    int entId = reserveEntSlot();
    if (entId == -1) return -1;
    ObjComponent* objComp = addObjComponent(entId, COMP_PHYSICS_SIMPLE);
    objComp->obj->attr0 |= ATTR0_4BPP;
    objComp->obj->attr2 |= ATTR2_ID(fetchSprite(sprite1Tiles, sprite1TilesLen)) | ATTR2_PALBANK(0);
    SimplePhysicsComponent phys = {
        {entId, COMP_PHYSICS_SIMPLE, 0},
        { (SWord)x, (SWord)y },
        (PhysArchetype*)&gPhysArchetypesStatic[ARCHETYPE_WEAK_ENEMY],
        {{0}, {0}},
        0xF000
    };
    AiRandComponent ai = { {entId, COMP_AI_RAND, 0} };

    if (!objComp) return -1;
    if (!addComponentCustom(COMP_PHYSICS_SIMPLE, &phys)) return -1;
    if (!addComponentCustom(COMP_AI_RAND, &ai)) return -1;
    return 1;
}
