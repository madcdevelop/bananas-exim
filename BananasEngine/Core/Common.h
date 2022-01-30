#ifndef CORE_ENGINE_COMMON_H
#define CORE_ENGINE_COMMON_H

#include <cstdint>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include "../Extern/glad/glad.h"
#include "../Extern/glm/glm.hpp"
#include "../Extern/glm/gtc/matrix_transform.hpp"
    
// Turns on Visual Studio specific memory leak check with DBG_NEW defines set
// https://docs.microsoft.com/en-us/visualstudio/debugger/finding-memory-leaks-using-the-crt-library?view=vs-2019
#define MEMORY_LEAK_CHECK 0
#if MEMORY_LEAK_CHECK
    // Includes and defines for debugging memory leaks
    #define _CRTDBG_MAP_ALLOC
    #include <stdlib.h>
    #include <crtdbg.h>

    #ifdef _DEBUG
        #define DBG_NEW new ( _NORMAL_BLOCK, __FILE__, __LINE__)
    #else
        #define DBG_NEW new
    #endif
#endif

// Virtual Key Codes
// https://docs.microsoft.com/en-us/windows/win32/inputdev/virtual-key-codes
#define BANANAS_KEY_W 0x57
#define BANANAS_KEY_S 0x53
#define BANANAS_KEY_D 0x44
#define BANANAS_KEY_A 0x41

// Common Types
typedef int8_t int8;
typedef int16_t int16;
typedef int32_t int32;
typedef int64_t int64;
typedef uint8_t uint8;
typedef uint16_t uint16;
typedef uint32_t uint32;
typedef uint64_t uint64;
typedef float real32;
typedef double real64;

// assert intrinsic to compiler
#define ASSERT(f) if (!(f)) __debugbreak(); // msvc specific
#define GLCALL(f) glClearError_(); \
                  f;\
                  ASSERT(glCheckError_(#f, __FILE__, __LINE__))

void glClearError_();
bool glCheckError_(const char *function, const char *file, int32 line);

std::string OpenFile(const char* filePath);

#endif
