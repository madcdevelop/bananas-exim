#include "Renderer.h"

namespace Core
{
Renderer::Renderer(Window* window, VertexBuffer* vbo, IndexBuffer* ibo, Texture* texture)
    : m_Window(window), m_VertexBuffer(vbo), m_IndexBuffer(ibo), m_Texture(texture) 
{
    Init();
}

Renderer::~Renderer()
{
    glDeleteBuffers(1, &m_shaderProgram);
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
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

    // Vertex attributes
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    // Texture coordinates(UV) attributes
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));

    // Load Textures
    m_Texture->m_RenderId = m_Texture->LoadBMPCustom("C:\\Code\\bananas-exim\\bananas-exim\\content\\textures\\uvtemplate.bmp");
    glUniform1i(glGetUniformLocation(m_shaderProgram, "texture1"), 0);
}

void Renderer::Draw()
{
    glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(m_shaderProgram);

    glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)m_Window->m_Width/(float)m_Window->m_Height, 0.1f, 100.0f);
    // glm::mat4 projection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, 0.0f, 100.0f);
    CameraTransform(projection);

    m_VertexBuffer->Bind();
    m_IndexBuffer->Bind();
    m_Texture->Bind(0);

    glDrawArrays(GL_TRIANGLES, 0, 36);
}

void Renderer::CameraTransform(glm::mat4 projection)
{
    // Camera Matrix
    glm::mat4 view = glm::lookAt(glm::vec3(4,3,-3), glm::vec3(0,0,0), glm::vec3(0,1,0));

    // Model Matrix : an identity matrix which will be at the origin
    glm::mat4 model = glm::mat4(1.0f);

    // ModelViewProjection : multiplication of 3 matrices
    glm::mat4 mvp = projection * view * model;

    unsigned int matrixId = glGetUniformLocation(m_shaderProgram, "mvp");
    glUniformMatrix4fv(matrixId, 1, GL_FALSE, &mvp[0][0]);
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
