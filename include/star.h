#ifndef STAR
#define STAR

#include "global.h"
#include "entity.h"
#include "component.h"
#include "eventListener.h"
#include "player.h"


extern const StarLine StarLines[];

void timerCallback();

void removeStarLine(int entId);

void doGroupActionStarLine(MemberComponent * m, GroupComponent * g);

void spawnStarLineRandomPos(int entId);

void spawnStarLine(int starLineType, PositionMini pos, u32 freq);

int spawnStar(int x, int y);

#endif
