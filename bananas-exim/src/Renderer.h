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