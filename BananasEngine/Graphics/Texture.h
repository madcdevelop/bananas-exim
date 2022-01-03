#ifndef GRAPHICS_ENGINE_TEXTURE_H
#define GRAPHICS_ENGINE_TEXTURE_H

#include "../Core/Common.h"

namespace GraphicsEngine
{

class Texture {

public:
    uint32 m_renderId;
    std::string m_type;
    std::string m_filePath;

public:
    Texture();
    Texture(const std::string& type, const std::string& filePath);
    ~Texture();

    void Bind(uint32 textureIndex) const;
    void UnBind() const;

    uint32 LoadBMPCustom(const char* imagepath);

};

}

#endif