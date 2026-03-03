#ifndef GLOBAL
#define GLOBAL

// basically every other header file includes this one; it has the constants, globals and types

#include "tonc.h"
#include <stdlib.h>
#include <string.h>

#include "type.h"
#include "constants.h"
#include "util.h"

extern int gFrameCount;
extern int gNumEnts;
extern int gNumCustomPhysArchetypes;
extern int gNumSpritesAllocated;
extern int gNumEntsToDelete;

extern s16 gPlayerId;

extern OBJ_ATTR gObjBuffer[128];
extern OBJ_AFFINE* gObjAffBuffer;
extern u8 gObjAllocArr[1024];
extern SpriteAllocList gSpriteAllocList[MAX_ALLOC_SPRITES];
extern u8 gEntsToDelete[MAX_ENTS];

extern GameState gGameState;
extern u32 gFlags;
extern Direction gDPadDir;

extern u8 gEntFlags[MAX_ENTS];

// components
extern ObjComponent gObjCompsDense[MAX_OBJ_COMPONENTS];
extern ObjAffComponent gObjAffCompsDense[MAX_OBJ_AFF_COMPONENTS];
extern TileComponent gTileCompsDense[MAX_TILE_COMPONENTS];
extern InputComponent gInputCompsDense[MAX_INPUT_COMPONENTS];
extern AudioComponent gAudioCompsDense[MAX_AUDIO_COMPONENTS];
extern PhysicsComponent gPhysCompsDense[MAX_PHYSICS_COMPONENTS];
extern SimplePhysicsComponent gSimplePhysCompsDense[MAX_SIMPLE_PHYSICS_COMPONENTS];
extern HitboxComponent gHitboxCompsDense[MAX_HITBOX_COMPONENTS];
extern AiRandComponent gAiRandCompsDense[MAX_AI_RAND_COMPONENTS];
extern TimerComponent gTimerCompsDense[MAX_TIMER_COMPONENTS];
extern SpawnerComponent gSpawnerCompsDense[MAX_SPAWNER_COMPONENTS];
extern MemberComponent gMemberCompsDense[MAX_MEMBER_COMPONENTS];
extern GroupComponent gGroupCompsDense [MAX_GROUP_COMPONENTS];

// physics component archetypes
extern const PhysArchetype gPhysArchetypesStatic[MAX_PHYS_ARCHETYPES_STATIC];
extern PhysArchetype gPhysArchetypesCustom[MAX_PHYS_ARCHETYPES_CUSTOM];

extern int gNumCompsPerType[NUM_COMP_TYPES];

/** Table with addresses of comp dense sets, sizes of comps, max comps per type and comp removers */
enum gCompTableIndexes { COMP_DSET_ADDRESSES, COMP_SIZES, COMP_MAX_PER_TYPE, COMP_REMOVERS };

#define denseSetAddr(x) (void*)gCompTable[x][COMP_DSET_ADDRESSES]
#define compSize(x) (int)gCompTable[x][COMP_SIZES]
#define maxComps(x) (int)gCompTable[x][COMP_MAX_PER_TYPE]
#define numComps(x) gNumCompsPerType[x]

extern const uint32_t gCompTable[NUM_COMP_TYPES][4];

// sparse sets for components. each element is an index into the dense array
extern s16 gCompSetSparse[NUM_COMP_TYPES][MAX_ENTS];

// entities

extern int (*gEntSpawners[NUM_ENT_KINDS])(int x, int y);

#endif
