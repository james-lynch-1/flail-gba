#ifndef GLOBAL
#define GLOBAL

// basically every other header file includes this one; it has the constants, globals and types

#include "tonc.h"
#include <stdlib.h>
#include <string.h>

#include "type.h"
#include "log.h"
#include "util.h"

// CONSTANTS

#define ANGLE_MIN           0
#define ANGLE_MAX           65535
#define SPEED_DECAY         0x800
#define MAX_ENTS            128
#define MAX_OBJ_COMPONENTS              128
#define MAX_OBJ_AFF_COMPONENTS          32
#define MAX_TILE_COMPONENTS             32
#define MAX_INPUT_COMPONENTS            32
#define MAX_AUDIO_COMPONENTS            32
#define MAX_PHYSICS_COMPONENTS          32
#define MAX_SIMPLE_PHYSICS_COMPONENTS   128
#define MAX_AI_RAND_COMPONENTS          128

#define MAX_PHYS_ARCHETYPES_STATIC      32
#define MAX_PHYS_ARCHETYPES_CUSTOM      32

#define MAX_ALLOC_SPRITES               128

// time (in frames) after laying an egg in which you cannot hurt each other
#define LAY_INVULN_TIME     60

// enemy-related constants
#define WEAK_TURN_FREQ      20

// Metatile width, in pixels
#define MT_WIDTH            16
#define SBB_WIDTH_MT        16
#define MT_PER_SBB          256
#define TILE_WIDTH          8
#define SBB_WIDTH_T         32
#define SBB_HEIGHT_T        32
#define TILES_PER_SBB       1024

// SBB defines
#define UI_SBB              26
#define MAP_SBB             31

// global flags (gFlags)
#define GFLAG_GRAVITY       0x80000000

// ent flags (in gEntFlags)
#define ENT_TO_DELETE           0b10000000
#define ENT_IN_USE              0b00000001

extern int gFrameCount;
extern int gNumEnts;
extern int gNumCustomPhysArchetypes;
extern int gNumSpritesAllocated;
extern int gNumEntsToDelete;

extern OBJ_ATTR gObjBuffer[128];
extern OBJ_AFFINE* gObjAffBuffer;
extern u8 gObjAllocArr[1024];
extern SpriteAllocList gSpriteAllocList[MAX_ALLOC_SPRITES];
extern u8 gEntsToDelete[MAX_ENTS];

extern GameState gGameState;
extern u32 gFlags;
extern PlayerEnt gPlayer;
extern Entity* gEntities;
extern Entity* gFirstAvailableEntSlot;
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
extern AiRandComponent gAiRandCompsDense[MAX_AI_RAND_COMPONENTS];

// physics component archetypes
extern const PhysArchetype gPhysArchetypesStatic[MAX_PHYS_ARCHETYPES_STATIC];
extern PhysArchetype gPhysArchetypesCustom[MAX_PHYS_ARCHETYPES_CUSTOM];

extern const void* gDenseSetAddresses[NUM_COMP_TYPES];
extern const int gCompSizes[NUM_COMP_TYPES];
extern int gNumCompsPerType[NUM_COMP_TYPES];
extern const int gMaxCompsPerType[NUM_COMP_TYPES];

// sparse sets for components. each element is an index into the dense array
extern s16 gCompSetSparse[NUM_COMP_TYPES][MAX_ENTS];

#endif
