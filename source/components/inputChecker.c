#include "component.h"

void logForInputChecker(int entId) {
    InputCheckerComponent* inputChecker = getComponent(entId, COMP_INPUT_CHECKER);
    u16 thing = inputChecker->keysToCheck & key_curr_state();
    log(U16HEX, thing);
}

void updateInputCheckers() {
    for (int i = 0; i < numComps(COMP_INPUT_CHECKER); i++) {
        InputCheckerComponent* inputChecker = &gInputCheckerCompsDense[i];
        if (inputChecker->keysToCheck & key_curr_state())
            inputChecker->onCheckedKeyPressed(inputChecker->header.entId);
    }
}

void addComponentInputChecker(int entId, u16 keys, void(*callback)(int)) {
    InputCheckerComponent iC = {{entId, 0}, callback, keys};
    addComponentCustom(&iC, COMP_INPUT_CHECKER);
}

void removeComponentInputChecker(int entId) {
    removeComponent(entId, COMP_INPUT_CHECKER);
}
