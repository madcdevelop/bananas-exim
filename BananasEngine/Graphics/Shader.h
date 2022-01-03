#ifndef GRAPHICS_ENGINE_SHADER_H
#define GRAPHICS_ENGINE_SHADER_H

#include "../Core/Common.h"

namespace GraphicsEngine
{

class Shader 
{
private:
    uint32 m_programId;

public:
    Shader();
    Shader(const char* vertexShaderFilePath, const char* fragmentShaderFilePath);
    ~Shader();

    void UseProgram();

    // Uniforms
    void SetFloat(const std::string& name, real32 v0);

    void SetInt(const std::string& name, int32 v0);

    void SetVec3(const std::string& name, real32 v0, real32 v1, real32 v2);

    void SetVec3(const std::string& name, const glm::vec3& vec) const;

    void SetMatrix3(const std::string& name, const bool isTranspose, const glm::mat3& matrix) const;

    void SetMatrix4(const std::string& name, const bool isTranspose, const glm::mat4& matrix) const;

};


}

#endif