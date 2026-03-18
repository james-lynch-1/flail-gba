#include "component.h"

void updateSpawners() {
    
}

void addComponentSpawner(int entId, u16 flags, enum EntityKind entKind) {
    // SpawnerComponent s = {{entId, flags}, entKind, };
}

void removeComponentSpawner(int entId) {
    removeComponent(entId, COMP_SPAWNER);
}
