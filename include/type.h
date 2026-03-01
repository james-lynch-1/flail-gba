#ifndef TYPE
#define TYPE

#include "tonc.h"
#include "constants.h"

// ent flags
#define ENT_ACTIVE      0b1000000000000000 // active
#define ENT_PERSIST     0b0100000000000000 // persists when off screen
#define ENT_AFFINE      0b0010000000000000 // uses affine object

// obj allocation defines

#define OBJ_SLOT_UNUSED     0
#define OBJ_SLOT_USED       1
#define OBJ_SLOT_CONTINUE   2

// archetype stuff

enum PhysArchetype {
    ARCHETYPE_PLAYER,
    ARCHETYPE_WEAK_ENEMY,
    NUM_PHYS_ARCHETYPES
};

typedef enum {
    COMP_OBJ,
    COMP_OBJ_AFF,
    COMP_TILE,
    COMP_INPUT,
    COMP_AUDIO,
    COMP_PHYSICS,
    COMP_PHYSICS_SIMPLE,
    COMP_AI_RAND,
    COMP_TIMER,
    COMP_COUNTER,
    COMP_SPAWNER,
    COMP_MEMBER,
    COMP_GROUP,
    NUM_COMP_TYPES
} ComponentType;

typedef enum {
    CHAR,
    DIRECTION,
    PTR,
    INT,
    INTHEX,
    U8,
    S16,
    S32,
    U16,
    U16HEX,
    U32,
    U32HEX,
    SPLITWORD,
    USPLITWORD
} Type;

enum Objective {
    SURVIVE,
    ROUT,
    DEFEND,
    DEFEND_BOSS,
    CHILL
};

enum PaletteEnum {
    PLAYER_PAL,
    ENEMY_WEAK_PAL,
    ENEMY_STRONG_PAL,
    ITEM_PAL
};

typedef const u16* PaletteSet[16];

enum EntityIlk {
    STR_0, // harmless
    STR_1, // weak
    STR_2, // med
    STR_3, // strong
};

// fixed point number types

typedef union SplitWord {
    s32 WORD;
    u32 WORD_U;
    struct {
        s16 LO, HI;
    } HALF;
    struct {
        u16 LO, HI;
    } HALF_U;
    struct {
        u8 byte0, byte1, byte2, byte3;
    } BYTES;
} SWord;

typedef union SplitHWord {
    s16 HWORD;
    u16 HWORD_U;
    struct {
        s8 LO : 4;
        u16 HI : 12;
    } HALF;
    struct {
        u8 LO : 4;
        u16 HI : 12;
    } HALF_U;
    struct {
        u8 byte1, byte2;
    } BYTES;
} SHWord;

// stuff for the game loop

typedef enum GameStateEnum_ {
    NORMAL,
    TITLE,
    PAUSE
} GameStateEnum;

typedef struct gGameState { // for game states (FSM)
    GameStateEnum gameStateEnum;
    void (*enterFunction)();
    void (*exitFunction)(GameStateEnum state);
    void (*updateFunction)();
} GameState;

// stuff with only one instance

typedef struct Viewport_ {
    s16 x, xMin, xMax,
        y, yMin, yMax;
} Viewport;

// stuff for entities

typedef s16 entId;

typedef struct Position_ {
    SWord x;
    SWord y;
} Position;

typedef struct PositionMini_ {
    u8 x;
    u8 y;
} PositionMini;

// data for spawning a star line, not the in-game object.
// Terminated by a [255, 255] position
typedef struct StarLine_ {
    PositionMini starPos[MAX_STARLINE_LENGTH];
} StarLine;

typedef struct Hitbox_ {
    u8 width;
    u8 height;
    s8 xOffset; // whole hitbox offset on x axis
    s8 yOffset; // whole hitbox offset on y axis
} Hitbox;

typedef struct Vector_ {
    SWord x;
    SWord y;
} Vector;

#define ILK_ENEMY   0
#define ILK_ITEM    1

typedef struct PlayerEnt_ {
    // shadow OAM entries
    OBJ_ATTR* obj; // 4 bytes
    OBJ_AFFINE* obj_aff; // 4 bytes

    // important
    Position position; // 8 bytes
    Hitbox hitbox; // 8 bytes
    int health; // 4 bytes
    int radius; // 4 bytes. Top speed
    SWord accel; // 4 bytes
    Vector vec; // 8 bytes
    int angleOffset; // 4 bytes
    u16 angle; // 2 bytes
    u16 angleVisual; // 2 bytes
    u16 remoteControlCountdown; // 2 bytes. No. of frames before control is handed back to the player. 65535 means indefinite
    u8 invincibleTime; // 1 byte
    u8 toBeDeleted; // 1 byte

    // visual
    u8 animationState; // 1 byte
    u8 animFrames; // 1 byte. how many frames ent has been in current animation state
} PlayerEnt;

typedef struct Entity_ {
    // this union to be used for the position when object is active,
    // and a ptr to the next free ent when object is inactive
    union {
        Position position;
        struct Entity_* next;
    } state; // 8 bytes
    Hitbox hitbox; // 8 bytes

    // shadow OAM entries
    OBJ_ATTR* obj; // 4 bytes
    OBJ_AFFINE* obj_aff; // 4 bytes

    // ai stuff
    u8 turnFrequency;

    int health; // 4 bytes
    int radius; // 4 bytes. Top speed
    SWord accel; // 4 bytes
    Vector vec; // 8 bytes
    int angleOffset; // 4 bytes
    u16 angle; // 2 bytes
    u16 angleVisual; // 2 bytes
    s16 lifetime; // 2 bytes
    u16 invincibleTime; // 2 bytes

    // 13-0: unused
    u16 flags; // 2 bytes
    u8 ilk; // 1 byte. 0: player, 1: enemy

    // visual
    u8 animationState; // 1 byte
    u8 animFrames; // 1 byte. how many frames ent has been in current animation state
} Entity;

typedef struct SpriteAllocList_ {
    const u16* tileData; // 4 bytes
    u16 arrIndex; // 2 bytes
    u16 numUsing; // 2 bytes
} SpriteAllocList;

typedef struct BGData_ {
    const u16* pal; // may have to store multiple palettes actually, for time of day changes
    int palLen;
    const u16* tiles;
    int tilesLen;
    const u16* map;
    int mapLen;
} BGData;

typedef struct Encounter_ {
    BGData* bgData;
    PaletteSet* paletteSet;
    int length; // in seconds
    void* music;
    enum Objective objective;
    int spawnScheduleIndex;
} Encounter;

typedef struct EncounterSet_ {
    int numSubCollections;
    struct EncounterSet* sets;
    // Scene* scenes;
} EncounterSet;

typedef enum Direction_ {
    EAST = 0,
    NORTHEAST = 0x2000,
    NORTH = 0x4000,
    NORTHWEST = 0x6000,
    WEST = 0x8000,
    SOUTHWEST = 0xA000,
    SOUTH = 0xC000,
    SOUTHEAST = 0xE000,
    STATIONARY = 0x10000
} Direction;

typedef struct ComponentHeader_ {
    s16 entId;
    // u8 componentType;
    u16 flags;
} ComponentHeader;

typedef struct ObjComponent_ {
    ComponentHeader header; // 4 bytes
    OBJ_ATTR* obj; // 4 bytes
    u8 posSourceCompType; // 2 bytes. The componentType of the obj holding the ent's position
} ObjComponent;

typedef struct ObjAffComponent_ {
    ComponentHeader header; // 4 bytes
    OBJ_AFFINE* objAff; // 4 bytes
} ObjAffComponent;

typedef struct TileComponent_ {
    ComponentHeader header;
    int tileIndex;
} TileComponent;

typedef struct AudioComponent_ {
    ComponentHeader header;
} AudioComponent;

typedef struct InputComponent_ {
    ComponentHeader header;
    void (*inputHandler)(s16 entId);
} InputComponent;

#define PHYS_GRAVITY_FLAG        0b10000000
#define CUSTOM_ARCHETYPE_FLAG    0b00000001

typedef struct PhysArchetype_ {
    Hitbox hitbox; // 4 bytes
    int radius; // 4 bytes
    SWord accel; // 4 bytes
    u16 decay; // 2 bytes
    u16 inUse; // 2 bytes
} PhysArchetype;

typedef struct PhysicsComponent_ {
    ComponentHeader header; // 4 bytes
    Position pos; // 8 bytes. should always be after header for updateObjs()
    PhysArchetype* archetype; // 4 bytes. should always be after header and pos for updateObjs()
    Vector vec; // 8 bytes
    void (*onCollide)(struct PhysicsComponent_);
    u16 angle; // 2 bytes
} PhysicsComponent;

typedef struct SimplePhysicsComponent_ {
    ComponentHeader header; // 4 bytes
    Position pos; // 8 bytes. should always be after header for updateObjs()
    PhysArchetype* archetype; // 4 bytes. should always be after header and pos for updateObjs()
    Vector vec; // 8 bytes
    u16 angle; // 2 bytes
} SimplePhysicsComponent;

typedef struct AiRandComponent_ {
    ComponentHeader header; // 4 bytes
} AiRandComponent;

typedef struct TimerComponent_ {
    ComponentHeader header; // 4 bytes
    u32 time; // 4 bytes
    void(*callback)(); // 4 bytes
} TimerComponent;

typedef struct CounterComponent_ {
    ComponentHeader header; // 4 bytes
    u16 counter; // 2 bytes
    u16 goal; // 2 bytes
    void(*callback)(); // 4 bytes
} CounterComponent;

typedef struct SpawnerComponent_ {
    ComponentHeader header; // 4 bytes
    Position pos; // 8 bytes
    u16 entType; // 2 bytes
    u16 spawnerBehaviourIdx; // 2 bytes
    u16 frequency; // 2 bytes
    u16 timer; // 2 bytes
} SpawnerComponent;

// member flags, to denote the kind of ent the member is,
// and to denote whether the member has been collected
#define MEMBER_ITEM             0b0000000000000001u
#define MEMBER_ENEMY            0b0000000000000010u
#define MEMBER_PLAYER           0b0000000000000100u
#define MEMBER_TIMER            0b0000000000001000u
#define MEMBER_GROUP            0b0000000000010000u
#define MEMBER_IS_COLLECTED     0b1000000000000000u

typedef struct MemberComponent_ {
    ComponentHeader header; // 4 bytes
    int groupId; // 4 bytes
} MemberComponent;

typedef struct GroupComponent_ {
    ComponentHeader header; // 4 bytes
    u16 memberIds[24]; // 48 bytes - max 24 members
    void (*onCollect)(struct MemberComponent_*); // 4 bytes
    u8 numCollected; // 1 byte
    u8 numMembers; // 1 byte
} GroupComponent;

// star line entity has multiple star components, each with the same entId (obvs)
// 

#endif
