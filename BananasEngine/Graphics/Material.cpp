#include "Material.h"


namespace GraphicsEngine 
{

Material::Material()
{
}

Material::Material(const std::string& name, const glm::vec3& ambient, const glm::vec3& diffuse, const glm::vec3& specular, const glm::vec3& emissive, const real32 shininess, const std::vector<Texture>& textures)
    : m_name(name), m_ambient(ambient), m_diffuse(diffuse), m_specular(specular), m_emissive(emissive), m_shininess(shininess), m_textures(textures)
{
     
}

Material::~Material()
{

}

}