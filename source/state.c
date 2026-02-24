#include "state.h"

void setGameState(GameStateEnum state) {
    switch (state) {
        case NORMAL:
            gGameState.enterFunction = enterNormal;
            gGameState.updateFunction = updateNormal;
            gGameState.exitFunction = exitNormal;
            break;
        case PAUSE:
            gGameState.enterFunction = enterPause;
            gGameState.updateFunction = updatePause;
            gGameState.exitFunction = exitPause;
            break;
        case TITLE:
            gGameState.enterFunction = enterTitle;
            gGameState.updateFunction = updateTitle;
            gGameState.exitFunction = exitTitle;
            break;
    }
    gGameState.enterFunction();
}
