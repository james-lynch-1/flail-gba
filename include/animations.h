#ifndef ANIMATIONS
#define ANIMATIONS

#include "global.h"
#include "component.h"

enum __attribute__ ((__packed__)) AffineAnimationEnum {
    AFF_ANIM_GROW_SHRINK,
    AFF_ANIM_GROW,
    AFF_ANIM_SHRINK,
    NUM_AFF_ANIMS
};

extern const Animation gAnimations[];

void addAffineAnimation(ObjAffComponent* objAff, int animIndex, int speed);

void finishAffineAnimation(ObjAffComponent* objAff);

#endif
