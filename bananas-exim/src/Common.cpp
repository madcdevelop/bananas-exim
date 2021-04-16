#include "Common.h"

#include <string>

void glClearError_()
{
    while(glGetError() != GL_NO_ERROR);
}

bool glCheckError_(const char *function, const char *file, int line)
{
    while(GLenum errorCode = glGetError())
    {
        std::string error;
        switch(errorCode)
        {
            case GL_INVALID_ENUM:                   error = "INVALID_ENUM"; break;
            case GL_INVALID_VALUE:                  error = "INVALID_VALUE"; break;
            case GL_INVALID_OPERATION:              error = "INVALID_OPERATION"; break;
            case GL_STACK_OVERFLOW:                 error = "STACK_OVERFLOW"; break;
            case GL_STACK_UNDERFLOW:                error = "STACK_UNDERFLOW"; break;
            case GL_OUT_OF_MEMORY:                  error = "OUT_OF_MEMORY"; break;
            case GL_INVALID_FRAMEBUFFER_OPERATION:  error = "INVALID_FRAMEBUFFER_OPERATION"; break;
        }
        std::string outputMessage = "ERROR\t\tOPENGL\t\t" + error + "\t\tFunction: " + std::string(function) + "\t\tFile: " + std::string(file) + "\t\tLine Number: " + std::to_string(line) + "\n";
        OutputDebugStringA(outputMessage.c_str());
        return false;
    }
    return true;
}