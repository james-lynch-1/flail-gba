#include "enemy.h"

int spawnEnemyWeak(int x, int y) {
    if ((numComps(COMP_OBJ) >= MAX_OBJ_COMPONENTS) ||
        (numComps(COMP_PHYSICS_SIMPLE) >= MAX_PHYSICS_SIMPLE_COMPONENTS) ||
        (numComps(COMP_AI_RAND) >= MAX_AI_RAND_COMPONENTS))
        return -1;
    s16 entId = reserveEntSlot();
    if (entId == -1) return -1;
    ObjComponent* objComp = addComponentObj(entId, 0, COMP_PHYSICS_SIMPLE);
    objComp->obj->attr0 |= ATTR0_4BPP;
    objComp->obj->attr2 |= ATTR2_ID(fetchSprite(sprite1Tiles, sprite1TilesLen)) | ATTR2_PALBANK(0);
    SimplePhysicsComponent phys = {
        {entId, 0},
        { (SWord)x, (SWord)y },
        (PhysArchetype*)&gPhysArchetypesStatic[ARCHETYPE_ENEMY_WEAK],
        {{0}, {0}},
        0xF000
    };
    AiRandComponent ai = { {entId, 0} };
    HitboxComponent hBox = { {entId, 0}, {8, 8, 0, 0} };

    addComponentCustom(&hBox, COMP_HITBOX);
    addComponentCustom(&phys, COMP_PHYSICS_SIMPLE);
    addComponentCustom(&ai, COMP_AI_RAND);
    return entId;
}

int spawnEnemy(int x, int y) {
    if ((numComps(COMP_OBJ) >= MAX_OBJ_COMPONENTS) ||
        (numComps(COMP_PHYSICS) >= MAX_PHYSICS_COMPONENTS) ||
        (numComps(COMP_AI) >= MAX_AI_COMPONENTS))
        return -1;
    s16 entId = reserveEntSlot();
    if (entId == -1) return -1;
    ObjComponent* objComp = addComponentObj(entId, 0, COMP_PHYSICS);
    objComp->obj->attr0 |= ATTR0_4BPP | ATTR0_SQUARE;
    objComp->obj->attr1 |= ATTR1_SIZE_16x16;
    objComp->obj->attr2 |= ATTR2_ID(fetchSprite(magicBallTiles, magicBallTilesLen)) | ATTR2_PALBANK(1);
    PhysicsComponent phys = {
        {entId, 0},
        { (SWord)x, (SWord)y },
        (PhysArchetype*)&gPhysArchetypesStatic[ARCHETYPE_ENEMY],
        {{0}, {0}},
        0xF000
    };
    AiComponent ai = { {entId, 0} };

    addComponentCustom(&phys, COMP_PHYSICS);
    addComponentCustom(&ai, COMP_AI);
    return entId;
}
