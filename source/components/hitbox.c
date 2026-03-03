#include "component.h"

void updateHitboxes() {
    PhysicsComponent* player = getComponent(gPlayerId, COMP_PHYSICS);
    for (int i = 0; i < gNumCompsPerType[COMP_HITBOX]; i++) {
        HitboxComponent* hBox = &gHitboxCompsDense[i];
        if (checkPlayerToHitboxCollision(player, hBox)) {
            markEntToBeDeleted(hBox->header.entId);
            // if member of group, do that group's collection action
            MemberComponent* mComp = getComponent(hBox->header.entId, COMP_MEMBER);
            if (mComp)
                doGroupCallbacks(hBox->header.entId);
        }
    }
    // todo: make this branch less. Maybe enable a flag that member components can use to
    // check if they need to do their action or not, in the member update function
}

void removeComponentHitbox(int entId) {
    removeComponent(entId, COMP_HITBOX);
}
