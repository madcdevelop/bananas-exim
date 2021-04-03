#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include "Window.h"
#include "VertexBuffer.h"

namespace Core
{

class Renderer {

private:
    VertexBuffer* m_VertexBuffer;
    Window* m_Window;
    
    GLuint m_shaderProgram;

public:
    Renderer(VertexBuffer* vbo, Window* window);
    ~Renderer();

    void Init();
    void Draw();

    std::string ReadFile(const char* filePath);
};

}