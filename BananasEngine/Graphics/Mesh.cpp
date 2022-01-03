#include "Mesh.h"

namespace GraphicsEngine
{

Mesh::Mesh(const std::string& name, const std::vector<Vertex>& vertices, const std::vector<uint32>& indices, Material& material)
    : m_name(name), m_vertices(vertices), m_indices(indices), m_material(material),
    m_vertexArrayObjectId(0), m_vertexBufferId(0), m_indexBufferId(0)
{
}

Mesh::~Mesh()
{
}

void Mesh::Draw(Shader& shader)
{
    uint32 textureDiffuseN  = 1;
    uint32 textureSpecularN = 1;

    for(uint32 texIndex = 0; texIndex < m_material.m_textures.size(); texIndex++)
    {
        std::string type = m_material.m_textures[texIndex].m_type;
        std::string number;
        if(type == "texture_diffuse")
            number = std::to_string(textureDiffuseN++);
        else if(type == "texture_specular")
            number = std::to_string(textureSpecularN++);

        shader.SetInt(("material." + type + number), texIndex);

        m_material.m_textures[texIndex].Bind(texIndex);
    }

    shader.SetVec3("material.ambient", m_material.m_ambient);
    shader.SetVec3("material.diffuse", m_material.m_diffuse);
    shader.SetVec3("material.specular", m_material.m_specular);
    shader.SetFloat("material.shininess", m_material.m_shininess);

    GLCALL(glBindVertexArray(m_vertexArrayObjectId));
    GLCALL(glDrawElements(GL_TRIANGLES, static_cast<uint32>(m_indices.size()), GL_UNSIGNED_INT, 0));
    GLCALL(glBindVertexArray(0));
}

void Mesh::SetupMesh()
{
    // Vertex Array Object
    GLCALL(glGenVertexArrays(1, &m_vertexArrayObjectId));
    GLCALL(glBindVertexArray(m_vertexArrayObjectId));

    // Vertex Buffer
    GLCALL(glGenBuffers(1, &m_vertexBufferId));
    GLCALL(glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferId));
    GLCALL(glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * m_vertices.size(), &m_vertices[0], GL_STATIC_DRAW));

    // Index Buffer
    GLCALL(glGenBuffers(1, &m_indexBufferId));
    GLCALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBufferId));
    GLCALL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32) * m_indices.size(), &m_indices[0], GL_STATIC_DRAW));

    // Vertex attributes
    GLCALL(glEnableVertexAttribArray(0));
    GLCALL(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GraphicsEngine::Vertex), (void*)0));
    // Normal attributes
    GLCALL(glEnableVertexAttribArray(1));
    GLCALL(glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(GraphicsEngine::Vertex), (void*)offsetof(GraphicsEngine::Vertex, GraphicsEngine::Vertex::normal)));
    // Texture coordinates(UV) attributes
    GLCALL(glEnableVertexAttribArray(2));
    GLCALL(glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(GraphicsEngine::Vertex), (void*)offsetof(GraphicsEngine::Vertex, GraphicsEngine::Vertex::textureUV)));

    GLCALL(glBindVertexArray(0));
}

}