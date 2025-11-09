#ifndef STATE
#define STATE

#include "main.h"

void setGameState(GameStateEnum state);

// normal state functions

void enterNormal();

void updateNormal();

void exitNormal(GameStateEnum state);

// pause state functions

void enterPause();

void updatePause();

void exitPause(GameStateEnum state);

// title state functions

void enterTitle();

void updateTitle();

void exitTitle(GameStateEnum state);

#endif
