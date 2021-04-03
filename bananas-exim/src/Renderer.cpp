#include "Renderer.h"

namespace Core
{
Renderer::Renderer(VertexBuffer* vbo, Window* window)
    : m_VertexBuffer(vbo), m_Window(window)
{
    Init();
}

Renderer::~Renderer()
{
}

void Renderer::Init()
{
    // Vertex Array Object
    GLuint vao = 0;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    // Shaders
    // @TODO: Change to relative paths ("content/file_name.glsl")
    std::string vertexShaderStr   = ReadFile("C:\\Code\\bananas-exim\\bananas-exim\\content\\test_vs.glsl");
    std::string fragmentShaderStr = ReadFile("C:\\Code\\bananas-exim\\bananas-exim\\content\\test_fs.glsl");

    const char* vertexShader   = vertexShaderStr.c_str();
    const char* fragmentShader = fragmentShaderStr.c_str();

    // Load Shaders
    GLuint vs = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vs, 1, &vertexShader, NULL);
    glCompileShader(vs);
    GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fs, 1, &fragmentShader, NULL);
    glCompileShader(fs);

    m_shaderProgram = glCreateProgram();
    glAttachShader(m_shaderProgram, vs);
    glAttachShader(m_shaderProgram, fs);
    glLinkProgram(m_shaderProgram);
}

void Renderer::Draw()
{
    glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(m_shaderProgram);

    glEnableVertexAttribArray(0);
    m_VertexBuffer->Bind();
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

    glDrawArrays(GL_TRIANGLES, 0, 3);
    glDisableVertexAttribArray(0);
}

std::string Renderer::ReadFile(const char* filePath)
{
    std::ifstream fileStream(filePath, std::ios::in);
    if(!fileStream.is_open()) {
        std::cerr << "Could not read file" << filePath << ". File does not exist." << "\n";
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
