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

void removeComponentObj(int entId) {
    ObjComponent* objComp = getComponent(entId, COMP_OBJ);
    if (!objComp) return;
#ifdef DEBUG
    removeComponentDebugBlob(entId);
#endif
    OBJ_ATTR* thisObjBufferPtr = objComp->obj;
    stopUsingSprite(objComp->obj->attr2 & ATTR2_ID_MASK);

    // replace this one with the last obj in the buffer,
    // also update that obj's obj comp to point to this new location
    int replacementEntId = removeComponent(entId, COMP_OBJ);
    ObjComponent* replacementObj = getComponent(replacementEntId, COMP_OBJ);

    // if there is only one obj left, or if it is the last obj in the dense array, just clear its attrs
    if (replacementEntId == entId) {
        objComp->obj->attr0 = 512;
        objComp->obj->attr1 = 0;
        objComp->obj->attr2 = 0;
        return;
    }

    // move the replacement obj buffer attrs to their new location
    OBJ_ATTR* replacementObjBufferPtr = replacementObj->obj;
    memcpy32(thisObjBufferPtr, replacementObjBufferPtr, 2);

    // finally, clear the replacement's old attrs
    replacementObj->obj->attr0 = 512;
    replacementObj->obj->attr1 = 0;
    replacementObj->obj->attr2 = 0;

    // make the replacement obj component point to this buffer location
    replacementObj->obj = thisObjBufferPtr;
}

// updates the pos based on the pos provided by corresponding comp of type posSourceCompType
void updateObjs() {
    for (int i = 0; i < gNumCompsPerType[COMP_OBJ]; i++) {
        ObjComponent* objComp = &gObjCompsDense[i];
        int posSourceCompType = objComp->posSourceCompType;
        PhysicsComponent* physComp = getComponent(objComp->header.entId, posSourceCompType);
        Position pos = *(Position*)((uint32_t)physComp + sizeof(ComponentHeader));
        const u8* sizes = obj_get_size(objComp->obj);
        if (!in_range(pos.x.HALF.HI, 0 - sizes[0] / 2, SCREEN_WIDTH + sizes[1] / 2) ||
            !in_range(pos.y.HALF.HI, 0 - sizes[0] / 2, SCREEN_HEIGHT + sizes[1] / 2))
            gObjCompsDense[i].obj->attr0 |= ATTR0_HIDE;
        else
            gObjCompsDense[i].obj->attr0 &= ~ATTR0_AFF_DBL;

        objComp->obj->attr0 &= ~ATTR0_Y_MASK;
        objComp->obj->attr0 |= ATTR0_Y(pos.y.HALF.HI - sizes[1] / 2) & ATTR0_Y_MASK;
        objComp->obj->attr1 &= ~ATTR1_X_MASK;
        objComp->obj->attr1 |= ATTR1_X(pos.x.HALF.HI - sizes[0] / 2) & ATTR1_X_MASK;
    }
}
