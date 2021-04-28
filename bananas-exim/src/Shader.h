#pragma once

#include "Common.h"

namespace Core
{

class Shader 
{
private:
    unsigned int m_ProgramId;

public:
    Shader(const char* vertexShaderFilePath, const char* fragmentShaderFilePath);
    ~Shader();

    void UseProgram();

    // Uniforms
    void SetInt1(const char* name, int v0);
    void SetMatrix4fv(const char* name, int count, bool transpose, const float* matrix);

private:
    std::string ReadFile(const char* filePath);

};


}