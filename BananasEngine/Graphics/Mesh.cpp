#include "Mesh.h"

namespace GraphicsEngine
{

Mesh::Mesh(const std::string& name, const std::vector<Vertex>& vertices, const std::vector<uint32>& indices, Material& material)
    : m_Name(name), m_Vertices(vertices), m_Indices(indices), m_Material(material),
      m_VAO(0)
{
    SetupMesh();
}

Mesh::~Mesh()
{
}

void Mesh::Draw(Shader& shader)
{
    uint32 textureDiffuseN  = 1;
    uint32 textureSpecularN = 1;

    for(uint32 texIndex = 0; texIndex < m_Material.m_Textures.size(); texIndex++)
    {
        std::string type = m_Material.m_Textures[texIndex].m_Type;
        std::string number;
        if(type == "texture_diffuse")
            number = std::to_string(textureDiffuseN++);
        else if(type == "texture_specular")
            number = std::to_string(textureSpecularN++);

        shader.SetInt(("material." + type + number), texIndex);

        m_Material.m_Textures[texIndex].Bind(texIndex);
    }

    shader.SetVec3("material.ambient", m_Material.m_Ambient);
    shader.SetVec3("material.diffuse", m_Material.m_Diffuse);
    shader.SetVec3("material.specular", m_Material.m_Specular);
    shader.SetFloat("material.shininess", m_Material.m_Shininess);

    m_VertexBuffer->Bind();
    m_IndexBuffer->Bind();
    GLCALL(glBindVertexArray(m_VAO));
    GLCALL(glDrawElements(GL_TRIANGLES, static_cast<uint32>(m_IndexBuffer->m_Count), GL_UNSIGNED_INT, 0));
    GLCALL(glBindVertexArray(0));
}

void Mesh::SetupMesh()
{
    // Vertex Array Object
    GLCALL(glGenVertexArrays(1, &m_VAO));
    GLCALL(glBindVertexArray(m_VAO));

    m_VertexBuffer = new VertexBuffer{m_Vertices, static_cast<uint32>(m_Vertices.size())};
    m_IndexBuffer = new IndexBuffer{m_Indices, static_cast<uint32>(m_Indices.size())};

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