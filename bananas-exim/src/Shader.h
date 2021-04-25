#pragma once

#include "Common.h"

namespace Core
{

class Shader 
{
public:
    unsigned int m_ProgramId;

public:
    Shader(const char* vertexShaderFilePath, const char* fragmentShaderFilePath);
    ~Shader();

    std::string ReadFile(const char* filePath);

};


}