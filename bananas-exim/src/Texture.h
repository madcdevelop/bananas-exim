#pragma once

namespace Core
{

class Texture {

public:
    unsigned int m_RenderId;

public:
    Texture();
    ~Texture();

    void Bind(unsigned int textureIndex) const;
    void UnBind() const;

    unsigned int LoadBMPCustom(const char* imagepath);

};

}