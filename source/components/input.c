#include "component.h"

int alpha = 0;

void handlePalTimerExpire(int entId) {
    memcpy16(&pal_bg_bank[MAP_PAL], gradientPal, gradientPalLen / sizeof(u16));
    markEntToBeDeleted(entId);
}

void addComponentInput(int entId, int flags, void (*inputHandler)(int entId)) {
    InputComponent inputComp = {{entId, flags}, inputHandler};
    addComponentCustom((void*)&inputComp, COMP_INPUT);
}

void handleInputNormal(int entId) { // coupled to PhysicsComponent
    PhysicsComponent* physComp = getComponent(entId, COMP_PHYSICS);
    if (!physComp) return;
    Vector dirVec = { {key_tri_horz() * physComp->archetype->radius * physComp->archetype->accel.WORD},
        {key_tri_vert() * physComp->archetype->radius * physComp->archetype->accel.WORD} };
    physComp->vec = addVec(physComp->vec, dirVec);

    if (key_is_down(KEY_DIR)) {
        CounterComponent* health = getCounterByFlags(gPlayerId, COUNTER_HEALTH_FLAG);
        CounterComponent* numDefeated = getCounterByFlags(gPlayerId, COUNTER_NUM_DEFEATED_FLAG);
        if (health && !(gFrameCount & 3)) incrementCounter(health, numDefeated ? -(clamp((numDefeated->curr * lu_div(10)) >> 16, 0, 4)) : -1);
    }

    if (key_hit(KEY_A)) {
        doNothing();
    }

    if (key_hit(KEY_START)) { // reset
        setGameState(PAUSE);
        return;
    }
#ifdef DEBUG
    if (key_hit(KEY_B)) {
        if (numComps(COMP_PHYSICS_SIMPLE) < 64)
            spawnParticlesEnemyDefeat(physComp);
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

void handleInputGameover(int entId) {
    if (numComps(COMP_TIMER) == 0 && key_hit(KEY_START)) {
        setGameState(NORMAL);
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
