#include "Model.h"

namespace Core
{

Model::Model(const VertexBuffer& vbo, const IndexBuffer& ibo, const Texture& tex)
    : m_VertexBuffer(vbo),
      m_IndexBuffer(ibo),
      m_Texture(tex)
{
    // Vertex Array Object
    unsigned int vao = 0;
    GLCALL(glGenVertexArrays(1, &vao));
    GLCALL(glBindVertexArray(vao));

    unsigned int lightVAO;
    GLCALL(glGenVertexArrays(1, &lightVAO));
    GLCALL(glBindVertexArray(lightVAO));
    
    // Vertex attributes
    GLCALL(glEnableVertexAttribArray(0));
    GLCALL(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0));
    // Texture coordinates(UV) attributes
    GLCALL(glEnableVertexAttribArray(1));
    GLCALL(glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float))));
}

Model::~Model()
{
}

}