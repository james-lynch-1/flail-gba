#include "component.h"
#include "log.h"

void updateDebugBlobs() {
    for (int i = 0; i < numComps(COMP_DEBUG_BLOB); i++) {
        DebugBlobComponent* blob = &gDebugBlobCompsDense[i];
        ObjComponent* objComp = getComponent(blob->header.entId, COMP_OBJ);
        ComponentHeader* posSrcComp = getComponent(blob->header.entId, objComp->posSourceCompType);
        Position pos = { {0}, {0} };
        pos = ((PhysicsComponent*)posSrcComp)->pos;
        objComp->obj->attr0 = ATTR0_REG | (blob->shape << ATTR0_SHAPE_SHIFT) | (ATTR0_Y(pos.y.HALF.HI - blob->hitbox.height / 2) & ATTR0_Y_MASK);
        objComp->obj->attr1 = (blob->size << ATTR1_SIZE_SHIFT) | (ATTR1_X(pos.x.HALF.HI - blob->hitbox.width / 2) & ATTR1_X_MASK);
        objComp->obj->attr2 = ATTR2_ID(blob->header.flags) | ATTR2_PALBANK(0);
        if (!in_range(pos.x.HALF.HI, 0 - blob->hitbox.width / 2, SCREEN_WIDTH + blob->hitbox.height / 2) ||
            !in_range(pos.y.HALF.HI, 0 - blob->hitbox.width / 2, SCREEN_HEIGHT + blob->hitbox.height / 2))
            gObjCompsDense[i].obj->attr0 |= ATTR0_HIDE;
        else
            gObjCompsDense[i].obj->attr0 &= ~ATTR0_AFF_DBL;
    }
}

DebugBlobComponent* addComponentDebugBlob(int entId) {
    // attach onto another ent that has an obj(aff) comp and a hitbox/phys comp
    // in each update first look at the obj(aff) comp to get the posSourceCompType
    // then look at that comp to get the position
    // then update the position of the debug blob
    // this blob should sit with the other objs i guess, but how to update it?
    // debug blobs should exist starting from the index gNumCompsPerType(COMP_OBJ)
    // and have its own counter. Blobs only spawn if there is enough space
    u32 width, height;
    ObjComponent* objComp = getComponent(entId, COMP_OBJ);
    DebugBlobComponent* existingBlob = getComponent(entId, COMP_DEBUG_BLOB);
    if (!objComp || existingBlob) return NULL;
    DebugBlobComponent blob = { {entId, 0} };
    PhysicsComponent* phys = getComponent(entId, COMP_PHYSICS);
    if (phys) {
        blob.hitbox = phys->archetype->hitbox;
        width = blob.hitbox.width;
        height = blob.hitbox.height;
    }
    else {
        HitboxComponent* hBox = getComponent(entId, COMP_HITBOX);
        if (hBox) {
            blob.hitbox = hBox->hitbox;
            width = blob.hitbox.width;
            height = blob.hitbox.height;
        }
        else return NULL;
    }
    blob.originalAttr2 = objComp->obj->attr2;
    blob.originalSize = objComp->obj->attr1 >> ATTR1_SIZE_SHIFT;
    blob.originalShape = objComp->obj->attr0 >> ATTR0_SHAPE_SHIFT;
    blob.size = getAppropriateSpriteSize(blob.hitbox.width, blob.hitbox.height);
    blob.shape = getAppropriateSpriteShape(blob.hitbox.width, blob.hitbox.height);
    blob.allocatedSprite = drawDebugBlob(width, height);
    blob.header.flags = fetchSprite(blob.allocatedSprite, nextPow2(width) * nextPow2(height) / 2);
    objComp->obj->attr0 = (blob.shape << ATTR0_SHAPE_SHIFT) | (objComp->obj->attr0 & ATTR0_Y_MASK);
    objComp->obj->attr1 = (blob.size << ATTR1_SIZE_SHIFT) | (objComp->obj->attr1 & ATTR1_X_MASK);
    objComp->obj->attr2 = ATTR2_ID(blob.header.flags) | ATTR2_PALBANK(0);
    DebugBlobComponent* ptr = (DebugBlobComponent*)addComponentCustom(&blob, COMP_DEBUG_BLOB);
    return ptr;
}

void removeComponentDebugBlob(int entId) {
    DebugBlobComponent* blob = getComponent(entId, COMP_DEBUG_BLOB);
    if (!blob) return;
    numComps(COMP_DEBUG_BLOB)--;
    ObjComponent* objComp = getComponent(entId, COMP_OBJ);
    stopUsingSprite(objComp->obj->attr2 & ATTR2_ID_MASK);
    objComp->obj->attr2 = blob->originalAttr2;
    objComp->obj->attr1 &= ~ATTR1_SIZE_MASK;
    objComp->obj->attr1 |= blob->originalSize << ATTR1_SIZE_SHIFT;
    objComp->obj->attr0 &= ~ATTR0_SHAPE_MASK;
    objComp->obj->attr0 |= blob->originalShape << ATTR0_SHAPE_SHIFT;
    free(blob->allocatedSprite);
    removeComponent(entId, COMP_DEBUG_BLOB);
}

u16* drawDebugBlob(u32 width, u32 height) {
    if (width > 64 || width == 0 || height > 64 || height == 0) return NULL;
    // actual sprite size rounding to nearest power of 2
    u32 blobDimension[2] = { nextPow2(width), nextPow2(height) };
    // u16 is 4 px
    int sprSize = (blobDimension[0] * blobDimension[1]) / 2; // 4 bits per pixel
    u16* spr = malloc(sprSize);
    u16* sprPtr = spr;
    int numTileRows = blobDimension[1] >> 3;
    int numTileCols = blobDimension[0] >> 3;
    for (int tileY = 0; tileY < numTileRows; tileY++) { // each iter is a row of tiles
        int tileHeight = clamp(height - (tileY * 8), 0, 9);
        for (int tileX = 0; tileX < numTileCols; tileX++) { // each iter is a tile
            u16* sprPtrNextTile = (u16*)((uint32_t)sprPtr + 32);
            int tileWidth = clamp(width - (tileX * 8), 0, 9);
            if (tileWidth > 0) {
                for (int y = 0; y < tileHeight; y++) { // each iter is a tile row
                    u32 pxRow8 = 0;
                    for (int x = 0; x < tileWidth; x++) { // each iter is a pixel
                        pxRow8 |= 1 << (x * 4);
                    }
                    u32* u32Ptr = (u32*)sprPtr;
                    *u32Ptr = pxRow8;
                    sprPtr += 2;
                }
            }
            sprPtr = sprPtrNextTile;
        }
    }
    return spr;
}
