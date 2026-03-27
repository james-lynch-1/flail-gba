#include "component.h"

CounterComponent* addComponentCounter(int entId, u16 flags, u16 max) {
    CounterComponent c = {{entId, flags}, max, max};
    CounterComponent* cAddr = (CounterComponent*)addComponentCustom(&c, COMP_COUNTER);
    return cAddr;
}

void removeComponentCounter(int entId) {
    removeComponent(entId, COMP_COUNTER);
}

void decrementCounter(int entId) {
    CounterComponent* counter = getComponent(entId, COMP_COUNTER);
    if (counter->curr > 0) counter->curr--;
}

void decrementCounterHealth(int entId) {
    GroupComponent* group = getComponent(entId, COMP_GROUP);
    CounterComponent* health = NULL;
    for (int i = 0; i < group->numMembers; i++) {
        health = getComponent(group->memberIds[i], COMP_COUNTER);
        if (health && (health->header.flags & COUNTER_HEALTH_FLAG)) break;
    }
    if (!health) return;
    if (health->curr > 0) health->curr--;
    // logVal("", health->curr);
}
