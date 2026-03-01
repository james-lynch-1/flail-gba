#include "star.h"

const StarLine StarLines[] = { {
    {{0, 10}, {0, 20}, {0, 40}, {120, 120}}
} };

void timerCallback() {
    log(CHAR, "time!");
}

// spawns a line of stars. Takes as parameters a type of star line, its
// position, and the frequency with which it spawns
void addStarLine(int starLineType, PositionMini pos, u32 freq) {
    s16 entId = reserveEntSlot();
    if (entId == -1) return;
    // timer, counter, physics
    TimerComponent t = { {entId, 0}, 60, timerCallback };
    addComponentCustom(&t, COMP_TIMER);
    StarLine sL = StarLines[starLineType];
    u16* posPtr = (u16*)&sL;
    int i = 0;
    while (*posPtr != 0 && i++ < MAX_STARLINE_LENGTH) {
        // add star pos to group as an enemy
        spawnEnemyWeak((*(u8*)posPtr) << 16, (*(u8*)((u8*)posPtr + 1)) << 16);
        posPtr++;
    }
    addComponentGroup(entId, 0, NULL, logSomething, 0);

    // addComponentCustom();
}
