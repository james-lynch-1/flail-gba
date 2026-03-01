#include "component.h"

void logCollected(MemberComponent* m) {
    GroupComponent* g = getComponent(m->groupId, COMP_GROUP);
    log(U8, g->numCollected);
    if (g->numCollected == g->numMembers)
        log(CHAR, "collected all members!");
}

void handleInputNormal(s16 entId) { // coupled to PhysicsComponent
    PhysicsComponent* ent = &gPhysCompsDense[gCompSetSparse[COMP_PHYSICS][entId]];
    if (key_hit(KEY_START)) {
        exitNormal(PAUSE);
        return;
    }
    if (key_hit(KEY_SELECT)) {
        gFlags ^= GFLAG_GRAVITY;
        // gPlayer.obj->attr2 ^= ATTR2_PALBANK(1);
    }
    if (key_hit(KEY_A))
        for (int i = 0; i < 127; i++)
            spawnEnemyWeak(ent->pos.x.WORD, ent->pos.y.WORD);

    if (key_hit(KEY_B)) {
        for (int i = 0; i < gNumEnts; i++) {
            if (gCompSetSparse[COMP_PHYSICS_SIMPLE][i] != -1) {
                markEntToBeDeleted(i);
            }
        }
    }
    if (key_hit(KEY_L)) {
        int enemy = spawnEnemyWeak(qran_range(0, SCREEN_WIDTH) << 16, qran_range(0, SCREEN_HEIGHT) << 16);
        int gid;
        if (gNumCompsPerType[COMP_GROUP] == 0) {
            gid = reserveEntSlot();
            addComponentGroup(gid, 0, NULL, logCollected, 0);
        }
        else gid = gGroupCompsDense[0].header.entId; // ZZZ MAKE THIS NOT DUMB. TEST ONLY
        addComponentMember(enemy, MEMBER_ENEMY, gid);
    }
    if (key_hit(KEY_R)) {
        int inputCompType = getComponentType((ComponentHeader*)denseSetAddr(COMP_INPUT));
        log(INT, inputCompType);
    }
    Vector dirVec = { {key_tri_horz() * ent->archetype->radius * ent->archetype->accel.WORD},
        {key_tri_vert() * ent->archetype->radius * ent->archetype->accel.WORD} };
    ent->vec = addVec(ent->vec, dirVec);
}

void updateInputComps() {
    for (int i = 0; i < gNumCompsPerType[COMP_INPUT]; i++) {
        gInputCompsDense[i].inputHandler(gInputCompsDense[i].header.entId);
    }
}
