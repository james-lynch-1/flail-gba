#include "component.h"

void updateAi() {
    PhysicsComponent* playerPhysComp = getComponent(gPlayerId, COMP_PHYSICS);
    for (int i = 0; i < gNumCompsPerType[COMP_AI]; i++) {
        PhysicsComponent* physComp = getComponent(gAiCompsDense[i].header.entId, COMP_PHYSICS);
        if (gFlags & GFLAG_POWERED_UP)
            pushAwayFromPosition(physComp, playerPhysComp->pos.x.WORD, playerPhysComp->pos.y.WORD);
        else
            pullTowardsPosition(physComp, playerPhysComp->pos.x.WORD, playerPhysComp->pos.y.WORD);
    }
}

void removeComponentAi(int entId) {
    removeComponent(entId, COMP_AI);
}
