#include "component.h"

ObjAffComponent* addComponentObjAff(s16 entId, u16 flags, bool isDblSize) {
    ObjComponent* o = getComponent(entId, COMP_OBJ);
    if (gNumCompsPerType[COMP_OBJ_AFF] >= MAX_OBJ_AFF_COMPONENTS || entId < 0 || !o) return NULL;
    ObjAffComponent oA = { {entId, flags}, gNumCompsPerType[COMP_OBJ_AFF] };
    ObjAffComponent* objAffComp = (ObjAffComponent*)addComponentCustom(&oA, COMP_OBJ_AFF);
    getObj(o)->attr0 |= ATTR0_AFF | (isDblSize << 9);
    getObj(o)->attr1 &= ~ATTR1_AFF_ID_MASK;
    getObj(o)->attr1 |= ATTR1_AFF_ID(oA.objAffIndex);
    o->header.flags &= ~ATTR0_MODE_MASK;
    o->header.flags |= ATTR0_AFF | (isDblSize << 9);
    return objAffComp;
}

void removeComponentObjAff(int entId) {
    ObjComponent* o = getComponent(entId, COMP_OBJ);
    if (o) {
        getObj(o)->attr0 &= ~ATTR0_MODE_MASK;
        o->header.flags &= ~ATTR0_MODE_MASK;
    }
    removeComponent(entId, COMP_OBJ_AFF);
}
