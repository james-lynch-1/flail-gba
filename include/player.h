#ifndef PLAYER
#define PLAYER

#include "component.h"

/** Spawns player at x, y (x and y are SplitWords) */
int spawnPlayer(int x, int y);

void updatePlayerStuff();

void handlePlayerDied();

#endif
