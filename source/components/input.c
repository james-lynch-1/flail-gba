#include "component.h"

void handleInputNormal(s16 entId) { // coupled to PhysicsComponent
    PhysicsComponent* physComp = getComponent(entId, COMP_PHYSICS);
    if (!physComp) return;
    Vector dirVec = { {key_tri_horz() * physComp->archetype->radius * physComp->archetype->accel.WORD},
        {key_tri_vert() * physComp->archetype->radius * physComp->archetype->accel.WORD} };
    physComp->vec = addVec(physComp->vec, dirVec);

    if (key_hit(KEY_START)) { // reset
        exitNormal(PAUSE);
        return;
    }
#ifdef DEBUG
    if (key_hit(KEY_B)) {
        if (gNumEnts < 50)
            for (int i = 0; i < 50; i++)
                spawnEnemyWeak(0, 0);
        return;
    }
    if (key_hit(KEY_L | KEY_R)) {
        u8* hBoxDimension = (u8*)&physComp->archetype->hitbox;
        char hPrefix[9] = "height: ";
        char wPrefix[8] = "width: ";
        char* dim = wPrefix;
        if (key_is_down(KEY_A)) {
            hBoxDimension++;
            dim = hPrefix;
        }
        if (key_hit(KEY_R)) {
            if (*hBoxDimension < 64) *hBoxDimension += 1;
        }
        else {
            if (*hBoxDimension > 1) *hBoxDimension -= 1;
        }
        removeComponentDebugBlob(gPlayerId);
        addComponentDebugBlob(gPlayerId);
        logVal(dim, *hBoxDimension);
        return;
    }
    if (key_hit(KEY_SELECT)) {
        gFlags ^= GFLAG_GRAVITY;
        ObjComponent* objComp = getComponent(gPlayerId, COMP_OBJ);
        getObj(objComp)->attr2 ^= ATTR2_PALBANK(1);
        return;
    }
#endif
}

void updateInputComps() {
    for (int i = 0; i < gNumCompsPerType[COMP_INPUT]; i++) {
        gInputCompsDense[i].inputHandler(gInputCompsDense[i].header.entId);
    }
}

void removeComponentInput(int entId) {
    removeComponent(entId, COMP_INPUT);
}
