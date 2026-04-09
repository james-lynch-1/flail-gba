#ifndef TASK
#define TASK

#include "global.h"

bool addTask(void (*task)(int), int entId, int length);

void doTasks();

void removeTask(int taskIndex);

#endif
