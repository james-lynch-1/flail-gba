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

enum __attribute__ ((__packed__)) PhysArchetypeEnum {
    ARCHETYPE_PLAYER,
    ARCHETYPE_WEAK_ENEMY,
    ARCHETYPE_ITEM,
    NUM_PHYS_ARCHETYPES
};

enum __attribute__ ((__packed__)) EntityKind {
    ENT_PLAYER,
    ENT_ENEMY_WEAK,
    ENT_ENEMY,
    ENT_ITEM,
    ENT_CUSTOM,
    NUM_ENT_KINDS
};

enum __attribute__ ((__packed__)) ComponentType {
    COMP_OBJ,
    COMP_OBJ_AFF,
    COMP_DEBUG_BLOB,
    COMP_TILE,
    COMP_INPUT,
    COMP_AUDIO,
    COMP_PHYSICS,
    COMP_PHYSICS_SIMPLE,
    COMP_HITBOX,
    COMP_AI_RAND,
    COMP_TIMER,
    COMP_COUNTER,
    COMP_SPAWNER,
    COMP_MEMBER,
    COMP_GROUP,
    COMP_INPUT_CHECKER,
    NUM_COMP_TYPES
};

enum __attribute__ ((__packed__)) Type {
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
};

enum __attribute__ ((__packed__)) Objective {
    SURVIVE,
    ROUT,
    DEFEND,
    DEFEND_BOSS,
    CHILL
};

enum __attribute__ ((__packed__)) PaletteEnum {
    PLAYER_PAL,
    ENEMY_WEAK_PAL,
    ENEMY_STRONG_PAL,
    ITEM_PAL
};

typedef const u16* PaletteSet[16];

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

enum __attribute__ ((__packed__)) GameState {
    NORMAL,
    TITLE,
    PAUSE
};

typedef struct gGameState { // for game states (FSM)
    enum GameState gameStateEnum;
    void (*enterFunction)();
    void (*exitFunction)(enum GameState state);
    void (*updateFunction)();
} GameState;

// Events and Listeners

typedef struct Event_ {
    u32 flags;
} Event;

typedef struct EventListener_ {
    u32 flags;
    void (*callback)(void* data);
} EventListener;

// stuff with only one instance

typedef struct Viewport_ {
    s16 x, xMin, xMax,
        y, yMin, yMax;
} Viewport;

// stuff for entities

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

enum Direction {
    EAST = 0,
    NORTHEAST = 0x2000,
    NORTH = 0x4000,
    NORTHWEST = 0x6000,
    WEST = 0x8000,
    SOUTHWEST = 0xA000,
    SOUTH = 0xC000,
    SOUTHEAST = 0xE000,
    STATIONARY = 0x10000
};

typedef struct ComponentHeader_ {
    s16 entId;
    // u8 componentType;
    u16 flags;
} ComponentHeader;

typedef struct ObjComponent_ {
    ComponentHeader header; // 4 bytes
    OBJ_ATTR* obj; // 4 bytes
    u8 posSourceCompType; // 1 byte. The componentType of the obj holding the ent's position
} ObjComponent;

typedef struct ObjAffComponent_ {
    ComponentHeader header; // 4 bytes
    OBJ_AFFINE* objAff; // 4 bytes
} ObjAffComponent;

typedef struct DebugBlobComponent_ {
    ComponentHeader header; // 4 bytes. we store posSourceCompType in the flags
    Hitbox hitbox; // 4 bytes
    u16* allocatedSprite; // 4 bytes
    u16 originalAttr2; // 2 bytes. To restore the original sprite when we remove the debug blob
    u8 originalSize;
    u8 originalShape;
    u8 size;
    u8 shape;
} DebugBlobComponent;

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
    // void (*onCollide)(struct PhysicsComponent_);
    u16 angle; // 2 bytes
} PhysicsComponent;

typedef struct SimplePhysicsComponent_ {
    ComponentHeader header; // 4 bytes
    Position pos; // 8 bytes. should always be after header for updateObjs()
    PhysArchetype* archetype; // 4 bytes. should always be after header and pos for updateObjs()
    Vector vec; // 8 bytes
    u16 angle; // 2 bytes
} SimplePhysicsComponent;

typedef struct HitboxComponent_ {
    ComponentHeader header; // 4 bytes
    // u8 width; // 1 byte
    // u8 height; // 1 byte
    // s8 xOffset; // 1 byte. Whole hitbox offset on x axis
    // s8 yOffset; // 1 byte. Whole hitbox offset on y axis
    Hitbox hitbox;
} HitboxComponent;

typedef struct AiRandComponent_ {
    ComponentHeader header; // 4 bytes
} AiRandComponent;

typedef struct TimerComponent_ {
    ComponentHeader header; // 4 bytes
    const u16 time; // 2 bytes
    u16 timeRemaining; // 2 bytes
    void(*callback)(); // 4 bytes
} TimerComponent;

typedef struct CounterComponent_ {
    ComponentHeader header; // 4 bytes
    u16 curr; // 2 bytes
    u16 max; // 2 bytes
} CounterComponent;

typedef struct SpawnerComponent_ {
    ComponentHeader header; // 4 bytes
    Position pos; // 8 bytes
    u16 spawnerBehaviourIdx; // 2 bytes
    u16 frequency; // 2 bytes. How often it spawns
    u16 timer; // 2 bytes
    enum EntityKind entKind; // 1 byte
} SpawnerComponent;

// member uses the header flags to determine what ent kind it is
typedef struct MemberComponent_ {
    ComponentHeader header; // 4 bytes
    s8 groupIds[MAX_GROUPS_PER_MEMBER]; // 4 bytes. Can be in up to 4 groups. Each id defaults to -1
} MemberComponent;

typedef struct GroupComponent_ {
    ComponentHeader header; // 4 bytes
    u16 memberIds[MAX_MEMBERS_PER_GROUP]; // 48 bytes - max 24 members
    void (*onCollect)(struct MemberComponent_*, struct GroupComponent_*); // 4 bytes
    u16 counter; // 2 bytes
    u8 numCollected; // 1 byte
    u8 numMembers; // 1 byte
} GroupComponent;

typedef struct InputCheckerComponent_ {
    ComponentHeader header; // 4 bytes
    void (*onCheckedKeyPressed)(struct InputCheckerComponent_*); // 4 bytes
    u16 keysToCheck; // 2 bytes
} InputCheckerComponent;

#endif
