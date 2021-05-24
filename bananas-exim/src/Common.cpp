#include "Common.h"

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

std::string OpenFile(const char* filePath)
{
    std::ifstream fileStream(filePath, std::ios::in);
    if(!fileStream.is_open()) {
        std::string errorMessage = "ERROR\t\tFileStream\t\tCould not read file path: " + std::string(filePath) + ". File does not exist.\n";
        OutputDebugStringA(errorMessage.c_str());
        return "";
    }

    std::string line = "";
    std::string content = "";
    while(!fileStream.eof()) {
        std::getline(fileStream, line);
        content.append(line + "\n");
    }

    fileStream.close();
    return content;
}