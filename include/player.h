#ifndef PLAYER
#define PLAYER

#include "component.h"
#include "eventListener.h"
#include "enemy.h"

void handlePowerFull();

void endPowerfulMode();

void doPlayerHurtAnimation();

/** Spawns player at x, y (x and y are SplitWords) */
int spawnPlayer(int x, int y);

void updatePlayerStuff();

void handlePlayerDied();

#endif
