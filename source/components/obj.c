#include "obj.h"

ObjComponent* addObjComponent(u32 entId, int posSourceCompType) {
    if (gNumCompsPerType[COMP_OBJ] > MAX_OBJ_COMPONENTS || entId > UINT16_MAX) return NULL;
    ObjComponent o = {
        {entId, COMP_OBJ, 0},
        &gObjBuffer[gNumCompsPerType[COMP_OBJ]],
        posSourceCompType
    };
    ObjComponent* objComp = (ObjComponent*)addComponentCustom(COMP_OBJ, &o);
    return objComp;
}

// updates the pos based on the pos provided by corresponding comp of type posSourceCompType
void updateObjs() {
    for (int i = 0; i < gNumCompsPerType[COMP_OBJ]; i++) {
        int posSourceCompType = gObjCompsDense[i].posSourceCompType;
        ComponentHeader* physComp = (ComponentHeader*)(gDenseSetAddresses[posSourceCompType] +
            gCompSetSparse[posSourceCompType][gObjCompsDense[i].header.entIndex] * gCompSizes[posSourceCompType]);
        PhysArchetype arch = **(PhysArchetype**)((int)physComp + sizeof(ComponentHeader) + sizeof(Position));
        Position pos = *(Position*)((int)physComp + sizeof(ComponentHeader));

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
