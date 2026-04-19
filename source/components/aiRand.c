#include "component.h"

void updateAiRand() {
    for (int i = 0; i < gNumCompsPerType[COMP_AI_RAND]; i++) {
        SimplePhysicsComponent* physComp = getComponent(
            gAiRandCompsDense[i].header.entId,
            COMP_PHYSICS_SIMPLE
        );
        u16 angle = ArcTan2(physComp->vec.x.HALF.HI, physComp->vec.y.HALF.HI);
        angle += qran_range(-1, 2) * qran_range(0x1000, 0x2000);
        physComp->vec.x.WORD = lu_cos(angle) << 16;
        physComp->vec.y.WORD = -lu_sin(angle) << 16;
    }
}

void removeComponentAiRand(int entId) {
    removeComponent(entId, COMP_AI_RAND);
}
