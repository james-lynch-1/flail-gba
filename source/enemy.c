#include "enemy.h"

int spawnEnemyWeak(int x, int y) {
    if ((numComps(COMP_OBJ) >= MAX_OBJ_COMPONENTS) ||
        (numComps(COMP_PHYSICS_SIMPLE) >= MAX_PHYSICS_SIMPLE_COMPONENTS) ||
        (numComps(COMP_AI_RAND) >= MAX_AI_RAND_COMPONENTS))
        return -1;
    s16 entId = reserveEntSlot();
    if (entId == -1) return -1;
    ObjComponent* objComp = addComponentObj(entId, 0, COMP_PHYSICS_SIMPLE);
    getObj(objComp)->attr0 |= ATTR0_4BPP;
    getObj(objComp)->attr2 |= ATTR2_ID(fetchSprite(sprite1Tiles, sprite1TilesLen)) | ATTR2_PALBANK(0);
    SimplePhysicsComponent phys = {
        {entId, 0},
        { (SWord)x, (SWord)y },
        (PhysArchetype*)&gPhysArchetypesStatic[ARCHETYPE_ENEMY_WEAK],
        {{0}, {0}}
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
    getObj(objComp)->attr0 |= ATTR0_4BPP | ATTR0_SQUARE;
    getObj(objComp)->attr1 |= ATTR1_SIZE_16x16;
    getObj(objComp)->attr2 |= ATTR2_ID(fetchSprite(magicBallTiles, magicBallTilesLen)) | ATTR2_PRIO(1) | ATTR2_PALBANK(1);
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

void spawnEnemyCentred() {
    spawnEnemy(120 << 16, 80 << 16);
}
