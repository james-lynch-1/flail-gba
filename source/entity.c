#include "entity.h"

void updatePlayerPos(PlayerEnt* ent) {
    // get dpad vector and add to existing vector
    Vector directionVec = { {key_tri_horz() * ent->accel.WORD}, {key_tri_vert() * ent->accel.WORD} };
    bool centredAndSufficientlySlow =
        in_range(ent->position.x.HALF.HI, 119, 121) &&
        in_range(ent->position.y.HALF.HI, 79, 81) &&
        fastMagnitude(ent->vec.x.WORD, ent->vec.y.WORD) < 0x1800;
    ent->vec = addVec(ent->vec, directionVec);
    u32 mag = fastMagnitude(ent->vec.x.WORD, ent->vec.y.WORD);
    if (mag >> 16 > ent->radius) {
        ent->vec = normaliseVec(ent->vec);
        ent->vec = scalarMultVec(ent->vec, ent->radius);
    }

    // apply gravity vector, unless you're in the middle
    if ((gFlags & GFLAG_GRAVITY) && !centredAndSufficientlySlow) {
        Vector gravityVec = { {(120 << 16) - ent->position.x.WORD}, {(80 << 16) - ent->position.y.WORD} };
        gravityVec = normaliseVec(gravityVec);
        gravityVec = divVec(gravityVec, 4);
        ent->vec = addVec(ent->vec, gravityVec);
    }

    if (!directionVec.x.WORD && !directionVec.y.WORD)
        ent->vec = decaySpeed(ent->vec, 0xF800);

    ent->position.x.WORD += ent->vec.x.WORD;
    ent->position.y.WORD += ent->vec.y.WORD;

    // stop if centred and sufficiently slow
    if (centredAndSufficientlySlow && !directionVec.x.WORD && !directionVec.y.WORD) {
        ent->vec = scalarMultVec(ent->vec, 0);
        ent->position.x.WORD = 120 << 16;
        ent->position.y.WORD = 80 << 16;
    }

    if (ent->position.y.HALF.HI < 8) {
        ent->position.y.WORD = 8 << 16;
        ent->vec.y.WORD = 0;
    }
    else if (ent->position.y.HALF.HI >= 153) {
        ent->position.y.WORD = 152 << 16;
        ent->vec.y.WORD = 0;
    }
    if (ent->position.x.HALF.HI < 8) {
        ent->position.x.WORD = 8 << 16;
        ent->vec.x.WORD = 0;
    }
    else if (ent->position.x.HALF.HI >= 233) {
        ent->position.x.WORD = 232 << 16;
        ent->vec.x.WORD = 0;
    }
}

Direction getDirectionFromInput(int horz, int vert) {
    switch (horz) {
        case -1:
            switch (vert) {
                case -1:
                    return NORTHWEST;
                case 0:
                    return WEST;
                case 1:
                    return SOUTHWEST;
            }
        case 0:
            switch (vert) {
                case -1:
                    return NORTH;
                case 0:
                    return STATIONARY;
                case 1:
                    return SOUTH;
            }
        case 1:
            switch (vert) {
                case -1:
                    return NORTHEAST;
                case 0:
                    return EAST;
                case 1:
                    return SOUTHEAST;
            }
    }
    return STATIONARY;
}

void updateEntsNormal() {
    obj_set_pos(gPlayer.obj,
        gPlayer.position.x.HALF.HI - 8,
        gPlayer.position.y.HALF.HI - 8
    );
    for (int i = 0; i < MAX_ENTS; i++) {
        if (!(gEntities[i].flags & ENT_ACTIVE)) continue;
        if ((gEntities[i].invincibleTime == 0 &&
            checkPlayerEntCollision(&gPlayer, &gEntities[i])) ||
            gEntities[i].lifetime == 0)
            despawnEnemy(&gEntities[i]);
        else {
            if (gEntities[i].lifetime > 0) gEntities[i].lifetime--;
            if (gEntities[i].invincibleTime) --gEntities[i].invincibleTime;
            updateEnemyPosWeak(&gEntities[i]);
            obj_set_pos(gEntities[i].obj,
                gEntities[i].state.position.x.HALF.HI - obj_get_width(gEntities[i].obj) / 2,
                gEntities[i].state.position.y.HALF.HI - obj_get_height(gEntities[i].obj) / 2
            );
        }
        // obj_get_width() and obj_get_height() don't work for affine objects
    }
}

void updateEnemyPosWeak(Entity* ent) { // just move randomly
    if (--ent->turnFrequency == 0 ||
        !in_range(ent->state.position.x.HALF.HI, 16, SCREEN_WIDTH - 16) ||
        !in_range(ent->state.position.y.HALF.HI, 16, SCREEN_HEIGHT - 16)) {
        ent->angle += qran_range(-1, 2) * qran_range(0x1000, 0x2000);
        ent->turnFrequency = WEAK_TURN_FREQ;
    }
    ent->vec.x.WORD = ent->radius * lu_cos(ent->angle) << 4;
    ent->vec.y.WORD = ent->radius * -lu_sin(ent->angle) << 4;
    ent->state.position.x.WORD += ent->vec.x.WORD;
    ent->state.position.y.WORD += ent->vec.y.WORD;
    if (ent->state.position.y.HALF.HI < 8) {
        ent->state.position.y.WORD = 8 << 16;
        ent->vec.y.WORD = 0;
    }
    else if (ent->state.position.y.HALF.HI >= 153) {
        ent->state.position.y.WORD = 152 << 16;
        ent->vec.y.WORD = 0;
    }
    if (ent->state.position.x.HALF.HI < 8) {
        ent->state.position.x.WORD = 8 << 16;
        ent->vec.x.WORD = 0;
    }
    else if (ent->state.position.x.HALF.HI >= 233) {
        ent->state.position.x.WORD = 232 << 16;
        ent->vec.x.WORD = 0;
    }
}

bool checkPlayerEntCollision(PlayerEnt* player, Entity* ent) {
    // return in_range(player->position.x.HALF.HI,
    //     ent->state.position.x.HALF.HI - ent->hitbox.width / 2,
    //     ent->state.position.x.HALF.HI + ent->hitbox.width / 2) &&
    //     in_range(player->position.y.HALF.HI,
    //         ent->state.position.y.HALF.HI - ent->hitbox.height / 2,
    //         ent->state.position.y.HALF.HI + ent->hitbox.height / 2);
    return ((player->position.x.HALF.HI - player->hitbox.width / 2) <=
        (ent->state.position.x.HALF.HI + ent->hitbox.width / 2 + 1)) &&
        ((player->position.x.HALF.HI + player->hitbox.width / 2 - 1) >=
            (ent->state.position.x.HALF.HI - ent->hitbox.width / 2)) &&
        ((player->position.y.HALF.HI - player->hitbox.height / 2) <=
            (ent->state.position.y.HALF.HI + ent->hitbox.height / 2)) &&
        ((player->position.y.HALF.HI + player->hitbox.height / 2 - 1) >=
            (ent->state.position.y.HALF.HI - ent->hitbox.height / 2));
}

Entity* spawnEnemy(int x, int y, u8 ilk) {
    Entity* ent = gFirstAvailableEntSlot;
    if (ent) {
        gFirstAvailableEntSlot = gFirstAvailableEntSlot->state.next;
        ent->angle = qran_range(0, 65535);
        ent->turnFrequency = WEAK_TURN_FREQ;
        ent->lifetime = -1;
        ent->invincibleTime = 60;
        ent->flags |= ENT_ACTIVE;
        ent->state.position.x.HALF.HI = x;
        ent->state.position.y.HALF.HI = y;
        ent->ilk = ilk;
        ent->obj->attr0 = ATTR0_REG | ATTR0_4BPP | ATTR0_SQUARE;
        ent->obj->attr1 = ATTR1_SIZE_8x8;
        ent->obj->attr2 = ATTR2_ID(4) | ATTR2_PRIO(2) | ATTR2_PALBANK(1);
        return ent;
    }
    return NULL;
}

void despawnEnemy(Entity* ent) {
    ent->flags &= ~ENT_ACTIVE;
    obj_hide(ent->obj);
    ent->state.position.y.WORD = 0;
    ent->state.next = gFirstAvailableEntSlot;
    gFirstAvailableEntSlot = ent;
}

/* component stuff now
--------------------------------------------------------------
--------------------------------------------------------------
--------------------------------------------------------------
-------------------------------------------------------------- */



// int spawnEnemyWeak(Position pos) {
//     // obj, physics, ai?
//     ObjComponent objComp = {
//         {}
//     }
// }

// int spawnEnt(int )
