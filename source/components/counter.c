#include "component.h"

CounterComponent* addComponentCounter(int entId, u16 flags, s16 curr, s16 max) {
    CounterComponent c = { {entId, flags}, curr, max, (SWord)0x10000 };
    CounterComponent* cAddr = (CounterComponent*)addComponentCustom(&c, COMP_COUNTER);
    return cAddr;
}

void removeComponentCounter(int entId) {
    removeComponent(entId, COMP_COUNTER);
}

CounterComponent* getCounterByFlags(int entId, u16 flags) {
    CounterComponent* counter = NULL;
    GroupComponent* group = getComponent(entId, COMP_GROUP);
    if (!group) return NULL;
    for (int i = 0; i < group->numMembers; i++) {
        counter = getComponent(group->memberIds[i], COMP_COUNTER);
        if (counter && (counter->header.flags & flags)) break;
    }
    return counter;
}

void handlePlayerToPhysCollision(int entId) {
    CounterComponent* power = getCounterByFlags(gPlayerId, COUNTER_POWER_FLAG);
    CounterComponent* health = getCounterByFlags(gPlayerId, COUNTER_HEALTH_FLAG);
    CounterComponent* numDefeated = getCounterByFlags(gPlayerId, COUNTER_NUM_DEFEATED_FLAG);
    bool powerIsMax = power && (power->curr == power->max);
    if (powerIsMax) {
        // delete this fella
        markEntToBeDeleted(entId);

        memcpy32(&pal_bg_bank[MAP_PAL], gradientPal, gradientPalLen / sizeof(u32));
        ObjAffComponent* objAff = getComponent(gPlayerId, COMP_OBJ_AFF);
        finishAffineAnimation(objAff);
        obj_aff_identity(getObjAff(objAff));

        // spawn a new fella
        addComponentTimer(reserveEntSlot(), TIMER_DELETE_ENT, 120, spawnEnemyCentred);

        // reset power and health
        power->curr = 0;
        if (health) health->curr = health->max;

        CounterComponent* numDefeated = getCounterByFlags(gPlayerId, COUNTER_NUM_DEFEATED_FLAG);
        if (numDefeated && numDefeated->curr < 255) incrementCounter(numDefeated, 1);
        else numDefeated->curr = 255;
        return;
    }
    if (!health) return;
    int nDDiv = clamp((numDefeated->curr * lu_div(20)) >> 16, 0, 3);
    if (health->curr > 0) incrementCounter(health, PLAYER_HEALTH_DECREMENT - nDDiv);
}

void incrementPower(int entId) {
    CounterComponent* power = getCounterByFlags(gPlayerId, COUNTER_POWER_FLAG);
    CounterComponent* numDefeated = getCounterByFlags(gPlayerId, COUNTER_NUM_DEFEATED_FLAG);
    if (!power || power->curr >= power->max) return;
    Position playerPos = ((PhysicsComponent*)getComponent(gPlayerId, COMP_PHYSICS))->pos;
    Position enemyPos = gPhysCompsDense[1].pos;
    Vector vec = { {playerPos.x.WORD - enemyPos.x.WORD}, {playerPos.y.WORD - enemyPos.y.WORD} };
    int distance = numComps(COMP_PHYSICS) > 1 ?
        (fastMagnitude(vec.x.HALF.HI, vec.y.HALF.HI) + 10 * clamp((numDefeated->curr * lu_div(10)) >> 16, 0, 5))
        : 120;
    SWord distPwrModifier = { .WORD = lu_div(clamp(distance, 0, 240)) << 12 };
    distPwrModifier = multSWord(distPwrModifier, power->incrementModifier);
    incrementCounter(power, clamp(distPwrModifier.HALF.HI, 20, power->max / 2));
    if (power->curr >= power->max) notify(gPlayerId, COMP_COUNTER, E_POWER_FULL);
}

void incrementHealthConditional() {
    CounterComponent* health = getCounterByFlags(gPlayerId, COUNTER_HEALTH_FLAG);
    CounterComponent* numDefeated = getCounterByFlags(gPlayerId, COUNTER_NUM_DEFEATED_FLAG);
    if(!health || !numDefeated) return;
    // if (numDefeated->curr >= 10)
        incrementCounter(health, 0x20);
}

void incrementCounter(CounterComponent* counter, int amount) {
    counter->curr += amount;
    counter->curr = clamp(counter->curr, 0, counter->max + 1);
}

void handlePowerFull() {
    lightenBgPalette(0x4000);
    addAffineAnimation(getComponent(gPlayerId, COMP_OBJ_AFF), AFF_ANIM_GROW, 1);
}
