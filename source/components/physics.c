#include "component.h"

void pullTowardsPosition(PhysicsComponent* physComp, int x, int y) {
    Vector gravityVec = { {x - physComp->pos.x.WORD}, {y - physComp->pos.y.WORD} };
    gravityVec = normaliseVec(gravityVec);
    gravityVec = divVec(gravityVec, 4);
    physComp->vec = addVec(physComp->vec, gravityVec);
}

void pushAwayFromPosition(PhysicsComponent* physComp, int x, int y) {
    Vector gravityVec = { {physComp->pos.x.WORD - x}, {physComp->pos.y.WORD - y} };
    gravityVec = normaliseVec(gravityVec);
    gravityVec = divVec(gravityVec, 4);
    physComp->vec = addVec(physComp->vec, gravityVec);
}

void updatePlayerPhysics() {
    PhysicsComponent* player = getComponent(gPlayerId, COMP_PHYSICS);
    for (int i = 0; i < numComps(COMP_PHYSICS); i++) {
        if (gPhysCompsDense[i].header.entId == player->header.entId) continue;
        if (checkPhysCompToPhysCompCollision(player, &gPhysCompsDense[i])) {
            notify(gPhysCompsDense[i].header.entId, COMP_PHYSICS, E_PHYS_TOUCHED);
        }
    }
    bool isGravityOn = (gFlags & GFLAG_GRAVITY) && (player->header.flags & PHYS_GRAVITY_FLAG);
    u32 mag = fastMagnitude(player->vec.x.WORD, player->vec.y.WORD);
    if (!isGravityOn && mag < 0x1800 && !(key_is_down(KEY_DIR))) {
        player->vec.x.WORD = 0;
        player->vec.y.WORD = 0;
    }
}

void addComponentSimplePhysics(int entId, u16 flags, int x, int y, int archetypeIndex, int vecX, int vecY) {
    SimplePhysicsComponent sP = { {entId, flags}, {{x}, {y}}, &gPhysArchetypesStatic[archetypeIndex], {{vecX}, {vecY}} };
    addComponentCustom((void*)&sP, COMP_PHYSICS_SIMPLE);
}

// first use the input or ai components to add a vec to this vec, then clamp and decay speed, and apply gravity
void updatePhysics() {
    for (int i = 0; i < gNumCompsPerType[COMP_PHYSICS]; i++) {
        PhysicsComponent* ent = &gPhysCompsDense[i];
        bool isGravityOn = (gFlags & GFLAG_GRAVITY) && (ent->header.flags & PHYS_GRAVITY_FLAG);
        u32 mag = fastMagnitude(ent->vec.x.WORD, ent->vec.y.WORD);
        bool isCentredAndSufficientlySlow =
            in_range(ent->pos.x.HALF.HI, 119, 121) &&
            in_range(ent->pos.y.HALF.HI, 79, 81) &&
            mag < 0x1800;

        ent->vec = decaySpeed(ent->vec, 0x10000 - ent->archetype->decay);

        if ((mag >> 16) > ent->archetype->radius) {
            ent->vec = normaliseVec(ent->vec);
            ent->vec = scalarMultVec(ent->vec, ent->archetype->radius);
        }
        if (isGravityOn && !isCentredAndSufficientlySlow) {
            pullTowardsPosition(ent, 120 << 16, 80 << 16);
        }

        // stop if centred and sufficiently slow
        if (isCentredAndSufficientlySlow && isGravityOn) {
            ent->vec.x.WORD = 0;
            ent->vec.y.WORD = 0;
            ent->pos.x.WORD = 120 << 16;
            ent->pos.y.WORD = 80 << 16;
        }

        if (!in_range(ent->pos.x.HALF.HI, 0 + ent->archetype->hitbox.width / 2, SCREEN_WIDTH - ent->archetype->hitbox.width / 2))
            ent->vec.x.WORD += ((reflect(ent->pos.x.HALF.HI, 0, SCREEN_WIDTH) - ent->pos.x.HALF.HI) << 16) / 4;
        if (!in_range(ent->pos.y.HALF.HI, 0, SCREEN_HEIGHT - ent->archetype->hitbox.height / 2)) {
            ent->vec.y.WORD += ((reflect(ent->pos.y.HALF.HI, 0, SCREEN_HEIGHT) - ent->pos.y.HALF.HI) << 16) / 4;
            doNothing();
        }
        ent->pos.x.WORD += ent->vec.x.WORD;
        ent->pos.y.WORD += ent->vec.y.WORD;
    }
}

// for enemies. doesn't use arctan2 to normalise vec.
// just changes vec using angle and adds vec to position
void updatePhysicsSimple() {
    PhysicsComponent* enemyPhys = &gPhysCompsDense[1];
    for (int i = 0; i < gNumCompsPerType[COMP_PHYSICS_SIMPLE]; i++) {
        SimplePhysicsComponent* ent = &gSimplePhysCompsDense[i];
        if (ent->archetype == &gPhysArchetypesStatic[ARCHETYPE_ITEM]) {
            ObjComponent* objComp = getComponent(ent->header.entId, COMP_OBJ);
            OBJ_ATTR* obj = getObj(objComp);
            Vector enemyToItemVec = {
                .x = {ent->pos.x.WORD - enemyPhys->pos.x.WORD},
                .y = {ent->pos.y.WORD - enemyPhys->pos.y.WORD}
            };
            if (fastMagnitude(enemyToItemVec.x.HALF.HI, enemyToItemVec.y.HALF.HI) < 40)
                obj->attr2 = (obj->attr2 & ~ATTR2_PALBANK_MASK) | ATTR2_PALBANK(1);
            else obj->attr2 = (obj->attr2 & ~ATTR2_PALBANK_MASK) | ATTR2_PALBANK(0);
            obj->attr1 = (obj->attr1 & ~ATTR1_VFLIP) | (key_is_down(KEY_DIR) > 0) << (ATTR1_FLIP_SHIFT + 1);
        }
        if (
            !in_range(
                ent->pos.x.HALF.HI, 0 + ent->archetype->hitbox.width / 2,
                SCREEN_WIDTH - ent->archetype->hitbox.width / 2
            )) {
            SWord refX = { .WORD = reflect(ent->pos.x.WORD, (0 + ent->archetype->hitbox.width / 2) << 16, (SCREEN_WIDTH - ent->archetype->hitbox.width / 2) << 16) };
            ent->vec.x.WORD += refX.WORD - ent->pos.x.WORD;
        }
        if (
            !in_range(
                ent->pos.y.HALF.HI, 0 + ent->archetype->hitbox.height / 2,
                SCREEN_HEIGHT - ent->archetype->hitbox.height / 2
            )) {
            SWord refY = { .WORD = reflect(ent->pos.y.WORD, (0 + ent->archetype->hitbox.height / 2) << 16, (SCREEN_HEIGHT - ent->archetype->hitbox.height / 2) << 16) };
            ent->vec.y.WORD += refY.WORD - ent->pos.y.WORD;
        }
        ent->vec = decaySpeed(ent->vec, 0x10000 - ent->archetype->decay);
        ent->pos.x.WORD += ent->vec.x.WORD;
        ent->pos.y.WORD += ent->vec.y.WORD;
    }
}

bool checkPlayerToHitboxCollision(PhysicsComponent* ent, HitboxComponent* hBox) {
    SimplePhysicsComponent* simplePhys = getComponent(hBox->header.entId, COMP_PHYSICS_SIMPLE);
    int playerhBWidth = ent->archetype->hitbox.width, playerhBHeight = ent->archetype->hitbox.height;
    return in_range(ent->pos.x.HALF.HI,
        simplePhys->pos.x.HALF.HI - (playerhBWidth + hBox->hitbox.width) / 2,
        simplePhys->pos.x.HALF.HI + (playerhBWidth + hBox->hitbox.width) / 2) &&
        in_range(ent->pos.y.HALF.HI,
            simplePhys->pos.y.HALF.HI - (playerhBHeight + hBox->hitbox.height) / 2,
            simplePhys->pos.y.HALF.HI + (playerhBHeight + hBox->hitbox.height) / 2);
}

bool checkPhysCompToPhysCompCollision(PhysicsComponent* p1, PhysicsComponent* p2) {
    int p1hBWidth = p1->archetype->hitbox.width, p2hBWidth = p2->archetype->hitbox.width;
    int p1hBHeight = p1->archetype->hitbox.height, p2hBHeight = p2->archetype->hitbox.height;
    return in_range(p1->pos.x.HALF.HI,
        p2->pos.x.HALF.HI - (p1hBWidth + p2hBWidth) / 2,
        p2->pos.x.HALF.HI + (p1hBWidth + p2hBWidth) / 2) &&
        in_range(p1->pos.y.HALF.HI,
            p2->pos.y.HALF.HI - (p1hBHeight + p2hBHeight) / 2,
            p2->pos.y.HALF.HI + (p1hBHeight + p2hBHeight) / 2);
}

void handlePlayerToPhysCollision(int entId) {
    CounterComponent* power = getCounterByFlags(gPlayerId, COUNTER_POWER_FLAG);
    CounterComponent* health = getCounterByFlags(gPlayerId, COUNTER_HEALTH_FLAG);
    CounterComponent* numDefeated = getCounterByFlags(gPlayerId, COUNTER_NUM_DEFEATED_FLAG);
    bool powerIsMax = power && (power->curr == power->max);
    if (powerIsMax) { // kill 'im
        PhysicsComponent* enemyPhys = getComponent(entId, COMP_PHYSICS);
        removeComponentAi(entId);
        gHitstunFrameCount = 2;
        PhysicsComponent* playerPhys = getComponent(gPlayerId, COMP_PHYSICS);
        // knock it away, then it explodes when it either it hits the screen edge or 2 seconds pass,
        // whichever comes first
        Vector flyAwayVec = {
            {enemyPhys->pos.x.WORD - playerPhys->pos.x.WORD},
            {enemyPhys->pos.y.WORD - playerPhys->pos.y.WORD}
        };
        flyAwayVec = normaliseVec(flyAwayVec);
        enemyPhys->archetype = &gPhysArchetypesStatic[ARCHETYPE_ENEMY_DEAD];
        enemyPhys->vec.x.WORD = 8 * flyAwayVec.x.WORD;
        enemyPhys->vec.y.WORD = 8 * flyAwayVec.y.WORD;
        addTask(taskExplodeWhenWallIsHit, entId, 120);

        // set background to normal colours
        memcpy32(&pal_bg_bank[MAP_PAL], gradientPal, gradientPalLen / sizeof(u32));

        // shrink player to normal size
        ObjAffComponent* objAff = getComponent(gPlayerId, COMP_OBJ_AFF);
        finishAffineAnimation(objAff);
        obj_aff_identity(getObjAff(objAff));

        // spawn a new fella after 2 seconds
        addComponentTimer(reserveEntSlot(), TIMER_DELETE_ENT, 120, spawnEnemyCentred);

        // reset power and health
        power->curr = 0;
        if (health) health->curr = health->max;

        if (numDefeated && numDefeated->curr < 255) incrementCounter(numDefeated, 1);
        else numDefeated->curr = 255;
        return;
    }
    if (!health) return;
    // health decrement increases every 20 enemies defeated, caps at 40
    int nDDiv = clamp((numDefeated->curr * lu_div(20)) >> 16, 0, 3);
    if (health->curr > 0) incrementCounter(health, PLAYER_HEALTH_DECREMENT - nDDiv);
}

// utils

Vector decaySpeed(Vector vec, u32 rate) {
    vec.x.WORD = ((s64)rate * vec.x.WORD) >> 16;
    vec.y.WORD = ((s64)rate * vec.y.WORD) >> 16;
    return vec;
}

// https://en.m.wikipedia.org/wiki/Alpha_max_plus_beta_min_algorithm
u32 fastMagnitude(int x, int y) {
    u32 max = MAX(ABS(x), ABS(y));
    u32 min = MIN(ABS(x), ABS(y));
    return (((u64)max * 0b1111010111011111) >> 16) + ((u64)min * 0b0110010111011000 >> 16);
}

Vector addVec(Vector vec1, Vector vec2) {
    Vector vec = { {vec1.x.WORD + vec2.x.WORD}, {vec1.y.WORD + vec2.y.WORD} };
    return vec;
}

// uses lu_div tonc_math function to perform a reciprocal multiplication (0 ≤ x ≤ 255)
Vector divVec(Vector vec, u32 divisor) {
    vec.x.WORD *= lu_div(divisor);
    vec.y.WORD *= lu_div(divisor);
    vec.x.WORD >>= 16;
    vec.y.WORD >>= 16;
    return vec;
}

Vector normaliseVec(Vector vec) {
    u32 angle = ArcTan2(vec.x.WORD >> 12, -vec.y.WORD >> 12);
    Vector normVec = { {(lu_cos(angle)) << 4}, {(-lu_sin(angle)) << 4} };
    return normVec;
}

Vector scalarMultVec(Vector vec, int scalar) {
    Vector newVec = { {vec.x.WORD * scalar}, {vec.y.WORD * scalar} };
    return newVec;
}

void removeComponentPhysics(int entId) {
    removeComponent(entId, COMP_PHYSICS);
}

void removeComponentPhysicsSimple(int entId) {
    removeComponent(entId, COMP_PHYSICS_SIMPLE);
}
