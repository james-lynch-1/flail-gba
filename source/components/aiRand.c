#include "component.h"

void updateAiRand() {
    for (int i = 0; i < gNumCompsPerType[COMP_AI_RAND]; i++) {
        SimplePhysicsComponent* physComp = getComponent(
            gAiRandCompsDense[i].header.entId,
            COMP_PHYSICS_SIMPLE
        );
        physComp->angle += qran_range(-1, 2) * qran_range(0x1000, 0x2000);
    }
}

void removeComponentAiRand(int entId) {
    removeComponent(entId, COMP_AI_RAND);
}
