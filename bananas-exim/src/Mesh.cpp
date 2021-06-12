#include "Mesh.h"

namespace Core
{

Mesh::Mesh(const std::string& name, const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices, std::vector<Texture>& tex)
    : m_Name(name), m_Vertices(vertices), m_Indices(indices), m_Textures(tex),
      m_VAO(0), m_VBO(0), m_IBO(0)
{
    SetupMesh();
}

Mesh::~Mesh()
{
}

void Mesh::Draw(Shader& shader)
{
    GLCALL(glBindBuffer(GL_ARRAY_BUFFER, m_VBO));
    GLCALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO));

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

    GLCALL(glBindVertexArray(m_VAO));
    GLCALL(glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(m_Indices.size()), GL_UNSIGNED_INT, 0));
    GLCALL(glBindVertexArray(0));
}

void Mesh::SetupMesh()
{
    // Vertex Array Object
    GLCALL(glGenVertexArrays(1, &m_VAO));
    GLCALL(glBindVertexArray(m_VAO));

    // Vertex Buffer Object
    GLCALL(glGenBuffers(1, &m_VBO));
    GLCALL(glBindBuffer(GL_ARRAY_BUFFER, m_VBO));
    GLCALL(glBufferData(GL_ARRAY_BUFFER, m_Vertices.size() * sizeof(Vertex), &m_Vertices[0], GL_STATIC_DRAW));

    // Index Buffer Object
    GLCALL(glGenBuffers(1, &m_IBO));
    GLCALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO));
    GLCALL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_Indices.size() * sizeof(unsigned int), &m_Indices[0], GL_STATIC_DRAW));

    // Vertex attributes
    GLCALL(glEnableVertexAttribArray(0));
    GLCALL(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Core::Vertex), (void*)0));
    // Normal attributes
    GLCALL(glEnableVertexAttribArray(1));
    GLCALL(glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Core::Vertex), (void*)offsetof(Core::Vertex, Core::Vertex::normal)));
    // Texture coordinates(UV) attributes
    GLCALL(glEnableVertexAttribArray(2));
    GLCALL(glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Core::Vertex), (void*)offsetof(Core::Vertex, Core::Vertex::textureUV)));

    GLCALL(glBindVertexArray(0));
}

}