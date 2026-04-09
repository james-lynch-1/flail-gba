#include "task.h"

extern void scaleUpAffComp(int entId);
extern void scaleDownAffComp(int entId);

const TaskData gTaskData[] = {};

bool addTask(void (*task)(int), int entId, int length) {
    if (gNumTasks >= MAX_TASKS) return false;
    Task t = { task, length, entId };
    gTasks[gNumTasks++] = t;
    return true;
}

void doTasks() {
    for (int i = 0; i < gNumTasks; i++) {
        gTasks[i].task(gTasks->entId);
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
