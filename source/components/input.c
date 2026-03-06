#include "component.h"

void handleInputNormal(s16 entId) { // coupled to PhysicsComponent
    PhysicsComponent* physComp = getComponent(entId, COMP_PHYSICS);
    if (key_hit(KEY_START)) {
        exitNormal(PAUSE);
        return;
    }
    if (!physComp) return;
    if (key_hit(KEY_SELECT)) {
        gFlags ^= GFLAG_GRAVITY;
        ObjComponent* objComp = getComponent(gPlayerId, COMP_OBJ);
        objComp->obj->attr2 ^= ATTR2_PALBANK(1);
    }
    if (key_hit(KEY_A))
        for (int i = 0; i < 50; i++)
            spawnEnemyWeak(0, 0);

    if (key_hit(KEY_B)) {
        for (int i = 0; i < gNumEnts; i++) {
            if (gCompSetSparse[COMP_PHYSICS_SIMPLE][i] != -1) {
                markEntToBeDeleted(i);
            }
        }
    }
    // if (key_hit(KEY_L)) {
    //     int enemy = spawnEnemyWeak(qran_range(0, SCREEN_WIDTH) << 16, qran_range(0, SCREEN_HEIGHT) << 16);
    //     int gid;
    //     if (gNumCompsPerType[COMP_GROUP] == 0) {
    //         gid = reserveEntSlot();
    //         addComponentGroup(gid, 0, NULL, incrementNumCollectedAndLog, 0, ENT_ITEM);
    //     }
    //     else gid = gGroupCompsDense[0].header.entId; // ZZZ MAKE THIS NOT DUMB. TEST ONLY
    //     addComponentMember(enemy, ENT_ENEMY_WEAK, gid);
    // }
    if (key_hit(KEY_L)) {
        addComponentDebugBlob(gPlayerId);
    }
    if (key_hit(KEY_R)) {
        removeComponentDebugBlob(gPlayerId);
    }
    Vector dirVec = { {key_tri_horz() * physComp->archetype->radius * physComp->archetype->accel.WORD},
        {key_tri_vert() * physComp->archetype->radius * physComp->archetype->accel.WORD} };
    physComp->vec = addVec(physComp->vec, dirVec);
}

void updateInputComps() {
    for (int i = 0; i < gNumCompsPerType[COMP_INPUT]; i++) {
        gInputCompsDense[i].inputHandler(gInputCompsDense[i].header.entId);
    }
}

void removeComponentInput(int entId) {
    removeComponent(entId, COMP_INPUT);
}
