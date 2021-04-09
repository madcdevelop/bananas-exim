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

namespace Core
{

class Renderer {

private:
    VertexBuffer* m_VertexBuffer;
    IndexBuffer* m_IndexBuffer;
    Window* m_Window;
    
    GLuint m_shaderProgram;

public:
    Renderer(VertexBuffer* vbo, IndexBuffer* ibo, Window* window);
    ~Renderer();

    void Init();
    void Draw();
    void CameraTransform(glm::mat4 projection);

    std::string ReadFile(const char* filePath);
};

}