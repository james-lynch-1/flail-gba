#ifndef INITIALISE
#define INITIALISE

#include "type.h"
#include "global.h"
#include "graphics/graphics.h"
#include "state.h"
#include "components/component.h"
#include "encounter.h"
#include "mem.h"

void initialiseGame();

void initialisePlayer();

void initialiseEntities();

Entity* allocEntities();

void loadSprites();

#endif
