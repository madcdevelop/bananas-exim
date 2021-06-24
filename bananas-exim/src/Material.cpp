#include "Material.h"


namespace Core 
{

Material::Material()
{
}

Material::Material(const std::string& name, const glm::vec3& ambient, const glm::vec3& diffuse, const glm::vec3& specular, const glm::vec3& emissive, const float shininess, const std::vector<Texture>& textures)
    : m_Name(name), m_Ambient(ambient), m_Diffuse(diffuse), m_Specular(specular), m_Emissive(emissive), m_Shininess(shininess), m_Textures(textures)
{
     
}

Material::~Material()
{

}

}