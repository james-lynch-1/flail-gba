#include "component.h"

void addComponentTimer(s16 entId, u16 flags, u32 length, void(*callback)) {
    TimerComponent t = { {entId, 0}, length, callback };
    addComponentCustom(&t, COMP_TIMER);
}

void updateTimers() {
    for (int i = 0; i < gNumCompsPerType[COMP_TIMER]; i++) {
        gTimerCompsDense[i].time--;
        if (gTimerCompsDense[i].time == 0) {
            gTimerCompsDense[i].callback();
            gTimerCompsDense[i].time = 60;
        }
    }
}

void removeComponentTimer(int entId) {
    removeComponent(entId, COMP_TIMER);
}
