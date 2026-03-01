#ifndef COMPONENT
#define COMPONENT

#include "global.h"
#include "mem.h"

#include "enemy.h"
#include "state.h"

ComponentHeader* addComponentCustom(void* data, ComponentType componentType);

/** Moves last component in list to the slot of the one being removed.
Updates the corresponding entry in gCompSetSparse[componentType] to -1.
Decrements gNumCompsPerType[componentType]. */
void removeComponent(int index, int componentType);

bool hasComponent(s16 entId, int componentType);

/** Gets the first component of a given type on an entity */
void* getComponent(s16 entId, int componentType);

void* getComponentFromDenseIndex(int denseIndex, int componentType);

void removeObjComponent(s16 entId);

/** the idea is you can get a component's type by checking if it is in the
range of memory of a particular dense set.
as we iterate through gDenseSetAddresses, the addresses get smaller.
so, for a given component address, if it is greater than the current
dense set address, it is a component of that type. */
ComponentType getComponentType(ComponentHeader* compPtr);

void initialiseComponentArrays();

/** Returns -1 if no slot available */
int reserveEntSlot();

void markEntToBeDeleted(int index);

int deleteEnt(int index);

void deleteMarkedEnts();

// individual components
// obj

ObjComponent* addComponentObj(s16 entId, u16 flags, int posSourceCompType);

void updateObjs();

// ai

void updateAiRand();

// archetypes

int addPhysArchetypeCustom(void* data);

void initialiseArchetypeArrays();

// input

void handleInputNormal(s16 entId);

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

// timer

void addComponentTimer(s16 entId, u16 flags, u32 length, void(*callback));

void updateTimers();

// group

/** Helper function for creating a group component */
void addComponentGroup(s16 entId, u16 flags, s16* memberIds, void (*onCollect)(struct MemberComponent_*), int numMembers);

// member

/** Helper function for creating a member component. Assumes group exists */
void addComponentMember(s16 entId, u16 flags, int groupId);

#endif
