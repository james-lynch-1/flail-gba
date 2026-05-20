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

void incrementPower(int entId) {
    CounterComponent* power = getCounterByFlags(gPlayerId, COUNTER_POWER_FLAG);
    CounterComponent* numDefeated = getCounterByFlags(gPlayerId, COUNTER_NUM_DEFEATED_FLAG);
    if (!power || (gFlags & GFLAG_POWERED_UP)) return;
    Position playerPos = ((PhysicsComponent*)getComponent(gPlayerId, COMP_PHYSICS))->pos;
    Position enemyPos = gPhysCompsDense[1].pos; // ZZZ should get every enemy and choose the closest pos
    Vector vec = { {playerPos.x.WORD - enemyPos.x.WORD}, {playerPos.y.WORD - enemyPos.y.WORD} };
    int distance = numComps(COMP_PHYSICS) > 1 ?
        (fastMagnitude(vec.x.HALF.HI, vec.y.HALF.HI))
        : 120;
    // scale distance power modifier by 15% for every 10 fellas defeated
    int dist1PctFp = (distance * lu_div(100));
    distance += ((s64)(dist1PctFp * 15) * (numDefeated->curr * lu_div(10))) >> 32;
    SWord distPwrModifier = { .WORD = lu_div(clamp(distance, 0, 240)) << 12 };
    // if it's the last of the group, scale power gain up
    GroupComponent* group = getComponent(((MemberComponent*)(getComponent(entId, COMP_MEMBER)))->groupIds[0], COMP_GROUP);
    if (group->numCollected == group->numMembers) {
        distPwrModifier.WORD *= 2;
    }
    distPwrModifier = multSWord(distPwrModifier, power->incrementModifier);
    incrementCounter(power, clamp(distPwrModifier.HALF.HI, 20, power->max / 2));
    if (power->curr >= power->max) notify(gPlayerId, COMP_COUNTER, E_POWER_FULL);
}

void incrementHealthConditional() {
    CounterComponent* health = getCounterByFlags(gPlayerId, COUNTER_HEALTH_FLAG);
    CounterComponent* numDefeated = getCounterByFlags(gPlayerId, COUNTER_NUM_DEFEATED_FLAG);
    if(!health || !numDefeated) return;
    // if (numDefeated->curr >= 10)
        incrementCounter(health, PLAYER_HEALTH_INCREMENT);
}

void incrementCounter(CounterComponent* counter, int amount) {
    counter->curr += amount;
    counter->curr = clamp(counter->curr, 0, counter->max + 1);
}
