#include "input.h"

// void handleInputNormal() {
    // key_poll();
    // int dpadTribools[2] = { key_tri_horz(), key_tri_vert() };
    // gDPadDir = getDir(dpadTribools, dPadDir);
// }

// Direction getDir(int direction[2], Direction dir) {
//     switch ((direction[0] + 2) + 3 * (direction[1] + 2)) {
//         case 9:     return EAST;                    ////////////
//         case 6:     return NORTHEAST;             //   0x4000   //
//         case 5:     return NORTH;                //0x6000  0x2000//
//         case 4:     return NORTHWEST;           //                //
//         case 7:     return WEST;                //0x8000 :)      0//(0x10000)
//         case 10:    return SOUTHWEST;           //                //
//         case 11:    return SOUTH;                //0xA000  0xE000//
//         case 12:    return SOUTHEAST;             //   0xC000   //
//         default:    return STATIONARY;              ////////////
//     }
// }
