#include "star.h"

const StarLine StarLines[] = { {
    {{25, -10}, {30, 25}}
} };

void timerCallback() {
    log(CHAR, "time!");
}

void incrementNumCollectedAndLog(MemberComponent* m, GroupComponent* g) {
    markEntToBeDeleted(m->header.entId);
    notify(m->header.entId, COMP_PHYSICS, E_PHYS_TOUCHED);
    g->numCollected++;
    if (g->numCollected == g->numMembers)
        log(CHAR, "All collected!");
}

// spawns a line of stars. Takes as parameters a type of star line, its
// position, and the frequency with which it spawns
void addStarLine(int starLineType, PositionMini pos, u32 freq) {
    s16 entId = reserveEntSlot();
    if (entId == -1) return;
    // TimerComponent t = { {entId, 0}, 60, NULL };
    // addComponentCustom(&t, COMP_TIMER);
    StarLine sL = StarLines[starLineType];

    addComponentGroup(entId, 0, NULL, incrementNumCollectedAndLog, 0, ENT_ITEM);
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

    // addComponentCustom();
}

int spawnStar(int x, int y) {
    if ((numComps(COMP_OBJ) >= MAX_OBJ_COMPONENTS) ||
        (numComps(COMP_PHYSICS_SIMPLE) >= MAX_PHYSICS_SIMPLE_COMPONENTS))
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
