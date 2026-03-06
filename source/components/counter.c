#include "component.h"

void addComponentCounter(int entId, u16 max) {
    CounterComponent c = {{entId, 0}, max};
    addComponentCustom(&c, COMP_COUNTER);
}

void removeComponentCounter(int entId) {
    removeComponent(entId, COMP_COUNTER);
}
