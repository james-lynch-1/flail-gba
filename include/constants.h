#ifndef CONSTANTS
#define CONSTANTS

#define ANGLE_MIN           0
#define ANGLE_MAX           65535
#define SPEED_DECAY         0x800
#define MAX_ENTS            128

#define MAX_EVENT_LISTENERS_PER_TYPE    32

#define MAX_OBJ_COMPONENTS              128
#define MAX_OBJ_AFF_COMPONENTS          32
#define MAX_DEBUG_BLOB_COMPONENTS       128
#define MAX_TILE_COMPONENTS             32
#define MAX_INPUT_COMPONENTS            32
#define MAX_AUDIO_COMPONENTS            32
#define MAX_PHYSICS_COMPONENTS          32
#define MAX_PHYSICS_SIMPLE_COMPONENTS   128
#define MAX_HITBOX_COMPONENTS           128
#define MAX_AI_COMPONENTS               16
#define MAX_AI_RAND_COMPONENTS          128
#define MAX_TIMER_COMPONENTS            128
#define MAX_COUNTER_COMPONENTS            128
#define MAX_SPAWNER_COMPONENTS          128
#define MAX_MEMBER_COMPONENTS           128
#define MAX_GROUP_COMPONENTS            8
#define MAX_INPUT_CHECKER_COMPONENTS    8

#define MAX_GROUPS_PER_MEMBER           4
#define MAX_MEMBERS_PER_GROUP           24

#define MAX_PHYS_ARCHETYPES_STATIC      32
#define MAX_PHYS_ARCHETYPES_CUSTOM      32

#define MAX_STARLINE_LENGTH             32

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

#endif
