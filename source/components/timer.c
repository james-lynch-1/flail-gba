#include "component.h"

void updateTimers() {
    for (int i = 0; i < gNumCompsPerType[COMP_TIMER]; i++) {
        gTimerCompsDense[i].timeRemaining--;
        if (gTimerCompsDense[i].timeRemaining == 0) {
            if (gTimerCompsDense[i].callback) gTimerCompsDense[i].callback(gTimerCompsDense[i].header.entId);
            gTimerCompsDense[i].timeRemaining = gTimerCompsDense[i].time;
        }
    }
}

void addComponentTimer(s16 entId, u16 flags, u16 length, void(*callback)) {
    TimerComponent t = { {entId, 0}, length * 60, length * 60, callback };
    addComponentCustom(&t, COMP_TIMER);
}

void removeComponentTimer(int entId) {
    removeComponent(entId, COMP_TIMER);
}
