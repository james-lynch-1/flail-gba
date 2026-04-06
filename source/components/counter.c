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
    bool powerIsMax = power && (power->curr == power->max);
    if (powerIsMax) {
        // delete this fella
        markEntToBeDeleted(entId);

        // spawn a new fella
        spawnEnemy(120 << 16, 80 << 16);

        // reset power and health
        power->curr = 0;
        if (health) incDecCounter(health, 50);

        CounterComponent* numDefeated = getCounterByFlags(gPlayerId, COUNTER_NUM_DEFEATED_FLAG);
        if (numDefeated) incDecCounter(numDefeated, 1);
        return;
    }
    if (!health) return;
    if (health->curr > 0) incDecCounter(health, PLAYER_HEALTH_DECREMENT);
    if (health->curr <= 0) {
        notify(gPlayerId, COMP_COUNTER, E_PLAYER_DIED);
    }
}

void incrementPower(int entId) {
    CounterComponent* power = getCounterByFlags(gPlayerId, COUNTER_POWER_FLAG);
    if (!power) return;
    Position playerPos = ((PhysicsComponent*)getComponent(gPlayerId, COMP_PHYSICS))->pos;
    Position enemyPos = gPhysCompsDense[1].pos;
    Vector vec = { {playerPos.x.WORD - enemyPos.x.WORD}, {playerPos.y.WORD - enemyPos.y.WORD} };
    int distance = fastMagnitude(vec.x.HALF.HI, vec.y.HALF.HI);
    
    SWord distPwrModifier = {.HALF.HI = 140 - distance, .HALF.LO = 0};
    distPwrModifier = multSWord(distPwrModifier, power->incrementModifier);
    power->curr += clamp(distPwrModifier.HALF.HI, 20, power->max / 2);
    power->curr = clamp(power->curr, 0, power->max + 1);
}

void incDecCounter(CounterComponent* counter, int amount) {
    counter->curr += amount;
    counter->curr = clamp(counter->curr, 0, counter->max + 1);
}
