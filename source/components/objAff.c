#include "component.h"
#include "animations.h"

// todo: rotation by shearing:
// shear in X by -tan(angle/2)
// shear in Y by sin(angle)
// shear in X by -tan(angle/2)

// todo: add functions that add onto existing values rather than do offset from 1

// below transformations expect a 4.12 floating point number

void affRotate(OBJ_AFFINE* oAff, int val) {
    obj_aff_rotate(oAff, (1 << FIX_SHIFT) + val);
}

void affScaleX(OBJ_AFFINE* oAff, int val) {
    oAff->pa = clamp((1 << FIX_SHIFT) + (s16)(val >> 4), 0x80, 0x0180);
}

void affScaleY(OBJ_AFFINE* oAff, int val) {
    oAff->pd = clamp((1 << FIX_SHIFT) + (s16)(val >> 4), 0x80, 0x0180);
}

void affShearX(OBJ_AFFINE* oAff, int val) {
    obj_aff_shearx(oAff, (1 << FIX_SHIFT) + (s16)(val >> 4));
}

void affShearY(OBJ_AFFINE* oAff, int val) {
    obj_aff_sheary(oAff, (1 << FIX_SHIFT) + (s16)(val >> 4));
}

void (*transformFn[])(OBJ_AFFINE* oAff, int val) = {
    affRotate, affScaleX, affScaleY, affShearX, affShearY
};

ObjAffComponent* addComponentObjAff(s16 entId, u16 flags, bool isDblSize) {
    ObjComponent* o = getComponent(entId, COMP_OBJ);
    if (gNumCompsPerType[COMP_OBJ_AFF] >= MAX_OBJ_AFF_COMPONENTS || entId < 0 || !o) return NULL;
    ObjAffComponent oA = { {entId, flags} };
    getObj(o)->attr0 |= ATTR0_AFF | (isDblSize << 9);
    getObj(o)->attr1 &= ~ATTR1_AFF_ID_MASK;
    getObj(o)->attr1 |= ATTR1_AFF_ID(gNumCompsPerType[COMP_OBJ_AFF]);
    ObjAffComponent* objAffComp = (ObjAffComponent*)addComponentCustom(&oA, COMP_OBJ_AFF);
    o->header.flags &= ~ATTR0_MODE_MASK;
    o->header.flags |= ATTR0_AFF | (isDblSize << 9);
    return objAffComp;
}

void updateObjAffs() {
    for (int i = 0; i < numComps(COMP_OBJ_AFF); i++) {
        ObjAffComponent* objAff = &gObjAffCompsDense[i];
        OBJ_AFFINE* objAffBuf = getObjAff(objAff);

        // if it has an associated animation queue, use that to supply the flags
        if (objAff->animIndex != UINT8_MAX) {
            const Animation* anim = &gAnimations[objAff->animIndex];
            const AnimationStep* animStep = &(anim->animStep[objAff->currentStep]);
            if (objAff->framesElapsed >= animStep->length) {
                objAff->framesElapsed = 0;
                animStep = &(anim->animStep[++objAff->currentStep]);
                if (objAff->currentStep < anim->numSteps) {
                    objAff->header.flags = animStep->flags |
                        (animStep->lutStruct ? animStep->lutStruct->length : 0);
                }
                else {
                    finishAffineAnimation(objAff);
                    continue;
                }
            }
        }

        if (objAff->header.flags == 0) continue;

        bool isLutSize512 = (objAff->header.flags & OBJ_AFF_LUT_LRG) >> 9;
        int speed = objAff->framesElapsed * objAff->speed;
        int lutIndex = (objAff->header.flags & OBJ_AFF_LUT_SIZE_MASK) *
            ((speed >> isLutSize512) * (lu_div(60))) >> 16;
        int lutVal = objAff->lutStruct ?
            objAff->lutStruct->lut[lutIndex] :
            speed << 4;
        for (int i = 0; i < 5; i++) {
            if ((objAff->header.flags >> (i + 3)) & 1) {
                int negShift = i + 10;
                int negOrPos = 1 - ((objAff->header.flags >> negShift) & 0b10);
                transformFn[i](objAffBuf, lutVal* negOrPos);
            }
        }
        objAff->framesElapsed += objAff->speed; // todo: make speed have a fractional component
    }
}

void removeComponentObjAff(int entId) {
    ObjAffComponent* objAff = getComponent(entId, COMP_OBJ_AFF);
    if (!objAff) return;

    // set the associated obj's attrs back to regular mode
    ObjComponent* obj = getComponent(entId, COMP_OBJ);
    if (obj) {
        getObj(obj)->attr0 &= ~ATTR0_MODE_MASK;
        obj->header.flags &= ~ATTR0_MODE_MASK;
    }

    int replacementEntId = removeComponent(entId, COMP_OBJ_AFF);
    // if there's one objAff left, or it's the last objAff in the dense array, just clear its attrs
    if (replacementEntId == entId) {
        getObjAff(objAff)->pa = 0;
        getObjAff(objAff)->pb = 0;
        getObjAff(objAff)->pc = 0;
        getObjAff(objAff)->pd = 0;
        return;
    }

    // move the replacement objAff buffer attrs to their new location
    ObjAffComponent* replacementObjAff = getComponent(replacementEntId, COMP_OBJ_AFF);
    OBJ_AFFINE* replacementObjAffBufferPtr = getObjAff(replacementObjAff);
    getObjAff(objAff)->pa = replacementObjAffBufferPtr->pa;
    getObjAff(objAff)->pb = replacementObjAffBufferPtr->pb;
    getObjAff(objAff)->pc = replacementObjAffBufferPtr->pc;
    getObjAff(objAff)->pd = replacementObjAffBufferPtr->pd;

    // set the replacement obj's aff id to this slot
    ObjComponent* replacementObj = getComponent(replacementEntId, COMP_OBJ);
    getObj(replacementObj)->attr1 &= ~ATTR1_AFF_ID_MASK;
    getObj(replacementObj)->attr1 |= getObj(obj)->attr1 & ATTR1_AFF_ID_MASK;
    getObj(obj)->attr1 &= ~ATTR1_AFF_ID_MASK;
}
