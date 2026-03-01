#ifndef COMPONENT
#define COMPONENT

#include "global.h"
#include "mem.h"

#include "enemy.h"
#include "state.h"

ComponentHeader* addComponentCustom(void* data, ComponentType componentType);

/** Moves last component in list to the slot of the one being removed.
 * Updates the corresponding entry in gCompSetSparse[componentType] to -1.
 * Decrements gNumCompsPerType[componentType].
 */
void removeComponent(int entId, int componentType);

bool hasComponent(s16 entId, int componentType);

/** Gets the first component of a given type on an entity */
void* getComponent(s16 entId, int componentType);

void* getComponentFromDenseIndex(int denseIndex, int componentType);

/** The idea is you can get a component's type by checking if it is in the
 * range of memory of a particular dense set.
 * as we iterate through gDenseSetAddresses, the addresses get smaller.
 * so, for a given component address, if it is greater than the current
 * dense set address, it is a component of that type.
 */
ComponentType getComponentType(ComponentHeader* compPtr);

void initialiseComponentArrays();

// COMPONENTS
// Obj

ObjComponent* addComponentObj(s16 entId, u16 flags, int posSourceCompType);

/** Special component remove function for objs, as each obj component has
 * a pointer into the obj buffer. This means that if we just leave the data
 * there, we end up with junk. So we have to explicitly set the attrs to 0.
 */
void removeComponentObj(s16 entId);

void updateObjs();

// Obj Aff

void removeComponentObjAff(int entId);

// Tile

void removeComponentTile(int entId);

// Input

void handleInputNormal(s16 entId);

void updateInputComps();

void removeComponentInput(int entId);

// Audio

void removeComponentAudio(int entId);

// Phys archetypes

int addPhysArchetypeCustom(void* data);

void initialiseArchetypeArrays();

// Physics

void updatePhysics();

void updatePhysicsSimple();

bool checkPhysToSimplePhysCollision(PhysicsComponent* ent, SimplePhysicsComponent* simpleEnt);

Vector decaySpeed(Vector vec, int rate);

u32 fastMagnitude(int x, int y);

Vector addVec(Vector vec1, Vector vec2);

Vector divVec(Vector vec, u32 divisor);

Vector normaliseVec(Vector vec);

Vector scalarMultVec(Vector vec, int scalar);

void removeComponentPhysics(int entId);
void removeComponentPhysicsSimple(int entId);

// Ai

void updateAiRand();

void removeComponentAiRand(int entId);

// Timer

void addComponentTimer(s16 entId, u16 flags, u32 length, void(*callback));

void updateTimers();

void removeComponentTimer(int entId);

// Spawner

void removeComponentSpawner(int entId);

// Member

/** Helper function for creating a member component. Assumes group exists */
MemberComponent* addComponentMember(s16 entId, u16 flags, int groupId);

/** Removes a member component and also removes the reference to it in its
 * parent group, if there is one.
 * We remove the id from the group's array by assigning the id at the last spot
 * in the array to the to-be-deleted id's spot and decrementing numMembers by 1.
 */
void removeComponentMember(int entId);

// Group

/** Helper function for creating a group component */
GroupComponent* addComponentGroup(s16 entId, u16 flags, s16* memberIds, void (*onCollect)(struct MemberComponent_*), int numMembers, int entKind);

void removeComponentGroup(int entId);

#endif
