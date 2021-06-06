#pragma once

#include "Common.h"

namespace Core
{

class Texture {

public:
    unsigned int m_RenderId;
    std::string m_Type;
    const std::string m_FilePath;

public:
    Texture(const std::string& type, const std::string& filePath);
    ~Texture();

    void Bind(unsigned int textureIndex) const;
    void UnBind() const;

    unsigned int LoadBMPCustom(const char* imagepath);

};

}