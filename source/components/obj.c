#include "component.h"

ObjComponent* addComponentObj(s16 entId, u16 flags, int posSourceCompType) {
    if (gNumCompsPerType[COMP_OBJ] >= MAX_OBJ_COMPONENTS || entId < 0) return NULL;
    ObjComponent o = {
        {entId, 0},
        &gObjBuffer[gNumCompsPerType[COMP_OBJ]],
        posSourceCompType
    };
    ObjComponent* objComp = (ObjComponent*)addComponentCustom(&o, COMP_OBJ);
    return objComp;
}

void removeComponentObj(s16 entId) {
    ObjComponent* o = &gObjCompsDense[gCompSetSparse[COMP_OBJ][entId]];
    OBJ_ATTR* thisObjBufferPtr = o->obj;
    for (int i = 0; i < gNumSpritesAllocated; i++) {
        if (gSpriteAllocList[i].arrIndex == (o->obj->attr2 & ATTR2_ID_MASK)) {
            gSpriteAllocList[i].numUsing--;
            if (gSpriteAllocList[i].numUsing == 0) {
                deallocateObj(gSpriteAllocList[i].arrIndex);
                gSpriteAllocList[i] = gSpriteAllocList[gNumSpritesAllocated-- - 1];
            }
            break;
        }
    }
    
    // replace this one with the last obj in the buffer,
    // also update that obj's obj comp to point to this new location
    int replacementEntId = removeComponent(entId, COMP_OBJ);
    ObjComponent* replacementObj = getComponent(replacementEntId, COMP_OBJ);

    // if there is only one obj left, just set its buffer value to 0
    if (replacementEntId == entId) {
        replacementObj->obj->attr0 = 512;
        replacementObj->obj->attr1 = 0;
        replacementObj->obj->attr2 = 0;
        return;
    }

    // move the replacement obj buffer attrs to their new location
    OBJ_ATTR* replacementObjBufferPtr = replacementObj->obj;
    memcpy32(thisObjBufferPtr, replacementObjBufferPtr, 2);

    // finally, initialise the replacement's old obj buffer position to defaults
    replacementObj->obj->attr0 = 512;
    replacementObj->obj->attr1 = 0;
    replacementObj->obj->attr2 = 0;

    // make the replacement obj component point to this buffer location
    replacementObj->obj = thisObjBufferPtr;
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
