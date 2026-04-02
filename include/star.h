#ifndef STAR
#define STAR

#include "global.h"
#include "entity.h"
#include "component.h"

// A star line is an array of mini position structs. Each element
// is an offset from the first star. Therefore, spawning any given
// star pattern at position p means that the first star will be at
// p and all the other star's positions are given as offsets to
// the first one.
extern const StarLine StarLines[];

void timerCallback();

void removeStarLine(int entId);

void doGroupActionStarLine(MemberComponent * m, GroupComponent * g);

void spawnStarLineRandomPos(int entId);

void spawnStarLine(int starLineType, PositionMini pos, u32 freq);

int spawnStar(int x, int y);

#endif
