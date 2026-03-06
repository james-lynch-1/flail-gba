#include "component.h"

void logForInputChecker(InputCheckerComponent* inputChecker) {
    u16 thing = inputChecker->keysToCheck & key_curr_state();
    log(U16HEX, thing);
}

void updateInputCheckers() {
    for (int i = 0; i < numComps(COMP_INPUT_CHECKER); i++) {
        InputCheckerComponent* inputChecker = &gInputCheckerCompsDense[i];
        if (inputChecker->keysToCheck & key_curr_state())
            inputChecker->onCheckedKeyPressed(inputChecker);
    }
}

void addComponentInputChecker(int entId, u16 keys) {
    InputCheckerComponent iC = {{entId, 0}, logForInputChecker, keys};
    addComponentCustom(&iC, COMP_INPUT_CHECKER);
}

void removeComponentInputChecker(int entId) {
    removeComponent(entId, COMP_INPUT_CHECKER);
}
