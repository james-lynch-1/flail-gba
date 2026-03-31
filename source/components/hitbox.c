#include "component.h"

void updateHitboxes() {
    PhysicsComponent* player = getComponent(gPlayerId, COMP_PHYSICS);
    for (int i = 0; i < gNumCompsPerType[COMP_HITBOX]; i++) {
        HitboxComponent* hBox = &gHitboxCompsDense[i];
        if (checkPlayerToHitboxCollision(player, hBox)) {
            doGroupCallbacks(hBox->header.entId);
        }
    }
}

void removeComponentHitbox(int entId) {
    removeComponent(entId, COMP_HITBOX);
}
