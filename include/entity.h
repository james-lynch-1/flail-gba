#ifndef ENTITY
#define ENTITY

#include "global.h"
#include "physics.h"

void updatePlayerPos(PlayerEnt * ent);

Direction getDirectionFromInput(int horzTribool, int vertTribool);

void updateEntsNormal();

void updateEnemyPosWeak(Entity* ent);

bool checkPlayerEntCollision(PlayerEnt* player, Entity* ent);

Entity* spawnEnemy(int x, int y, u8 ilk);

void despawnEnemy(Entity* ent);

#endif
