#include "component.h"

void updateSpawners() {
    
}

void addComponentSpawner(int entId, u16 flags, int x, int y, enum EntityKind entKind) {
    // SpawnerComponent s = {{entId, flags}, {{x}, {y}}, entKind, };
}

void removeComponentSpawner(int entId) {
    removeComponent(entId, COMP_SPAWNER);
}
