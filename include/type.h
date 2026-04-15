#ifndef TYPE
#define TYPE

#include "tonc.h"
#include "constants.h"

// ent flags
#define ENT_ACTIVE      0b1000000000000000 // active
#define ENT_PERSIST     0b0100000000000000 // persists when off screen
#define ENT_AFFINE      0b0010000000000000 // uses affine object

// obj allocation defines

enum __attribute__ ((__packed__)) ObjSlotEnum {
    OBJ_SLOT_UNUSED,
    OBJ_SLOT_USED,
    OBJ_SLOT_CONTINUE
};

// archetype stuff

enum __attribute__ ((__packed__)) PhysArchetypeEnum {
    ARCHETYPE_PLAYER,
    ARCHETYPE_ENEMY_WEAK,
    ARCHETYPE_ENEMY,
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
    COMP_AI,
    COMP_AI_RAND,
    COMP_TIMER,
    COMP_COUNTER,
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
    PAUSE,
    GAMEOVER,
    NUM_GAME_STATES
};

typedef struct gGameState { // for game states (FSM)
    enum GameState gameStateEnum;
    void (*enterFunction)();
    void (*exitFunction)(enum GameState state);
    void (*updateFunction)();
    bool justChanged;
} GameState;

// Tasks

typedef struct Task_ {
    void (*task)(int entId);
    s16 timeRemaining;
    s16 entId;
} Task;

typedef struct ALIGN4 TaskData_ {
    void (*fn)(int entId);
    s16 length;
} TaskData;

// Events and Listeners

typedef struct Event_ {
    u32 flags;
} Event;

typedef struct EventListener_ {
    u32 flags;
    void (*callback)(int entId);
    bool isRemovedAfterCallback;
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
    s16 x;
    s16 y;
} PositionMini;

// data for spawning a star line, not the in-game object.
// Terminated by a {UINT16_MAX, UINT16_MAX} position
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
    int length; // in frames
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

typedef struct ALIGN4 ComponentHeader_ {
    s16 entId;
    u16 flags;
} ComponentHeader;

typedef struct LutStruct_ {
    const s16* lut;
    int length;
    int minX;
    int period;
} LutStruct;

#define OBJ_REG_FLAG            ATTR0_REG
#define OBJ_AFF_FLAG            ATTR0_AFF
#define OBJ_HIDE_FLAG           ATTR0_HIDE
#define OBJ_AFF_DBL_FLAG        ATTR0_AFF_DBL

typedef struct ALIGN4 ObjComponent_ {
    ComponentHeader header; // 4 bytes
    u8 objIndex; // 1 byte. Index into gObjBuffer
    u8 posSourceCompType; // 1 byte. The componentType of the obj holding the ent's position
    u8 numAnimFrames; // 1 byte
    u8 animSpeed; // 1 byte
} ObjComponent;

typedef struct ALIGN4 ObjAffComponent_ {
    ComponentHeader header; // 4 bytes
    u8 animIndex; // UINT8_MAX means no animation
    u8 currentStep; // UINT8_MAX means no stages
    u8 speed;
    u8 framesElapsed;
    const LutStruct* lutStruct;    
} ObjAffComponent;


#define OBJ_AFF_LUT_SML         0b0000000100000000
#define OBJ_AFF_LUT_LRG         0b0000001000000000
#define OBJ_AFF_LUT_SIZE_MASK   0b0000001100000000

#define ANIM_NO_RESET           0b0000010000000000

#define ANIM_ROTATE             0b0000000000001000
#define ANIM_SCALE_X            0b0000000000010000
#define ANIM_SCALE_Y            0b0000000000100000
#define ANIM_SHEAR_X            0b0000000001000000
#define ANIM_SHEAR_Y            0b0000000010000000
#define ANIM_POS_MASK           0b0000000011111000

#define ANIM_ROTATE_NEG         0b0000100000001000
#define ANIM_SCALE_X_NEG        0b0001000000010000
#define ANIM_SCALE_Y_NEG        0b0010000000100000
#define ANIM_SHEAR_X_NEG        0b0100000001000000
#define ANIM_SHEAR_Y_NEG        0b1000000010000000
#define ANIM_NEG_MASK           0b1111100000000000

typedef const struct AnimationStep_ {
    const LutStruct* lutStruct;
    const u16 flags;
    const u16 length;
} AnimationStep;

typedef struct Animation_ {
    const AnimationStep animStep[4];
    const int numSteps;
} Animation;

typedef struct ALIGN4 DebugBlobComponent_ {
    ComponentHeader header; // 4 bytes. we store posSourceCompType in the flags
    Hitbox hitbox; // 4 bytes
    u16* allocatedSprite; // 4 bytes
    u16 originalAttr2; // 2 bytes. To restore the original sprite when we remove the debug blob
    u8 originalSize;
    u8 originalShape;
    u8 size;
    u8 shape;
} DebugBlobComponent;

typedef struct ALIGN4 TileComponent_ {
    ComponentHeader header;
    int tileIndex;
} TileComponent;

typedef struct ALIGN4 AudioComponent_ {
    ComponentHeader header;
} AudioComponent;

typedef struct ALIGN4 InputComponent_ {
    ComponentHeader header;
    void (*inputHandler)(s16 entId);
} InputComponent;

#define PHYS_GRAVITY_FLAG        0b10000000
#define CUSTOM_ARCHETYPE_FLAG    0b00000001

typedef struct ALIGN4 PhysArchetype_ {
    Hitbox hitbox; // 4 bytes
    int radius; // 4 bytes
    SWord accel; // 4 bytes
    u16 decay; // 2 bytes
    u16 inUse; // 2 bytes
} PhysArchetype;

typedef struct ALIGN4 PhysicsComponent_ {
    ComponentHeader header; // 4 bytes
    Position pos; // 8 bytes. should always be after header for updateObjs()
    PhysArchetype* archetype; // 4 bytes. should always be after header and pos for updateObjs()
    Vector vec; // 8 bytes
    u16 angle; // 2 bytes
} PhysicsComponent;

typedef struct ALIGN4 SimplePhysicsComponent_ {
    ComponentHeader header; // 4 bytes
    Position pos; // 8 bytes. should always be after header for updateObjs()
    PhysArchetype* archetype; // 4 bytes. should always be after header and pos for updateObjs()
    Vector vec; // 8 bytes
    u16 angle; // 2 bytes
} SimplePhysicsComponent;

typedef struct ALIGN4 HitboxComponent_ {
    ComponentHeader header; // 4 bytes
    Hitbox hitbox;
} HitboxComponent;

typedef struct ALIGN4 AiComponent_ {
    ComponentHeader header; // 4 bytes
} AiComponent;

typedef struct ALIGN4 AiRandComponent_ {
    ComponentHeader header; // 4 bytes
} AiRandComponent;

#define TIMER_DELETE_ENT            0b1

typedef struct ALIGN4 TimerComponent_ {
    ComponentHeader header; // 4 bytes
    const u16 time; // 2 bytes
    u16 timeRemaining; // 2 bytes
    void(*callback)(int entId); // 4 bytes
} TimerComponent;

// UI components use these to decide what meter this counter is for
#define COUNTER_HEALTH_FLAG         0b1
#define COUNTER_POWER_FLAG          0b10
#define COUNTER_NUM_DEFEATED_FLAG   0b100

typedef struct ALIGN4 CounterComponent_ {
    ComponentHeader header; // 4 bytes
    s16 curr; // 2 bytes
    s16 max; // 2 bytes
    SWord incrementModifier; // 4 bytes
} CounterComponent;

// member uses the header flags to determine what ent kind it is
typedef struct ALIGN4 MemberComponent_ {
    ComponentHeader header; // 4 bytes
    s8 groupIds[MAX_GROUPS_PER_MEMBER]; // 4 bytes. Can be in up to 4 groups. Each id defaults to -1
} MemberComponent;

typedef struct ALIGN4 GroupComponent_ {
    ComponentHeader header; // 4 bytes
    s16 memberIds[MAX_MEMBERS_PER_GROUP]; // 48 bytes - max 24 members
    void (*onCollect)(struct MemberComponent_*, struct GroupComponent_*); // 4 bytes
    u16 counter; // 2 bytes
    u8 numCollected; // 1 byte
    u8 numMembers; // 1 byte
} GroupComponent;

typedef struct ALIGN4 InputCheckerComponent_ {
    ComponentHeader header; // 4 bytes
    void (*onCheckedKeyPressed)(int entId); // 4 bytes
    u16 keysToCheck; // 2 bytes
} InputCheckerComponent;

#endif
