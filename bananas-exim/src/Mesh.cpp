#include "Mesh.h"

namespace Core
{

Mesh::Mesh(const std::string& name, const VertexBuffer& vbo, const IndexBuffer& ibo, std::vector<Texture>& tex)
    : m_Name(name), m_VertexBuffer(vbo), m_IndexBuffer(ibo), m_Textures(tex),
      m_VAO(0)
{
    SetupMesh();
}

Mesh::~Mesh()
{
}

void Mesh::Draw(Shader& shader)
{
    unsigned int textureDiffuseN  = 1;
    unsigned int textureSpecularN = 1;

    for(unsigned int texIndex = 0; texIndex < m_Textures.size(); texIndex++)
    {
        std::string type = m_Textures[texIndex].m_Type;
        std::string number;
        if(type == "texture_diffuse")
            number = std::to_string(textureDiffuseN++);
        else if(type == "texture_specular")
            number = std::to_string(textureSpecularN++);

        shader.SetInt(("material." + type + number), texIndex);

        m_Textures[texIndex].Bind(texIndex);
    }

    // @TODO: Mesh indices data need to be fixed to draw with indices instead of vertices
    //GLCALL(glDrawElements(GL_TRIANGLES, static_cast<int>(m_IndexBuffer.m_Indices.size()), GL_UNSIGNED_INT, 0));
    GLCALL(glDrawArrays(GL_TRIANGLES, 0, static_cast<int>(m_VertexBuffer.m_Vertices.size())));
}

void Mesh::SetupMesh()
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
    GLCALL(glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Core::Vertex), (void*)offsetof(Core::Vertex, Core::Vertex::normal)));
    // Texture coordinates(UV) attributes
    GLCALL(glEnableVertexAttribArray(2));
    GLCALL(glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Core::Vertex), (void*)offsetof(Core::Vertex, Core::Vertex::textureUV)));
}

}