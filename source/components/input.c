#include "input.h"

void handleInputNormal(int entIndex) { // coupled to PhysicsComponent
    PhysicsComponent* ent = &gPhysCompsDense[gCompSetSparse[COMP_PHYSICS][entIndex]];
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
    if (key_hit(KEY_L | KEY_R))
        spawnEnemyWeak(qran_range(0, SCREEN_WIDTH) << 16, qran_range(0, SCREEN_HEIGHT) << 16);
    Vector dirVec = { {key_tri_horz() * ent->archetype->radius * ent->archetype->accel.WORD},
        {key_tri_vert() * ent->archetype->radius * ent->archetype->accel.WORD} };
    ent->vec = addVec(ent->vec, dirVec);
}

void updateInputComps() {
    for (int i = 0; i < gNumCompsPerType[COMP_INPUT]; i++) {
        gInputCompsDense[i].inputHandler(gInputCompsDense[i].header.entIndex);
    }
}
