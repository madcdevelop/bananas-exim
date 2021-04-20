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
    GLCALL(glDeleteBuffers(1, &m_shaderProgram));
    GLCALL(glDisableVertexAttribArray(0));
    GLCALL(glDisableVertexAttribArray(1));
}

void Renderer::Init()
{
    // Z Buffer Init
    GLCALL(glEnable(GL_DEPTH_TEST));
    GLCALL(glDepthFunc(GL_LESS));

    // Vertex Array Object
    GLuint vao = 0;
    GLCALL(glGenVertexArrays(1, &vao));
    GLCALL(glBindVertexArray(vao));

    // Shaders
    // @TODO: Change to relative paths ("content/file_name.glsl")
    std::string vertexShaderStr   = ReadFile("C:\\Code\\bananas-exim\\bananas-exim\\content\\test_vs.glsl");
    std::string fragmentShaderStr = ReadFile("C:\\Code\\bananas-exim\\bananas-exim\\content\\test_fs.glsl");

    const char* vertexShader   = vertexShaderStr.c_str();
    const char* fragmentShader = fragmentShaderStr.c_str();

    // Load Shaders
    GLCALL(GLuint vs = glCreateShader(GL_VERTEX_SHADER));
    GLCALL(glShaderSource(vs, 1, &vertexShader, NULL));
    GLCALL(glCompileShader(vs));
    GLCALL(GLuint fs = glCreateShader(GL_FRAGMENT_SHADER));
    GLCALL(glShaderSource(fs, 1, &fragmentShader, NULL));
    GLCALL(glCompileShader(fs));

    GLCALL(m_shaderProgram = glCreateProgram());
    GLCALL(glAttachShader(m_shaderProgram, vs));
    GLCALL(glAttachShader(m_shaderProgram, fs));
    GLCALL(glLinkProgram(m_shaderProgram));

    // Vertex attributes
    GLCALL(glEnableVertexAttribArray(0));
    GLCALL(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0));
    // Texture coordinates(UV) attributes
    GLCALL(glEnableVertexAttribArray(1));
    GLCALL(glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float))));

    // Load Textures
    m_Texture->m_RenderId = m_Texture->LoadBMPCustom("C:\\Code\\bananas-exim\\bananas-exim\\content\\textures\\uvtemplate.bmp");
}

void Renderer::Draw()
{
    GLCALL(glClearColor(0.3f, 0.3f, 0.3f, 1.0f));
    GLCALL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

    GLCALL(glUseProgram(m_shaderProgram));

    glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)m_Window->m_Width/(float)m_Window->m_Height, 0.1f, 100.0f);
    // glm::mat4 projection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, 0.0f, 100.0f);
    CameraTransform(projection);

    m_VertexBuffer->Bind();
    m_IndexBuffer->Bind();
    m_Texture->Bind(0);
    GLCALL(glUniform1i(glGetUniformLocation(m_shaderProgram, "texture1"), 0));

    GLCALL(glDrawArrays(GL_TRIANGLES, 0, 36));
}

void Renderer::CameraTransform(glm::mat4 projection)
{
    // Model Matrix : an identity matrix which will be at the origin
    glm::mat4 model = glm::mat4(1.0f);

    // Camera Matrix
    glm::mat4 view = glm::lookAt(m_CameraPos, m_CameraPos + m_CameraFront, m_CameraUp);

    // ModelViewProjection : multiplication of 3 matrices
    glm::mat4 mvp = projection * view * model;

    GLCALL(unsigned int matrixId = glGetUniformLocation(m_shaderProgram, "mvp"));
    GLCALL(glUniformMatrix4fv(matrixId, 1, GL_FALSE, &mvp[0][0]));
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
