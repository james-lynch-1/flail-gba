#ifndef STAR
#define STAR

#include "main.h"
#include "type.h"
#include "log.h"

// a star pattern is an array of mini position structs. Each element
// is an offset from the first star. Therefore, spawning any given
// star pattern at position p means that the first star will be at
// p and all the other star's positions are given as offsets to
// the first one.

extern const StarLine StarLines[];

void timerCallback();

void addStarLine(int starLineType, PositionMini pos, u32 freq);

// a star line has timer, progress, simplephysics components

// MiniPosition patt[] = {{0, 5}};
// int x = sizeof(MiniPosition);

#endif
