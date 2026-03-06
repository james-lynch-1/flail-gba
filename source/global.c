#include "global.h"

extern int spawnEnemyWeak(int x, int y);
extern int spawnPlayer(int x, int y);

extern void removeComponentObj(int entId);
extern void removeComponentObjAff(int entId);
extern void removeComponentDebugBlob(int entId);
extern void removeComponentTile(int entId);
extern void removeComponentInput(int entId);
extern void removeComponentAudio(int entId);
extern void removeComponentPhysics(int entId);
extern void removeComponentPhysicsSimple(int entId);
extern void removeComponentHitbox(int entId);
extern void removeComponentAiRand(int entId);
extern void removeComponentTimer(int entId);
extern void removeComponentCounter(int entId);
extern void removeComponentSpawner(int entId);
extern void removeComponentMember(int entId);
extern void removeComponentGroup(int entId);
extern void removeComponentInputChecker(int entId);

int gFrameCount = 0;
int gNumEnts = 0;
int gNumCustomPhysArchetypes = 0;
int gNumSpritesAllocated = 0;
int gNumEntsToDelete = 0;

s16 gPlayerId;

OBJ_ATTR gObjBuffer[128];
OBJ_AFFINE* gObjAffBuffer = (OBJ_AFFINE*)gObjBuffer;
u8 gObjAllocArr[1024];
SpriteAllocList gSpriteAllocList[MAX_ALLOC_SPRITES];
u8 gEntFlags[MAX_ENTS];
u8 gEntsToDelete[MAX_ENTS];

GameState gGameState;
u32 gFlags = GFLAG_GRAVITY;
enum Direction gDPadDir = STATIONARY;

// // events and listeners
int gNumListenersPerType[NUM_COMP_TYPES];
EventListener gEventListeners[NUM_COMP_TYPES][MAX_EVENT_LISTENERS_PER_TYPE];

// components
EWRAM_BSS ObjComponent gObjCompsDense[MAX_OBJ_COMPONENTS];
EWRAM_BSS ObjAffComponent gObjAffCompsDense[MAX_OBJ_AFF_COMPONENTS];
EWRAM_BSS DebugBlobComponent gDebugBlobCompsDense[MAX_DEBUG_BLOB_COMPONENTS];
EWRAM_BSS TileComponent gTileCompsDense[MAX_TILE_COMPONENTS];
EWRAM_BSS InputComponent gInputCompsDense[MAX_INPUT_COMPONENTS];
EWRAM_BSS AudioComponent gAudioCompsDense[MAX_AUDIO_COMPONENTS];
EWRAM_BSS PhysicsComponent gPhysCompsDense[MAX_PHYSICS_COMPONENTS];
EWRAM_BSS SimplePhysicsComponent gSimplePhysCompsDense[MAX_SIMPLE_PHYSICS_COMPONENTS];
EWRAM_BSS HitboxComponent gHitboxCompsDense[MAX_HITBOX_COMPONENTS];
EWRAM_BSS AiRandComponent gAiRandCompsDense[MAX_AI_RAND_COMPONENTS];
EWRAM_BSS TimerComponent gTimerCompsDense[MAX_TIMER_COMPONENTS];
EWRAM_BSS CounterComponent gCounterCompsDense[MAX_COUNTER_COMPONENTS];
EWRAM_BSS SpawnerComponent gSpawnerCompsDense[MAX_SPAWNER_COMPONENTS];
EWRAM_BSS MemberComponent gMemberCompsDense[MAX_MEMBER_COMPONENTS];
EWRAM_BSS GroupComponent gGroupCompsDense[MAX_GROUP_COMPONENTS];
EWRAM_BSS InputCheckerComponent gInputCheckerCompsDense[MAX_INPUT_CHECKER_COMPONENTS];

const uint32_t gCompTable[NUM_COMP_TYPES][4] = {
    {(uint32_t)&gObjCompsDense, sizeof(ObjComponent), MAX_OBJ_COMPONENTS, (uint32_t)removeComponentObj},
    {(uint32_t)&gObjAffCompsDense, sizeof(ObjAffComponent), MAX_OBJ_AFF_COMPONENTS, (uint32_t)removeComponentObjAff},
    {(uint32_t)&gDebugBlobCompsDense, sizeof(DebugBlobComponent), MAX_DEBUG_BLOB_COMPONENTS, (uint32_t)removeComponentDebugBlob},
    {(uint32_t)&gTileCompsDense, sizeof(TileComponent), MAX_TILE_COMPONENTS, (uint32_t)removeComponentTile},
    {(uint32_t)&gInputCompsDense, sizeof(InputComponent), MAX_INPUT_COMPONENTS, (uint32_t)removeComponentInput},
    {(uint32_t)&gAudioCompsDense, sizeof(AudioComponent), MAX_AUDIO_COMPONENTS, (uint32_t)removeComponentAudio},
    {(uint32_t)&gPhysCompsDense, sizeof(PhysicsComponent), MAX_PHYSICS_COMPONENTS, (uint32_t)removeComponentPhysics},
    {(uint32_t)&gSimplePhysCompsDense, sizeof(SimplePhysicsComponent), MAX_SIMPLE_PHYSICS_COMPONENTS, (uint32_t)removeComponentPhysicsSimple},
    {(uint32_t)&gHitboxCompsDense, sizeof(HitboxComponent), MAX_HITBOX_COMPONENTS, (uint32_t)removeComponentHitbox},
    {(uint32_t)&gAiRandCompsDense, sizeof(AiRandComponent), MAX_AI_RAND_COMPONENTS, (uint32_t)removeComponentAiRand},
    {(uint32_t)&gTimerCompsDense, sizeof(TimerComponent), MAX_TIMER_COMPONENTS, (uint32_t)removeComponentTimer},
    {(uint32_t)&gCounterCompsDense, sizeof(CounterComponent), MAX_COUNTER_COMPONENTS, (uint32_t)removeComponentCounter},
    {(uint32_t)&gSpawnerCompsDense, sizeof(SpawnerComponent), MAX_SPAWNER_COMPONENTS, (uint32_t)removeComponentSpawner},
    {(uint32_t)&gMemberCompsDense, sizeof(MemberComponent), MAX_MEMBER_COMPONENTS, (uint32_t)removeComponentMember},
    {(uint32_t)&gGroupCompsDense, sizeof(GroupComponent), MAX_GROUP_COMPONENTS, (uint32_t)removeComponentGroup},
    {(uint32_t)&gInputCheckerCompsDense, sizeof(InputCheckerComponent), MAX_INPUT_CHECKER_COMPONENTS, (uint32_t)removeComponentInputChecker}
};
int gNumCompsPerType[NUM_COMP_TYPES];

// physics component archetypes
const PhysArchetype gPhysArchetypesStatic[MAX_PHYS_ARCHETYPES_STATIC] = {
    { // player
    {16, 16, 0, 0},
    5,
    {0x00001000},
    SPEED_DECAY,
    true
    },
    { // weak enemy
    {8, 8, 0, 0},
    1,
    {0x00002000},
    0,
    true
    },
    { // item
    {8, 8, 0, 0},
    0,
    {0},
    0,
    true
    }
};
PhysArchetype gPhysArchetypesCustom[MAX_PHYS_ARCHETYPES_CUSTOM];

// sparse sets for components. each element is an index into the dense array
s16 gCompSetSparse[NUM_COMP_TYPES][MAX_ENTS];

// entities

/** Spawners for entities */
int (*gEntSpawners[NUM_ENT_KINDS])(int x, int y) = {
    spawnPlayer,
    spawnEnemyWeak,
    NULL, // enemy
    NULL // item
};
