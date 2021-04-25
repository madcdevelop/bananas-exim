#pragma once

#include "Camera.h"
#include "IndexBuffer.h"
#include "Shader.h"
#include "Texture.h"
#include "VertexBuffer.h"
#include "Window.h"

namespace Core
{

class Renderer {

public:
    Camera m_Camera;

private:
    VertexBuffer* m_VertexBuffer;
    IndexBuffer* m_IndexBuffer;
    Texture* m_Texture;
    Window* m_Window;
    
    Shader m_Shader1;

public:
    Renderer(Window* window, VertexBuffer* vbo, IndexBuffer* ibo, Texture* texture);
    ~Renderer();

    void Init();
    void Draw();
    void CameraTransform(glm::mat4 projection);

    std::string ReadFile(const char* filePath);
};

}