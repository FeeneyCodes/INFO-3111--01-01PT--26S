#pragma once

#define GLAD_GL_IMPLEMENTATION
#include <glad/glad.h>
//#include "include/glad/glad.h"
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <stdlib.h>

#include "cBasicFlyCamera/cBasicFlyCamera.h"

extern cBasicFlyCamera* g_pFlyCamera;

// Only function signatures
float getRand(void);

// NEVER put code here unless it's a template
// (it's almost never a template)
// only variable declarations (with extern)
// int x = 0;
// extern int x = 0;
extern int x;
