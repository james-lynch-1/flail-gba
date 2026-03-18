#include "component.h"

void updateAi() {
    PhysicsComponent* playerPhysComp = getComponent(gPlayerId, COMP_PHYSICS);
    for (int i = 0; i < gNumCompsPerType[COMP_AI]; i++) {
        PhysicsComponent* physComp = getComponent(
            gAiCompsDense[i].header.entId,
            COMP_PHYSICS
        );
        pullTowardsPosition(physComp, playerPhysComp->pos.x.WORD, playerPhysComp->pos.y.WORD);
    }
}

void removeComponentAi(int entId) {
    removeComponent(entId, COMP_AI);
}
