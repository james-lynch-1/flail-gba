#include "ui.h"

void updateUINormal() {
    memset16(&se_mem[UI_SBB], 0, SCREEN_WIDTH_T);
    GroupComponent* group = getComponent(gPlayerId, COMP_GROUP);
    CounterComponent* health = NULL;
    int i = 0;
    while (!health && i < group->numMembers) {
        CounterComponent* counter = getComponent(group->memberIds[i++], COMP_COUNTER);
        if (counter && (counter->header.flags & COUNTER_HEALTH_FLAG)) {
            health = counter;
            break;
        }
    }
    if (!health) return;
    int numTilesRemaining = (SCREEN_WIDTH_T * fxdiv(health->curr, health->max)) >> FIX_SHIFT;
    memset16(&se_mem[UI_SBB], (SE_ID(512) | SE_PALBANK(1)), numTilesRemaining);
}

void initialiseUi() {
    memcpy16(&pal_bg_bank[1], healthBarPal, healthBarPalLen / sizeof(u16));
    memcpy32(&tile_mem[1][0], healthBarTiles, healthBarTilesLen / sizeof(u32));
}
