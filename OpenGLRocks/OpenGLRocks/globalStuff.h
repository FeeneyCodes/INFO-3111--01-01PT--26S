#pragma once

#include <stdlib.h>

#include "cBasicFlyCamera/cBasicFlyCamera.h"
#include "cLightManager.h"
#include "cBasicTextureManager/cBasicTextureManager.h"
#include "cMesh.h"

extern cBasicFlyCamera* g_pFlyCamera;

extern cLightManager* g_pLightManager;

extern cBasicTextureManager* g_pTextureManager;

// Only function signatures
float getRand(void);

// NEVER put code here unless it's a template
// (it's almost never a template)
// only variable declarations (with extern)
// int x = 0;
// extern int x = 0;
extern int x;

// Reutrn NULL if isn't found
cMesh* g_pFindMeshByFriendlyName(std::string nameToFind);

extern int g_SelectedLightIndex;	// = 0