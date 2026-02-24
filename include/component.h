#ifndef COMPONENT
#define COMPONENT

#include "global.h"
#include "mem.h"

#include "enemy.h"
#include "state.h"

ComponentHeader* addComponentCustom(int componentType, void* data);

void removeComponent(int index, int componentType);

void* getComponent(int index, int componentType);

void removeObjComponent(u32 entId);

void initialiseComponentArrays();

int reserveEntSlot();

void markEntToBeDeleted(int index);

int deleteEnt(int index);

void deleteMarkedEnts();

// individual components
// obj

ObjComponent* addObjComponent(u32 entId, int posSourceCompType);

void updateObjs();

// ai

void updateAiRand();

// archetypes

int addPhysArchetypeCustom(void* data);

void initialiseArchetypeArrays();

// input

void handleInputNormal(int entIndex);

void updateInputComps();

// physics

void updatePhysics();

void updatePhysicsSimple();

bool checkPhysToSimplePhysCollision(PhysicsComponent* ent, SimplePhysicsComponent* simpleEnt);

Vector decaySpeed(Vector vec, int rate);

u32 fastMagnitude(int x, int y);

Vector addVec(Vector vec1, Vector vec2);

Vector divVec(Vector vec, u32 divisor);

Vector normaliseVec(Vector vec);

Vector scalarMultVec(Vector vec, int scalar);

#endif
