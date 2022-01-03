#ifndef GRAPHICS_ENGINE_MATERIAL_H
#define GRAPHICS_ENGINE_MATERIAL_H

#include "../Core/Common.h"
#include "Shader.h"
#include "Texture.h"


namespace GraphicsEngine
{

// How a mesh looks and the way it interacts with lights
class Material 
{

public:
    std::string m_name;

    glm::vec3 m_ambient;
    glm::vec3 m_diffuse;
    glm::vec3 m_specular;
    glm::vec3 m_emissive;

    real32 m_shininess;

    std::vector<Texture> m_textures;

public:
    Material();
    Material(const std::string& name, const glm::vec3& ambient, const glm::vec3& diffuse, const glm::vec3& specular, const glm::vec3& emissive, const real32 shininess, const std::vector<Texture>& textures);
    ~Material();
};

}

#endif