#include "component.h"

void handleInputNormal(s16 entId) { // coupled to PhysicsComponent
    PhysicsComponent* physComp = getComponent(entId, COMP_PHYSICS);
    if (!physComp) return;
    Vector dirVec = { {key_tri_horz() * physComp->archetype->radius * physComp->archetype->accel.WORD},
        {key_tri_vert() * physComp->archetype->radius * physComp->archetype->accel.WORD} };
    physComp->vec = addVec(physComp->vec, dirVec);
    if (key_hit(KEY_SELECT)) {
        gFlags ^= GFLAG_GRAVITY;
        ObjComponent* objComp = getComponent(gPlayerId, COMP_OBJ);
        objComp->obj->attr2 ^= ATTR2_PALBANK(1);
        return;
    }
    if (key_hit(KEY_B)) {
        if (gNumEnts < 50)
            for (int i = 0; i < 50; i++)
                spawnEnemyWeak(0, 0);
        return;
    }

    if (key_hit(KEY_START)) { // reset
        for (int i = 0; i < numComps(COMP_PHYSICS_SIMPLE); i++)
            markEntToBeDeleted(gSimplePhysCompsDense[i].header.entId);

        physComp->archetype->hitbox.width = 16;
        physComp->archetype->hitbox.height = 16;
        removeComponentDebugBlob(gPlayerId);
        PositionMini p = { 0, 0 };
        addStarLine(0, p, 60);
        return;
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
    //     return;
    // }
    // if (key_hit(KEY_R)) {
    //     if (hasComponent(gPlayerId, COMP_DEBUG_BLOB)) removeComponentDebugBlob(gPlayerId);
    //     else addComponentDebugBlob(gPlayerId);
    //     return;
    // }
    // if (key_hit(KEY_A)) {
    //     if (key_is_down(KEY_SELECT))
    //         while (numComps(COMP_DEBUG_BLOB) > 0)
    //             removeComponentDebugBlob(gDebugBlobCompsDense[0].header.entId);
    //     else
    //         for (int i = 0; i < numComps(COMP_OBJ); i++)
    //             addComponentDebugBlob(gObjCompsDense[i].header.entId);
    //     return;
    // }
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
        // logVal("numObjs: ", numComps(COMP_OBJ));
        // logVal("numBlobs: ", numComps(COMP_DEBUG_BLOB));
        logVal(dim, *hBoxDimension);
        return;
    }
}

void updateInputComps() {
    for (int i = 0; i < gNumCompsPerType[COMP_INPUT]; i++) {
        gInputCompsDense[i].inputHandler(gInputCompsDense[i].header.entId);
    }
}

void removeComponentInput(int entId) {
    removeComponent(entId, COMP_INPUT);
}
