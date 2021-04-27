#include "Shader.h"

namespace Core
{

Shader::Shader(const char* vertexShaderFilePath, const char* fragmentShaderFilePath)
{
    // Read Shader Files
    // @TODO: Change to relative paths ("content/file_name.glsl")
    std::string vertexShaderStr   = ReadFile(vertexShaderFilePath);
    std::string fragmentShaderStr = ReadFile(fragmentShaderFilePath);

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

void Shader::SetInt1(const char* name, int v0)
{
    GLCALL(glUniform1i(glGetUniformLocation(m_ProgramId, name), v0));
}

void Shader::SetMatrix4fv(const char* name, int count, bool transpose, const float* matrix)
{
    GLCALL(unsigned int matrixId = glGetUniformLocation(m_ProgramId, name));
    GLCALL(glUniformMatrix4fv(matrixId, count, transpose, matrix));
}

std::string Shader::ReadFile(const char* filePath)
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


}   