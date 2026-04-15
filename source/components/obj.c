#include "component.h"

ObjComponent* addComponentObj(s16 entId, u16 flags, int posSourceCompType) {
    if (gNumCompsPerType[COMP_OBJ] >= MAX_OBJ_COMPONENTS || entId < 0) return NULL;
    ObjComponent o = {
        {entId, flags},
        gNumCompsPerType[COMP_OBJ],
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
    int objIndex = objComp->objIndex;
    OBJ_ATTR* thisObjBufferPtr = getObj(objComp);
    stopUsingSprite(getObj(objComp)->attr2 & ATTR2_ID_MASK);

    // replace this one with the last obj in the buffer,
    // also update that obj's obj comp to point to this new location
    int replacementEntId = removeComponent(entId, COMP_OBJ);

    // if there's one obj left, or it's the last obj in the dense array, just clear its attrs
    if (replacementEntId == entId) {
        getObj(objComp)->attr0 = 512;
        getObj(objComp)->attr1 = 0;
        getObj(objComp)->attr2 = 0;
        return;
    }

    // move the replacement obj buffer attrs to their new location
    ObjComponent* replacementObj = getComponent(replacementEntId, COMP_OBJ);
    OBJ_ATTR* replacementObjBufferPtr = getObj(replacementObj);
    memcpy16(thisObjBufferPtr, replacementObjBufferPtr, 3);

    // finally, clear the replacement's old attrs
    getObj(replacementObj)->attr0 = 512;
    getObj(replacementObj)->attr1 = 0;
    getObj(replacementObj)->attr2 = 0;

    // make the replacement obj component point to this buffer location
    replacementObj->objIndex = objIndex;
}

// updates the pos based on the pos provided by corresponding comp of type posSourceCompType
void updateObjs() {
    for (int i = 0; i < gNumCompsPerType[COMP_OBJ]; i++) {
        ObjComponent* objComp = &gObjCompsDense[i];
        int posSourceCompType = objComp->posSourceCompType;
        PhysicsComponent* physComp = getComponent(objComp->header.entId, posSourceCompType);
        Position pos = *(Position*)((uint32_t)physComp + sizeof(ComponentHeader));
        const u8* sizes = obj_get_size(getObj(objComp));
        if (!in_range(pos.x.HALF.HI, 0 - sizes[0] / 2, SCREEN_WIDTH + sizes[1] / 2) ||
            !in_range(pos.y.HALF.HI, 0 - sizes[0] / 2, SCREEN_HEIGHT + sizes[1] / 2)) {
            getObj(objComp)->attr0 &= ~ATTR0_MODE_MASK;
            getObj(objComp)->attr0 |= ATTR0_HIDE;
        }
        else {
            getObj(objComp)->attr0 &= ~ATTR0_MODE_MASK;
            getObj(objComp)->attr0 |= objComp->header.flags & ATTR0_MODE_MASK;
        }
        bool isDbl = (((getObj(objComp)->attr0 & ATTR0_MODE_MASK) >> ATTR0_MODE_SHIFT) + 1) >> 2;
        getObj(objComp)->attr0 &= ~ATTR0_Y_MASK;
        getObj(objComp)->attr0 |= ATTR0_Y((pos.y.HALF.HI - (sizes[1] >> (1 - isDbl)))) & ATTR0_Y_MASK;
        getObj(objComp)->attr1 &= ~ATTR1_X_MASK;
        getObj(objComp)->attr1 |= ATTR1_X((pos.x.HALF.HI - (sizes[0] >> (1 - isDbl)))) & ATTR1_X_MASK;
    }
}
