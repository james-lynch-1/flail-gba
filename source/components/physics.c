#include "physics.h"

// first use the input or ai components to add a vec to this vec, then clamp and decay speed, and apply gravity
void updatePhysics() {
    for (int i = 0; i < gNumCompsPerType[COMP_PHYSICS]; i++) {
        PhysicsComponent* ent = &gPhysCompsDense[i];
        bool isGravityOn = (gFlags & GFLAG_GRAVITY) && (ent->header.flags & PHYS_GRAVITY_FLAG);
        u32 mag = fastMagnitude(ent->vec.x.WORD, ent->vec.y.WORD);
        bool centredAndSufficientlySlow =
            in_range(ent->pos.x.HALF.HI, 119, 121) &&
            in_range(ent->pos.y.HALF.HI, 79, 81) &&
            mag < 0x1800;

        ent->vec = decaySpeed(ent->vec, 0x10000 - ent->archetype->decay);

        if ((mag >> 16) > ent->archetype->radius) {
            ent->vec = normaliseVec(ent->vec);
            ent->vec = scalarMultVec(ent->vec, ent->archetype->radius);
        }
        if (isGravityOn && !centredAndSufficientlySlow) {
            Vector gravityVec = { {(120 << 16) - ent->pos.x.WORD},
                                  {(80 << 16) - ent->pos.y.WORD} };
            gravityVec = normaliseVec(gravityVec);
            gravityVec = divVec(gravityVec, 4);
            ent->vec = addVec(ent->vec, gravityVec);
        }

        // stop if centred and sufficiently slow
        if (centredAndSufficientlySlow && isGravityOn) {
            // ent->vec = scalarMultVec(ent->vec, 0);
            ent->vec.x.WORD = 0;
            ent->vec.y.WORD = 0;
            ent->pos.x.WORD = 120 << 16;
            ent->pos.y.WORD = 80 << 16;
        }
        if (!isGravityOn && mag < 0x1800 && !(key_is_down(KEY_UP | KEY_DOWN | KEY_LEFT | KEY_RIGHT))) {
            ent->vec.x.WORD = 0;
            ent->vec.y.WORD = 0;
        }

        // for (int i = 0; i < gNumCompsPerType[COMP_PHYSICS_SIMPLE]; i++) {
        //     if (checkPhysToSimplePhysCollision(ent, &(gSimplePhysCompsDense[i]))) {
        //         markEntToBeDeleted(gSimplePhysCompsDense[i].header.entIndex);
        //     }
        // }

        if (!in_range(ent->pos.x.HALF.HI, 0 + ent->archetype->hitbox.width / 2, SCREEN_WIDTH - ent->archetype->hitbox.width / 2))
            ent->vec.x.WORD += (reflect(ent->pos.x.HALF.HI, 0, SCREEN_WIDTH) - ent->pos.x.HALF.HI) << 14;
        if (!in_range(ent->pos.y.HALF.HI, 0 + ent->archetype->hitbox.height / 2, SCREEN_HEIGHT - ent->archetype->hitbox.height / 2))
            ent->vec.y.WORD += (reflect(ent->pos.y.HALF.HI, 0, SCREEN_HEIGHT) - ent->pos.y.HALF.HI) << 14;
        ent->pos.x.WORD += ent->vec.x.WORD;
        ent->pos.y.WORD += ent->vec.y.WORD;
    }
}

// for enemies. doesn't use arctan2 to normalise vec.
// just changes vec using angle and adds vec to position
void updatePhysicsSimple() {
    for (int i = 0; i < gNumCompsPerType[COMP_PHYSICS_SIMPLE]; i++) {
        SimplePhysicsComponent* ent = &gSimplePhysCompsDense[i];
        ent->vec.x.WORD = ent->archetype->radius *
            lu_cos(ent->angle) << 4;
        ent->vec.y.WORD = ent->archetype->radius *
            -lu_sin(ent->angle) << 4;
        if (!in_range(ent->pos.x.HALF.HI, 0 + ent->archetype->hitbox.width / 2, SCREEN_WIDTH - ent->archetype->hitbox.width / 2))
            ent->vec.x.WORD += (reflect(ent->pos.x.HALF.HI, 0, SCREEN_WIDTH) - ent->pos.x.HALF.HI) << 14;
        if (!in_range(ent->pos.y.HALF.HI, 0 + ent->archetype->hitbox.height / 2, SCREEN_HEIGHT - ent->archetype->hitbox.height / 2))
            ent->vec.y.WORD += (reflect(ent->pos.y.HALF.HI, 0, SCREEN_HEIGHT) - ent->pos.y.HALF.HI) << 14;
        ent->pos.x.WORD += ent->vec.x.WORD;
        ent->pos.y.WORD += ent->vec.y.WORD;
        // TODO: clamp movement to screen
    }
}

bool checkPhysToSimplePhysCollision(PhysicsComponent* ent, SimplePhysicsComponent* simpleEnt) {
    return ((ent->pos.x.HALF.HI + ent->archetype->hitbox.width / 2) > simpleEnt->pos.x.HALF.HI) &&
        ((ent->pos.x.HALF.HI - ent->archetype->hitbox.width / 2) < simpleEnt->pos.x.HALF.HI) &&
        ((ent->pos.y.HALF.HI + ent->archetype->hitbox.height / 2) > simpleEnt->pos.y.HALF.HI) &&
        ((ent->pos.y.HALF.HI - ent->archetype->hitbox.height / 2) < simpleEnt->pos.y.HALF.HI);
}

// utils

Vector decaySpeed(Vector vec, int rate) {
    vec.x.WORD = ((s64)rate * vec.x.WORD) >> 16;
    vec.y.WORD = ((s64)rate * vec.y.WORD) >> 16;
    return vec;
}

// https://en.m.wikipedia.org/wiki/Alpha_max_plus_beta_min_algorithm
u32 fastMagnitude(int x, int y) {
    u32 max = MAX(ABS(x), ABS(y));
    u32 min = MIN(ABS(x), ABS(y));
    return (((u64)max * 0b1111010111011111) >> 16) + ((u64)min * 0b0110010111011000 >> 16);
}

Vector addVec(Vector vec1, Vector vec2) {
    Vector vec = { {vec1.x.WORD + vec2.x.WORD}, {vec1.y.WORD + vec2.y.WORD} };
    return vec;
}

// uses lu_div tonc_math function to perform a reciprocal multiplication (0 ≤ x ≤ 255)
Vector divVec(Vector vec, u32 divisor) {
    vec.x.WORD *= lu_div(divisor);
    vec.y.WORD *= lu_div(divisor);
    vec.x.WORD >>= 16;
    vec.y.WORD >>= 16;
    return vec;
}

Vector normaliseVec(Vector vec) {
    u32 angle = ArcTan2(vec.x.WORD >> 12, -vec.y.WORD >> 12);
    Vector normVec = { {(lu_cos(angle)) << 4}, {(-lu_sin(angle)) << 4} };
    return normVec;
}

Vector scalarMultVec(Vector vec, int scalar) {
    Vector newVec = { {vec.x.WORD * scalar}, {vec.y.WORD * scalar} };
    return newVec;
}
