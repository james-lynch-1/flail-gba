#include "aiRand.h"

void updateAiRand() {
    for (int i = 0; i < gNumCompsPerType[COMP_AI_RAND]; i++)
        ((SimplePhysicsComponent*)getComponent(gAiRandCompsDense[i].header.entIndex, COMP_PHYSICS_SIMPLE))->angle +=
            qran_range(-1, 2) * qran_range(0x1000, 0x2000);
}
