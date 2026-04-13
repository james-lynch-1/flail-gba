#include "animations.h"

const Animation gAnimations[] = {
    { // AFF_ANIM_GROW_SHRINK
        {
            {&gLuts[LUT_SQRT], ANIM_SCALE_X_NEG | ANIM_SCALE_Y_NEG, 60},
            {&gLuts[LUT_SQRT], ANIM_SCALE_X | ANIM_SCALE_Y, 60}
        },
        2
    },
    { // AFF_ANIM_GROW
        {
            {&gLuts[LUT_SQRT], ANIM_SCALE_X_NEG | ANIM_SCALE_Y_NEG | ANIM_NO_RESET, 10},
        },
        1
    },
    { // AFF_ANIM_SHRINK
        {
            {&gLuts[LUT_SQRT], ANIM_SCALE_X | ANIM_SCALE_Y | ANIM_NO_RESET, 10},
        },
        1
    }
};

void addAffineAnimation(ObjAffComponent* objAff, int animIndex, int speed) {
    const Animation* anim = &gAnimations[animIndex];
    const AnimationStep* animStep = &(anim->animStep[0]);
    objAff->animIndex = animIndex;
    objAff->currentStep = 0;
    objAff->speed = speed;
    objAff->framesElapsed = 0;
    objAff->lutStruct = anim->animStep[0].lutStruct;

    objAff->header.flags = animStep->flags | (animStep->lutStruct ? animStep->lutStruct->length : 0);
}

void finishAffineAnimation(ObjAffComponent* objAff) {
    objAff->animIndex = UINT8_MAX;
    objAff->currentStep = UINT8_MAX;
    objAff->speed = 0;
    objAff->framesElapsed = 0;
    objAff->lutStruct = NULL;

    if (!(objAff->header.flags & ANIM_NO_RESET)) obj_aff_identity(getObjAff(objAff));

    objAff->header.flags = 0;
}
