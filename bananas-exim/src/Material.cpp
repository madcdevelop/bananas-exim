#include "Material.h"


namespace Core 
{

Material::Material()
{
}

Material::Material(const std::string& name, glm::vec3& ambient, glm::vec3& diffuse, glm::vec3& specular, glm::vec3& emissive, float shininess, Shader& shader, std::vector<Texture>& textures)
    : m_Name(name), m_Ambient(ambient), m_Diffuse(diffuse), m_Specular(specular), m_Emissive(emissive), m_Shininess(shininess), m_Shader(shader), m_Textures(textures)
{
     
}

Material::~Material()
{

}

}