#include "util.h"

u8 attr1SizesArr[4][4] = {
    {ATTR1_SIZE_8x8 >> ATTR1_SIZE_SHIFT, ATTR1_SIZE_8x16 >> ATTR1_SIZE_SHIFT, ATTR1_SIZE_8x32 >> ATTR1_SIZE_SHIFT, 0},
    {ATTR1_SIZE_16x8 >> ATTR1_SIZE_SHIFT, ATTR1_SIZE_16x16 >> ATTR1_SIZE_SHIFT, ATTR1_SIZE_16x32 >> ATTR1_SIZE_SHIFT, 0},
    {ATTR1_SIZE_32x8 >> ATTR1_SIZE_SHIFT, ATTR1_SIZE_32x16 >> ATTR1_SIZE_SHIFT, ATTR1_SIZE_32x32 >> ATTR1_SIZE_SHIFT, ATTR1_SIZE_32x64 >> ATTR1_SIZE_SHIFT},
    {0, 0, ATTR1_SIZE_64x32 >> ATTR1_SIZE_SHIFT, ATTR1_SIZE_64x64 >> ATTR1_SIZE_SHIFT}
};

int getAppropriateSpriteSize(int width, int height) {
    if (width > 64 || height > 64) return 0;
    int nextPowDimensions[2] = { nextPow2(width), nextPow2(height) };
    int arrIndexes[2] = { 0, 0 };
    for (int i = 0; i < 2; i++)
        for (int j = 3; j < 7; j++)
            if ((nextPowDimensions[i] >> j) & 1) {
                arrIndexes[i] = j - 3;
                break;
            }
    return (int)(attr1SizesArr[arrIndexes[0]][arrIndexes[1]]);
}

int getAppropriateSpriteShape(int width, int height) {
    if (!in_range(width, 0, 64) || !in_range(height, 0, 64)) return 0;
    int pow2Width = nextPow2(width);
    int pow2Height = nextPow2(height);
    if (pow2Width > pow2Height)
        return ATTR0_WIDE >> ATTR0_SHAPE_SHIFT;
    if (pow2Width == pow2Height)
        return ATTR0_SQUARE >> ATTR0_SHAPE_SHIFT;
    if (pow2Width < pow2Height)
        return ATTR0_TALL >> ATTR0_SHAPE_SHIFT;
    return ATTR0_SQUARE >> ATTR0_SHAPE_SHIFT;
}

void doNothing() {}

void logSomething() {
    log(CHAR, "something");
}

int nextPow2(int num) {
    int x = num - 1;
    x |= x >> 1;
    x |= x >> 2;
    x |= x >> 4;
    x |= x >> 8;
    x |= x >> 16;
    return x + 1;
}
