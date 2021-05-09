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
    void SetFloat(const std::string& name, float v0);

    void SetInt(const std::string& name, int v0);

    void SetVec3(const std::string& name, float v0, float v1, float v2);

    void SetVec3(const std::string& name, const glm::vec3& vec) const;

    void SetMatrix4(const std::string& name, const bool isTranspose, const glm::mat4& matrix) const;

private:
    std::string ReadFile(const char* filePath);

};


}