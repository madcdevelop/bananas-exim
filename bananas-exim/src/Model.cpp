#include "Model.h"

namespace Core
{

Model::Model(const VertexBuffer& vbo, const IndexBuffer& ibo, std::vector<Texture>& tex)
    : m_VertexBuffer(vbo), m_IndexBuffer(ibo), m_Textures(tex),
      m_VAO(0)
{
    SetupMesh();
}

Model::~Model()
{
}

void Model::SetupMesh()
{
    // Vertex Array Object
    unsigned int m_VAO;
    GLCALL(glGenVertexArrays(1, &m_VAO));
    GLCALL(glBindVertexArray(m_VAO));
    
    // Vertex attributes
    GLCALL(glEnableVertexAttribArray(0));
    GLCALL(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Core::Vertex), (void*)0));
    // Normal attributes
    GLCALL(glEnableVertexAttribArray(1));
    GLCALL(glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Core::Vertex), (void*)offsetof(Core::Vertex, Core::Vertex::Normal)));
    // Texture coordinates(UV) attributes
    GLCALL(glEnableVertexAttribArray(2));
    GLCALL(glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Core::Vertex), (void*)offsetof(Core::Vertex, Core::Vertex::TextureUV)));
}

}