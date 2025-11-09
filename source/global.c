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

const void* gDenseSetAddresses[NUM_COMP_TYPES] = {
    &gObjCompsDense,
    &gObjAffCompsDense,
    &gTileCompsDense,
    &gInputCompsDense,
    &gAudioCompsDense,
    &gPhysCompsDense,
    &gSimplePhysCompsDense,
    &gAiRandCompsDense
};

const int gCompSizes[NUM_COMP_TYPES] = {
    sizeof(ObjComponent),
    sizeof(ObjAffComponent),
    sizeof(TileComponent),
    sizeof(InputComponent),
    sizeof(AudioComponent),
    sizeof(PhysicsComponent),
    sizeof(SimplePhysicsComponent),
    sizeof(AiRandComponent)
};
int gNumCompsPerType[NUM_COMP_TYPES];
const int gMaxCompsPerType[NUM_COMP_TYPES] = {
    MAX_OBJ_COMPONENTS,
    MAX_OBJ_AFF_COMPONENTS,
    MAX_TILE_COMPONENTS,
    MAX_INPUT_COMPONENTS,
    MAX_AUDIO_COMPONENTS,
    MAX_PHYSICS_COMPONENTS,
    MAX_SIMPLE_PHYSICS_COMPONENTS,
    MAX_AI_RAND_COMPONENTS
};

// sparse sets for components. each element is an index into the dense array
s16 gCompSetSparse[NUM_COMP_TYPES][MAX_ENTS];
