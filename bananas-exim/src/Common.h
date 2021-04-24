#pragma once

#include "Timestep.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <glad\glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// assert intrinsic to compiler
#define ASSERT(f) if (!(f)) __debugbreak(); // msvc specific
#define GLCALL(f) glClearError_(); \
                  f;\
                  ASSERT(glCheckError_(#f, __FILE__, __LINE__))

void glClearError_();
bool glCheckError_(const char *function, const char *file, int line);
