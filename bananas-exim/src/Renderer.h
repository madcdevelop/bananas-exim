#pragma once

#include "Window.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Texture.h"


namespace Core
{

class Renderer {

public:
    glm::vec3 m_CameraPos   = glm::vec3(0.0f, 0.0f, 3.0f);
    glm::vec3 m_CameraFront = glm::vec3(0.0f, 0.0f,-1.0f);
    glm::vec3 m_CameraUp    = glm::vec3(0.0f, 1.0f, 0.0f);

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