#include "task.h"

extern void scaleUpAffComp(int entId);
extern void scaleDownAffComp(int entId);

void spawnParticlesEnemyDefeat(PhysicsComponent* enemyPhys) {
    for (int i = 0; i < 32; i++) {
        int id = reserveEntSlot();
        if (id == -1) break;
        int angle = i * (0xFFFF >> 5);
        addComponentSimplePhysics(id, 0, enemyPhys->pos.x.WORD, enemyPhys->pos.y.WORD, ARCHETYPE_PARTICLE, lu_cos(angle) << 4, -lu_sin(angle) << 4);
        addComponentTimer(id, 0, 120, markEntToBeDeleted);
        ObjComponent* obj = addComponentObj(id, 0, COMP_PHYSICS_SIMPLE);
        getObj(obj)->attr2 = fetchSprite(dotTiles, dotTilesLen) | ATTR2_PALBANK(2);
    }
}

void taskExplodeWhenWallIsHit(Task* task, int entId) {
    PhysicsComponent* phys = getComponent(entId, COMP_PHYSICS);
    if (!in_range(phys->pos.x.HALF.HI, 0, SCREEN_WIDTH) ||
        !in_range(phys->pos.y.HALF.HI, 0, SCREEN_HEIGHT)) {
        // delete this fella
        markEntToBeDeleted(entId);
        spawnParticlesEnemyDefeat(phys);
        task->timeRemaining = 0;
    }
}

bool addTask(void (*task)(Task*, int), int entId, int length) {
    if (gNumTasks >= MAX_TASKS) return false;
    Task t = { task, length, entId };
    gTasks[gNumTasks++] = t;
    return true;
}

void doTasks() {
    for (int i = 0; i < gNumTasks; i++) {
        gTasks[i].task(&gTasks[i], gTasks->entId);
        if (--gTasks[i].timeRemaining <= 0) removeTask(i);
    }
}

void removeTask(int taskIndex) {
    if (gNumTasks == taskIndex + 1) {
        memset(&gTasks[--gNumTasks], 0, sizeof(Task));
        return;
    }
    gTasks[taskIndex] = gTasks[--gNumTasks];
}
