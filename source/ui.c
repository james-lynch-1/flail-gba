#include "ui.h"

void updateUINormal() {
    tte_erase_line();
    memset32(&se_mem[UI_SBB], 0, SCREEN_WIDTH_T / 2);
    memset32((void*)(int)&se_mem[UI_SBB] + sizeof(SCR_ENTRY) * SBB_WIDTH_T * (SCREEN_HEIGHT_T - 1),
        0,
        SCREEN_WIDTH_T / 2);
    CounterComponent* health = getCounterByFlags(gPlayerId, COUNTER_HEALTH_FLAG);
    CounterComponent* power = getCounterByFlags(gPlayerId, COUNTER_POWER_FLAG);
    CounterComponent* numDefeated = getCounterByFlags(gPlayerId, COUNTER_NUM_DEFEATED_FLAG);
    if (health) {
        int healthCurrOverMax = ((s64)(health->curr << 16) * lu_div(health->max)) >> 16;
        int numTilesRemaining = (SCREEN_WIDTH_T * (healthCurrOverMax)) >> 16;
        memset16(&se_mem[UI_SBB], (SE_ID(1) | SE_PALBANK(UI_PAL)), numTilesRemaining);
        gObjBuffer[127].attr1 &= ~ATTR1_X_MASK;
        gObjBuffer[127].attr1 |= ATTR1_X(((SCREEN_WIDTH * healthCurrOverMax) >> 16) - TILE_WIDTH - 11) & ATTR1_X_MASK;
        if (health->curr == 0) gObjBuffer[127].attr0 |= ATTR0_HIDE;
        else gObjBuffer[127].attr0 &= ~ATTR0_MODE_MASK;
    }
    if (power) {
        int powerCurrOverMax = ((int)(power->curr * (1 << 16))) / (int)power->max;
        int numTilesRemaining = (SCREEN_WIDTH_T * powerCurrOverMax) >> 16;
        memset16((void*)(int)&se_mem[UI_SBB] + sizeof(SCR_ENTRY) * SBB_WIDTH_T * (SCREEN_HEIGHT_T - 1),
            (SE_ID(1) | SE_PALBANK(UI_PAL)),
            numTilesRemaining);
        gObjBuffer[126].attr1 &= ~ATTR1_X_MASK;
        gObjBuffer[126].attr1 |= ATTR1_X(((SCREEN_WIDTH * powerCurrOverMax) >> 16) - TILE_WIDTH - 11) & ATTR1_X_MASK;
        if (power->curr == 0) gObjBuffer[126].attr0 |= ATTR0_HIDE;
        else gObjBuffer[126].attr0 &= ~ATTR0_MODE_MASK;
    }
    if (numDefeated) {
        char numDefStr[7];
        sprintf(numDefStr, "%d", numDefeated->curr);
        tte_set_pos(8, TILE_WIDTH);
        tte_write(numDefStr);
    }
}

void initialiseUi() {
    // set ui bar palette
    memcpy16(&pal_bg_bank[UI_PAL], healthBarPal, healthBarPalLen / sizeof(u16));
    memcpy16(&pal_obj_bank[2], healthBarPal, healthBarPalLen / sizeof(u16));
    memcpy32(&tile_mem[BG_UI][1], healthBarTiles, healthBarTilesLen / sizeof(u32));
    tte_init_se(
        BG_TEXT,
        BG_CBB(CBB_TEXT) | BG_SBB(TEXT_SBB) | BG_PRIO(PRIO_TEXT),
        0,
        CLR_WHITE,
        14,
        NULL,
        se_drawg_w8h8
    );
    // health bar obj
    gObjBuffer[127].attr2 = ATTR2_ID(fetchSprite(healthBarTiles, healthBarTilesLen)) |
        ATTR2_PRIO(1) | ATTR2_PALBANK(2);
    gObjBuffer[127].attr0 = ATTR0_Y(0) | ATTR0_WIDE;
    gObjBuffer[127].attr1 = ATTR1_SIZE_16x8;

    // power bar obj
    gObjBuffer[126].attr2 = ATTR2_ID(fetchSprite(healthBarTiles, healthBarTilesLen)) |
        ATTR2_PRIO(1) | ATTR2_PALBANK(2);
    gObjBuffer[126].attr0 = ATTR0_Y(152) | ATTR0_WIDE;
    gObjBuffer[126].attr1 = ATTR1_SIZE_16x8;
}
