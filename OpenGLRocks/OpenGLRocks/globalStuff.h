#pragma once

#include <stdlib.h>

#include "cBasicFlyCamera/cBasicFlyCamera.h"
#include "cLightManager.h"

extern cBasicFlyCamera* g_pFlyCamera;

extern cLightManager* g_pLightManager;

// Only function signatures
float getRand(void);

// NEVER put code here unless it's a template
// (it's almost never a template)
// only variable declarations (with extern)
// int x = 0;
// extern int x = 0;
extern int x;
