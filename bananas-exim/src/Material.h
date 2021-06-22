#pragma once

#include "Common.h"
#include "Shader.h"
#include "Texture.h"


namespace Core
{

// How a mesh looks and the way it interacts with lights
class Material 
{

public:
    std::string m_Name;

    glm::vec3 m_Ambient;
    glm::vec3 m_Diffuse;
    glm::vec3 m_Specular;
    glm::vec3 m_Emissive;

    float m_Shininess;

    std::vector<Texture> m_Textures;

public:
    Material();
    Material(const std::string& name, const glm::vec3& ambient, const glm::vec3& diffuse, const glm::vec3& specular, const glm::vec3& emissive, const float shininess, const std::vector<Texture>& textures);
    ~Material();
};

}