#ifndef PHYSICS
#define PHYSICS

#include "component.h"

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
