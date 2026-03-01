#include "player.h"

int spawnPlayer(int x, int y) {
    int index = reserveEntSlot();
    if (index == -1) return -1;

    ObjComponent* o = addComponentObj(index, 0, COMP_PHYSICS);
    o->obj->attr0 = ATTR0_REG | ATTR0_Y(x >> 16);
    o->obj->attr1 = ATTR1_SIZE_16x16 | ATTR1_X(y >> 16);
    o->obj->attr2 = ATTR2_ID(fetchSprite(magicBallYellowTiles, magicBallYellowTilesLen)) | ATTR2_PALBANK(0);

    InputComponent input = { {index, 0}, handleInputNormal };

    PhysicsComponent phys = {
        {index, PHYS_GRAVITY_FLAG},
        {{x}, {y}},
        (PhysArchetype*)&gPhysArchetypesStatic[ARCHETYPE_PLAYER],
        { {0}, {0} },
        NULL,
        0
    };
    if (!addComponentCustom(&input, COMP_INPUT)) return -1;
    if (!addComponentCustom(&phys, COMP_PHYSICS)) return -1;
    return 1;
}
