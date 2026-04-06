#include "ui.h"

void updateUINormal() {
    tte_erase_screen();
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
        memset16(&se_mem[UI_SBB], (SE_ID(512) | SE_PALBANK(1)), numTilesRemaining);
        gObjBuffer[127].attr1 &= ~ATTR1_X_MASK;
        gObjBuffer[127].attr1 |= ATTR1_X(((SCREEN_WIDTH * healthCurrOverMax) >> 16) - TILE_WIDTH - 11) & ATTR1_X_MASK;
        if (health->curr == 0) gObjBuffer[127].attr0 |= ATTR0_HIDE;
        else gObjBuffer[127].attr0 &= ~ATTR0_MODE_MASK;
    }
    if (power) {
        int powerCurrOverMax = ((int)(power->curr * (1 << 16))) / (int)power->max;
        int numTilesRemaining = (SCREEN_WIDTH_T * powerCurrOverMax) >> 16;
        memset16((void*)(int)&se_mem[UI_SBB] + sizeof(SCR_ENTRY) * SBB_WIDTH_T * (SCREEN_HEIGHT_T - 1),
            (SE_ID(512) | SE_PALBANK(1)),
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
    memcpy16(&pal_bg_bank[1], healthBarPal, healthBarPalLen / sizeof(u16));
    memcpy16(&pal_obj_bank[2], healthBarPal, healthBarPalLen / sizeof(u16));
    memcpy32(&tile_mem[1][0], healthBarTiles, healthBarTilesLen / sizeof(u32));
    for (int i = 0; i < 10; i++)
        memcpy32((void*)(int)&tile_mem[1] + healthBarTilesLen + i * oneTilesLen,
            gNumTileArr[i],
            oneTilesLen / sizeof(u32));
    tte_init_se(
        2,
        BG_CBB(2) | BG_SBB(TEXT_SBB) | BG_PRIO(2),
        0,
        CLR_MONEYGREEN,
        14,
        NULL,
        NULL
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
