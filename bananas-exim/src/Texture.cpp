#include "Texture.h"
#include "Renderer.h"

namespace Core
{

    Texture::Texture(const char* imagepath)
        : m_Imagepath(imagepath), m_Data(NULL) 
    {
        LoadBMPCustom(m_Imagepath);

        glGenTextures(1, &m_RenderId);
        Bind();
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_Width, m_Height, 0, GL_BGR, GL_UNSIGNED_BYTE, m_Data);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    }

    Texture::~Texture()
    {
        glDeleteBuffers(1, &m_RenderId);
        delete[] m_Data;
    }

    void Texture::Bind() const
    {
        glBindTexture(GL_TEXTURE_2D, m_RenderId);
    }

    void Texture::UnBind() const
    {
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    unsigned int Texture::LoadBMPCustom(const char* imagepath)
    {
        // Each BMP file begins with a 54-bytes header
        unsigned char header[54];

        // Open the file
        FILE* file;
        fopen_s(&file, imagepath, "rb");
        
        if(!file){
            MessageBoxA(NULL, "Image could not be opened", "Error", 0);
            return 0;
        }
        if(fread(header, 1, 54, file) != 54){
            // If not 54 bytes read : problem
            MessageBoxA(NULL, "Not a correct BMP file", "Error", 0);
            return 0;
        }
        if(header[0] != 'B' || header[1] != 'M'){
            MessageBoxA(NULL, "Not a correct BMP file", "Error", 0);
            return 0;
        }

        // Read ints from the byte array
        unsigned int dataPos   = *(int*)&(header[0x0A]); // Position in the file where the actual data begins
        unsigned int imageSize = *(int*)&(header[0x22]); // = width*height*3
        m_Width                = *(int*)&(header[0x12]);
        m_Height               = *(int*)&(header[0x16]);

        // Some bmp files are misformatted, guess missing information
        if(imageSize == 0) imageSize=m_Width*m_Height*3; // 3: one byte for each color(red, green, blue) component
        if(dataPos == 0) dataPos=54;

        // Create a buffer
        // Actual RGB data
        m_Data = new unsigned char [imageSize];

        // Read the actual data from the file into the buffer
        fread(m_Data, 1, imageSize, file);

        // Everything is in memory now, the file can be closed
        fclose(file);

        return 1;
    }


}