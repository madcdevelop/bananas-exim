#ifndef CORE_ENGINE_COMMON_H
#define CORE_ENGINE_COMMON_H

#include "Timestep.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <glad\glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// Virtual Key Codes
// https://docs.microsoft.com/en-us/windows/win32/inputdev/virtual-key-codes
#define BANANAS_KEY_W 0x57
#define BANANAS_KEY_S 0x53
#define BANANAS_KEY_D 0x44
#define BANANAS_KEY_A 0x41

// assert intrinsic to compiler
#define ASSERT(f) if (!(f)) __debugbreak(); // msvc specific
#define GLCALL(f) glClearError_(); \
                  f;\
                  ASSERT(glCheckError_(#f, __FILE__, __LINE__))

void glClearError_();
bool glCheckError_(const char *function, const char *file, int line);

std::string OpenFile(const char* filePath);

#endif