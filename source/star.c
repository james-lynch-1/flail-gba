#include "star.h"

const StarLine StarLines[] = {
    {{{25, -10}, {30, 25}}},
    {{{25, 5}, {50, -10}}}
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
    if (power->curr != power->max) power->curr = 0;
    removeStarLine(entId);
    addComponentTimer(reserveEntSlot(), 0, 2, spawnStarLineRandomPos);
}

void doGroupActionStarLine(MemberComponent* m, GroupComponent* g) {
    removeComponentObj(m->header.entId);
    removeComponentPhysicsSimple(m->header.entId);
    removeComponentHitbox(m->header.entId);
    notify(m->header.entId, COMP_PHYSICS, E_STAR_COLLECTED);
    g->numCollected++;
    g->counter++; // this is the no. of stars collected without input
    if (g->numCollected == g->numMembers) {
        if (g->numMembers == g->counter) {
            CounterComponent* power = getCounterByFlags(gPlayerId, COUNTER_POWER_FLAG);
            if (power) power->curr = power->max;
        }
        removeStarLine(g->header.entId);
        addComponentTimer(reserveEntSlot(), 0, 2, spawnStarLineRandomPos);
    }
}

void spawnStarLineRandomPos(int entId) {
    PositionMini pos = { qran_range(40, 200), qran_range(40, 120) };
    addStarLine(qran_range(0, sizeof(StarLines) / sizeof(StarLine)), pos, 1);
    markEntToBeDeleted(entId);
}

// spawns a line of stars. Takes as parameters a type of star line, its
// position, and the frequency with which it spawns
void addStarLine(int starLineType, PositionMini pos, u32 freq) {
    s16 entId = reserveEntSlot();
    if (entId == -1) return;
    StarLine sL = StarLines[starLineType];
    addComponentGroup(entId, 0, NULL, doGroupActionStarLine, 0, ENT_ITEM);
    addComponentInputChecker(entId, KEY_DIR, setGroupCompCounterToZero);
    addComponentTimer(entId, 0, 3, handleStarLineTimerExpire);
    // the below walks through the provided array of stars and spawns each one
    // until either it reaches the end of the array or reaches a value of zero (early end)
    int starId = spawnStar(pos.x << 16, pos.y << 16);
    addComponentMember(starId, 0, entId);
    u16* posPtr = (u16*)&sL;
    int i = 0;
    while (*posPtr != 0 && i++ < MAX_STARLINE_LENGTH) {
        starId = spawnStar((*posPtr + pos.x) << 16, (*(u16*)(posPtr + 1) + pos.y) << 16);
        addComponentMember(starId, 0, entId);
        posPtr += 2;
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
    objComp->obj->attr0 |= ATTR0_4BPP;
    objComp->obj->attr2 |= ATTR2_ID(fetchSprite(sprite1Tiles, sprite1TilesLen)) | ATTR2_PALBANK(0);
    SimplePhysicsComponent phys = {
        {entId, 0},
        { (SWord)x, (SWord)y },
        (PhysArchetype*)&gPhysArchetypesStatic[ARCHETYPE_ITEM],
        {{0}, {0}},
        0xF000
    };
    addComponentCustom(&phys, COMP_PHYSICS_SIMPLE);
    HitboxComponent hBox = { {entId, 0}, {8, 8, 0, 0} };
    addComponentCustom(&hBox, COMP_HITBOX);
    return entId;
}
