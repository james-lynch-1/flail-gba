#include "star.h"

const StarLine StarLines[] = {
    {{{-18, -10}, {7, -20}, {12, 15}, {UINT16_MAX, UINT16_MAX}}},
    {{{-25, 0}, {0, 5}, {25, 0}, {UINT16_MAX, UINT16_MAX}}}
};

void timerCallback() {
    log(CHAR, "time!");
}

void setGroupCompCounterToZero(int entId) {
    GroupComponent* group = getComponent(entId, COMP_GROUP);
    group->counter = 0;
}

void removeStarLine(int entId) {
    GroupComponent* group = getComponent(entId, COMP_GROUP);
    for (int i = 0; i < group->numMembers; i++)
        markEntToBeDeleted(group->memberIds[i]);
    markEntToBeDeleted(entId);
}

void handleStarLineTimerExpire(int entId) {
    CounterComponent* power = getCounterByFlags(gPlayerId, COUNTER_POWER_FLAG);
    power->curr = 0;
    removeStarLine(entId);
    bool spawnerExists = false;
    for (int i = 0; i < numComps(COMP_TIMER); i++) {
        if (gTimerCompsDense[i].callback == spawnStarLineRandomPos) {
            spawnerExists = true;
            break;
        }
    }
    if (!spawnerExists) addComponentTimer(reserveEntSlot(), 0, 2 * 60, spawnStarLineRandomPos);
}

void doGroupActionStarLine(MemberComponent* m, GroupComponent* g) {
    removeComponentObj(m->header.entId);
    removeComponentPhysicsSimple(m->header.entId);
    removeComponentHitbox(m->header.entId);
    CounterComponent* power = getCounterByFlags(gPlayerId, COUNTER_POWER_FLAG);
    if (!key_is_down(KEY_DIR)) {
        power->incrementModifier = setSWord(1, 0x8000);
    }
    else power->incrementModifier = setSWord(1, 0);
    notify(m->header.entId, COMP_PHYSICS, E_STAR_COLLECTED);
    g->numCollected++;
    g->counter++; // this is the no. of stars collected without input
    if (g->numCollected == g->numMembers) {
        // if (g->numMembers == g->counter) { // if whole group collected without input, max power
        //     if (power) power->curr = power->max;
        // }
        removeStarLine(g->header.entId);
        bool spawnerExists = false;
        for (int i = 0; i < numComps(COMP_TIMER); i++) {
            if (gTimerCompsDense[i].callback == spawnStarLineRandomPos) {
                spawnerExists = true;
                break;
            }
        }
        if (!spawnerExists) addComponentTimer(reserveEntSlot(), 0, 2 * 60, spawnStarLineRandomPos);
    }
}

void spawnStarLineRandomPos(int entId) {
    CounterComponent* numDefeated = getCounterByFlags(gPlayerId, COUNTER_NUM_DEFEATED_FLAG);
    int xModifier = numDefeated->curr * 9;
    int yModifier = numDefeated->curr * 8;
    // bool isXAxisModified = qran_range(0, 2);
    int xRanges[2] = { qran_range(20, 120 - xModifier), qran_range(120 + xModifier, 220) };
    int yRanges[2] = { qran_range(20, 80 - yModifier), qran_range(80 + yModifier, 120) };
    PositionMini pos = { xRanges[qran_range(0, 2)], yRanges[qran_range(0, 2)] };
    // PositionMini pos = { isXAxisModified ? xRanges[qran_range(0, 2)] : qran_range(20, 220),
    //     !isXAxisModified ? yRanges[qran_range(0, 2)] : qran_range(20, 140) };
    spawnStarLine(qran_range(0, sizeof(StarLines) / sizeof(StarLine)), pos, 1);
    markEntToBeDeleted(entId);
}

// spawns a line of stars. Takes as parameters a type of star line, its
// position, and the frequency with which it spawns
void spawnStarLine(int starLineType, PositionMini pos, u32 freq) {
    s16 entId = reserveEntSlot();
    if (entId == -1) return;
    StarLine sL = StarLines[starLineType];
    addComponentGroup(entId, 0, NULL, doGroupActionStarLine, 0, ENT_ITEM);
    addComponentInputChecker(entId, KEY_DIR, setGroupCompCounterToZero);
    addComponentTimer(entId, 0, 4 * 60, handleStarLineTimerExpire);
    // the below walks through the provided array of stars and spawns each one
    // until either it reaches the end of the array or reaches a value of zero (early end)
    // int starId = spawnStar(pos.x << 16, pos.y << 16);
    // addComponentMember(starId, 0, entId);
    PositionMini* posPtr = (PositionMini*)&sL;
    int i = 0;
    while (*(u32*)posPtr != UINT32_MAX && i++ < MAX_STARLINE_LENGTH) {
        int starId = spawnStar((pos.x + *(s16*)posPtr) << 16, (pos.y + *((s16*)posPtr + 1)) << 16);
        addComponentMember(starId, 0, entId);
        posPtr++;
    }
}

int spawnStar(int x, int y) {
    if ((numComps(COMP_OBJ) >= MAX_OBJ_COMPONENTS) ||
        (numComps(COMP_PHYSICS_SIMPLE) >= MAX_PHYSICS_SIMPLE_COMPONENTS) ||
        (numComps(COMP_HITBOX) >= MAX_HITBOX_COMPONENTS))
        return -1;
    s16 entId = reserveEntSlot();
    if (entId == -1) return -1;
    ObjComponent* objComp = addComponentObj(entId, 0, COMP_PHYSICS_SIMPLE);
    getObj(objComp)->attr0 |= ATTR0_4BPP;
    getObj(objComp)->attr2 |= ATTR2_ID(fetchSprite(sprite1Tiles, sprite1TilesLen)) | ATTR2_PALBANK(0) | ATTR2_PRIO(1);
    HitboxComponent hBox = { {entId, 0}, {12, 8, 0, 0} };
    addComponentCustom(&hBox, COMP_HITBOX);
    int width = obj_get_width(getObj(objComp));
    int height = obj_get_height(getObj(objComp));
    SimplePhysicsComponent phys = {
        {entId, 0},
        {
            (SWord)(clamp(x >> 16, 0 + width / 2, SCREEN_WIDTH - width / 2) << 16),
            (SWord)(clamp(y >> 16, 0 + height / 2, SCREEN_HEIGHT - height / 2) << 16)
        },
        (PhysArchetype*)&gPhysArchetypesStatic[ARCHETYPE_ITEM],
        {{0}, {0}},
        0xF000
    };
    addComponentCustom(&phys, COMP_PHYSICS_SIMPLE);
    return entId;
}
