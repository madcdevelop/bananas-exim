#pragma once

namespace Core
{

class Texture {

    unsigned int m_RenderId;
    unsigned int m_Width;
    unsigned int m_Height;
    
    const char* m_Imagepath;
    unsigned char* m_Data;

public:
    Texture(const char* imagepath);
    ~Texture();

    void Bind() const;
    void UnBind() const;

    unsigned int LoadBMPCustom(const char* imagepath);

};

}