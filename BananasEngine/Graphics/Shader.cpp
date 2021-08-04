#include "Shader.h"

namespace GraphicsEngine
{

Shader::Shader()
{
}

Shader::Shader(const char* vertexShaderFilePath, const char* fragmentShaderFilePath)
{
    // Read Shader Files
    // TODO: Change to relative paths ("content/file_name.glsl")
    std::string vertexShaderStr   = OpenFile(vertexShaderFilePath);
    std::string fragmentShaderStr = OpenFile(fragmentShaderFilePath);

    const char* vertexShader   = vertexShaderStr.c_str();
    const char* fragmentShader = fragmentShaderStr.c_str();

    // Load Shaders
    GLCALL(GLuint vs = glCreateShader(GL_VERTEX_SHADER));
    GLCALL(glShaderSource(vs, 1, &vertexShader, NULL));
    GLCALL(glCompileShader(vs));
    GLCALL(GLuint fs = glCreateShader(GL_FRAGMENT_SHADER));
    GLCALL(glShaderSource(fs, 1, &fragmentShader, NULL));
    GLCALL(glCompileShader(fs));

    GLCALL(m_ProgramId = glCreateProgram());
    GLCALL(glAttachShader(m_ProgramId, vs));
    GLCALL(glAttachShader(m_ProgramId, fs));
    GLCALL(glLinkProgram(m_ProgramId));
}

Shader::~Shader()
{
    GLCALL(glDeleteBuffers(1, &m_ProgramId));
}

void Shader::UseProgram()
{
    GLCALL(glUseProgram(m_ProgramId));
}

void Shader::SetFloat(const std::string& name, real32 v0)
{
    GLCALL(glUniform1f(glGetUniformLocation(m_ProgramId, name.c_str()), v0));
}

void Shader::SetInt(const std::string& name, int32 v0)
{
    GLCALL(glUniform1i(glGetUniformLocation(m_ProgramId, name.c_str()), v0));
}

void Shader::SetVec3(const std::string& name, real32 v0, real32 v1, real32 v2)
{
    GLCALL(glUniform3f(glGetUniformLocation(m_ProgramId, name.c_str()), v0, v1, v2));
}

void Shader::SetVec3(const std::string& name, const glm::vec3& vec) const
{
    GLCALL(glUniform3fv(glGetUniformLocation(m_ProgramId, name.c_str()), 1, &vec[0]));
}

void Shader::SetMatrix3(const std::string& name, const bool isTranspose, const glm::mat3& matrix) const
{
    GLCALL(glUniformMatrix3fv(glGetUniformLocation(m_ProgramId, name.c_str()), 1, isTranspose, &matrix[0][0]));
}

void Shader::SetMatrix4(const std::string& name, const bool isTranspose, const glm::mat4& matrix) const
{
    GLCALL(glUniformMatrix4fv(glGetUniformLocation(m_ProgramId, name.c_str()), 1, isTranspose, &matrix[0][0]));
}

}   