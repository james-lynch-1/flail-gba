#include "component.h"

ObjComponent* addComponentObj(s16 entId, u16 flags, int posSourceCompType) {
    if (gNumCompsPerType[COMP_OBJ] > MAX_OBJ_COMPONENTS || entId > INT16_MAX) return NULL;
    ObjComponent o = {
        {entId, 0},
        &gObjBuffer[gNumCompsPerType[COMP_OBJ]],
        posSourceCompType
    };
    ObjComponent* objComp = (ObjComponent*)addComponentCustom(&o, COMP_OBJ);
    return objComp;
}

// updates the pos based on the pos provided by corresponding comp of type posSourceCompType
void updateObjs() {
    for (int i = 0; i < gNumCompsPerType[COMP_OBJ]; i++) {
        int posSourceCompType = gObjCompsDense[i].posSourceCompType;
        ComponentHeader* physComp = (ComponentHeader*)(denseSetAddr(posSourceCompType) +
            gCompSetSparse[posSourceCompType][gObjCompsDense[i].header.entId] * compSize(posSourceCompType));
        PhysArchetype arch = **(PhysArchetype**)((uint32_t)physComp + sizeof(ComponentHeader) + sizeof(Position));
        Position pos = *(Position*)((uint32_t)physComp + sizeof(ComponentHeader));

        if (!in_range(pos.x.HALF.HI, 0 - arch.hitbox.width / 2, SCREEN_WIDTH + arch.hitbox.height / 2) ||
            !in_range(pos.y.HALF.HI, 0 - arch.hitbox.width / 2, SCREEN_HEIGHT + arch.hitbox.height / 2))
            gObjCompsDense[i].obj->attr0 |= ATTR0_HIDE;
        else
            gObjCompsDense[i].obj->attr0 &= ~ATTR0_AFF_DBL;

        gObjCompsDense[i].obj->attr0 &= ~ATTR0_Y_MASK;
        gObjCompsDense[i].obj->attr0 |= ATTR0_Y(pos.y.HALF.HI - arch.hitbox.height / 2) & ATTR0_Y_MASK;
        gObjCompsDense[i].obj->attr1 &= ~ATTR1_X_MASK;
        gObjCompsDense[i].obj->attr1 |= ATTR1_X(pos.x.HALF.HI - arch.hitbox.width / 2) & ATTR1_X_MASK;
    }
}
