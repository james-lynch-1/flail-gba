#include "component.h"

void updateAi() {
    PhysicsComponent* playerPhysComp = getComponent(gPlayerId, COMP_PHYSICS);
    CounterComponent* powerComp = getCounterByFlags(gPlayerId, COUNTER_POWER_FLAG);
    for (int i = 0; i < gNumCompsPerType[COMP_AI]; i++) {
        PhysicsComponent* physComp = getComponent(gAiCompsDense[i].header.entId, COMP_PHYSICS);
        if (powerComp && powerComp->curr == powerComp->max)
            pushAwayFromPosition(physComp, playerPhysComp->pos.x.WORD, playerPhysComp->pos.y.WORD);
        else
            pullTowardsPosition(physComp, playerPhysComp->pos.x.WORD, playerPhysComp->pos.y.WORD);
    }
}

void removeComponentAi(int entId) {
    removeComponent(entId, COMP_AI);
}
