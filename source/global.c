#include "global.h"

int gFrameCount = 0;
int gNumEnts = 0;
int gNumCustomPhysArchetypes = 0;
int gNumSpritesAllocated = 0;
int gNumEntsToDelete = 0;

OBJ_ATTR gObjBuffer[128];
OBJ_AFFINE* gObjAffBuffer = (OBJ_AFFINE*)gObjBuffer;
u8 gObjAllocArr[1024];
SpriteAllocList gSpriteAllocList[MAX_ALLOC_SPRITES];
u8 gEntFlags[MAX_ENTS];
u8 gEntsToDelete[MAX_ENTS];

GameState gGameState;
u32 gFlags = GFLAG_GRAVITY;
PlayerEnt gPlayer;
Entity* gEntities;
Entity* gFirstAvailableEntSlot;
Direction gDPadDir = STATIONARY;

u8 gEntFlags[MAX_ENTS];

// components

ObjComponent gObjCompsDense[MAX_OBJ_COMPONENTS];
ObjAffComponent gObjAffCompsDense[MAX_OBJ_AFF_COMPONENTS];
TileComponent gTileCompsDense[MAX_TILE_COMPONENTS];
InputComponent gInputCompsDense[MAX_INPUT_COMPONENTS];
AudioComponent gAudioCompsDense[MAX_AUDIO_COMPONENTS];
PhysicsComponent gPhysCompsDense[MAX_PHYSICS_COMPONENTS];
SimplePhysicsComponent gSimplePhysCompsDense[MAX_SIMPLE_PHYSICS_COMPONENTS];
AiRandComponent gAiRandCompsDense[MAX_AI_RAND_COMPONENTS];
TimerComponent gTimerCompsDense[MAX_TIMER_COMPONENTS];
CounterComponent gCounterCompsDense[MAX_COUNTER_COMPONENTS];
SpawnerComponent gSpawnerCompsDense[MAX_SPAWNER_COMPONENTS];
MemberComponent gMemberCompsDense[MAX_MEMBER_COMPONENTS];
GroupComponent gGroupCompsDense[MAX_GROUP_COMPONENTS];

const uint32_t gCompTable[NUM_COMP_TYPES][3] = {
    {(uint32_t)&gObjCompsDense, sizeof(ObjComponent), MAX_OBJ_COMPONENTS},
    {(uint32_t)&gObjAffCompsDense, sizeof(ObjAffComponent), MAX_OBJ_AFF_COMPONENTS},
    {(uint32_t)&gTileCompsDense, sizeof(TileComponent), MAX_TILE_COMPONENTS},
    {(uint32_t)&gInputCompsDense, sizeof(InputComponent), MAX_INPUT_COMPONENTS},
    {(uint32_t)&gAudioCompsDense, sizeof(AudioComponent), MAX_AUDIO_COMPONENTS},
    {(uint32_t)&gPhysCompsDense, sizeof(PhysicsComponent), MAX_PHYSICS_COMPONENTS},
    {(uint32_t)&gSimplePhysCompsDense, sizeof(SimplePhysicsComponent), MAX_SIMPLE_PHYSICS_COMPONENTS},
    {(uint32_t)&gAiRandCompsDense, sizeof(AiRandComponent), MAX_AI_RAND_COMPONENTS},
    {(uint32_t)&gTimerCompsDense, sizeof(TimerComponent), MAX_TIMER_COMPONENTS},
    {(uint32_t)&gCounterCompsDense, sizeof(CounterComponent), MAX_COUNTER_COMPONENTS},
    {(uint32_t)&gSpawnerCompsDense, sizeof(SpawnerComponent), MAX_SPAWNER_COMPONENTS},
    {(uint32_t)&gMemberCompsDense, sizeof(MemberComponent), MAX_MEMBER_COMPONENTS},
    {(uint32_t)&gGroupCompsDense, sizeof(GroupComponent), MAX_GROUP_COMPONENTS}
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
    }
};
PhysArchetype gPhysArchetypesCustom[MAX_PHYS_ARCHETYPES_CUSTOM];

// sparse sets for components. each element is an index into the dense array
s16 gCompSetSparse[NUM_COMP_TYPES][MAX_ENTS];

// entities. Is the order of all tables relating to different entities,
// e.g. spawner table

enum Entities_ {
    ENT_PLAYER,
    ENT_ENEMY,
    ENT_ITEM,
    NUM_ENTITIES
};
