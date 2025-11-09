#include "player.h"

int initialisePlayerComps() {
    int index = reserveEntSlot();
    if (index == -1) return -1;

    ObjComponent* o = addObjComponent(index, COMP_PHYSICS);
    o->obj->attr0 = ATTR0_REG | ATTR0_Y(80);
    o->obj->attr1 = ATTR1_SIZE_16x16 | ATTR1_X(120);
    o->obj->attr2 = ATTR2_ID(fetchSprite(magicBallYellowTiles, magicBallYellowTilesLen)) | ATTR2_PALBANK(0);

    InputComponent input = { {index, COMP_INPUT, 0}, handleInputNormal };

    PhysicsComponent phys = {
        {index, COMP_PHYSICS, PHYS_GRAVITY_FLAG},
        {{120 << 16}, {80 << 16}},
        (PhysArchetype*)&gPhysArchetypesStatic[ARCHETYPE_PLAYER],
        { {0}, {0} },
        0
    };
    if (!addComponentCustom(COMP_INPUT, &input)) return -1;
    if (!addComponentCustom(COMP_PHYSICS, &phys)) return -1;
    return 1;
}
