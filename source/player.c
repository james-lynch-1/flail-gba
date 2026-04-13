#include "player.h"

void updatePlayerStuff() {
    // deplete power
    CounterComponent* power = getCounterByFlags(gPlayerId, COUNTER_POWER_FLAG);
    if (power && gFrameCount & 1) {
        if (power->curr < power->max) {
            incrementCounter(power, PLAYER_POWER_DECREMENT);
            power->curr = clamp(power->curr, 0, power->max);
        }
    }
}

void doPlayerHurtAnimation() {
    int dir = 1 - (gFrameCount & 1) * 2;
    obj_aff_scale(getObjAff(((ObjAffComponent*)getComponent(gPlayerId, COMP_OBJ_AFF))),
    0x100 + dir,
    0x100 + dir);
}

int spawnPlayer(int x, int y) {
    int entId = reserveEntSlot();
    gPlayerId = entId;
    if (entId == -1) return -1;

    ObjComponent* o = addComponentObj(entId, 0, COMP_PHYSICS);
    getObj(o)->attr0 = ATTR0_REG | ATTR0_Y(x >> 16);
    getObj(o)->attr1 = ATTR1_SIZE_16x16 | ATTR1_X(y >> 16);
    getObj(o)->attr2 = ATTR2_ID(fetchSprite(magicBallYellowTiles, magicBallYellowTilesLen)) | ATTR2_PRIO(1) | ATTR2_PALBANK(0);

    ObjAffComponent* oA = addComponentObjAff(entId, 0, true);
    obj_aff_identity(getObjAff(oA));

    InputComponent input = { {entId, 0}, handleInputNormal };

    PhysicsComponent phys = {
        {entId, PHYS_GRAVITY_FLAG},
        {{x}, {y}},
        (PhysArchetype*)&gPhysArchetypesStatic[ARCHETYPE_PLAYER],
        { {0}, {0} },
        0
    };
    addComponentGroup(gPlayerId, 0, NULL, NULL, 0, ENT_PLAYER);
    MemberComponent* healthMember = addComponentMember(reserveEntSlot(), 0, gPlayerId);
    addComponentCounter(healthMember->header.entId, COUNTER_HEALTH_FLAG, 150, 150);
    MemberComponent* powerMember = addComponentMember(reserveEntSlot(), 0, gPlayerId);
    addComponentCounter(powerMember->header.entId, COUNTER_POWER_FLAG, 0, 360);
    MemberComponent* numDefeatedMember = addComponentMember(reserveEntSlot(), 0, gPlayerId);
    addComponentCounter(numDefeatedMember->header.entId, COUNTER_NUM_DEFEATED_FLAG, 0, INT16_MAX);
    if (!addComponentCustom(&input, COMP_INPUT)) return -1;
    if (!addComponentCustom(&phys, COMP_PHYSICS)) return -1;

    // addEventListener(COMP_PHYSICS, E_PHYS_TOUCHED, doPlayerHurtAnimation, false);
    return 1;
}

void handlePlayerDied() {
    setGameState(GAMEOVER);
}
