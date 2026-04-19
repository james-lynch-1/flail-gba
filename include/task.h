#ifndef TASK
#define TASK

#include "global.h"

void spawnParticlesEnemyDefeat(PhysicsComponent* enemyPhys);

void taskExplodeWhenWallIsHit(Task* task, int entId);

bool addTask(void (*task)(Task*, int), int entId, int length);

void doTasks();

void removeTask(int taskIndex);

#endif
