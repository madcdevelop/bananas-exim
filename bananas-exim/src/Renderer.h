#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Window.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Texture.h"


static GLenum glCheckError_(const char *file, int line)
{
    GLenum errorCode;
    while((errorCode = glGetError()) != GL_NO_ERROR)
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
        std::string fileName(file);
        std::string lineNumber = std::to_string(line);
        std::string outputMessage = "ERROR\t\tOPENGL\t\t" + error + "\t\t" + fileName + "\t\t" + lineNumber + "\n";
        OutputDebugStringA(outputMessage.c_str());
    }
    return errorCode;
}
#define glCheckError() glCheckError_(__FILE__, __LINE__)


namespace Core
{

class Renderer {

private:
    VertexBuffer* m_VertexBuffer;
    IndexBuffer* m_IndexBuffer;
    Texture* m_Texture;
    Window* m_Window;
    
    GLuint m_shaderProgram;

public:
    Renderer(Window* window, VertexBuffer* vbo, IndexBuffer* ibo, Texture* texture);
    ~Renderer();

    void Init();
    void Draw();
    void CameraTransform(glm::mat4 projection);

    std::string ReadFile(const char* filePath);
};

}