#include "Texture.h"

namespace GraphicsEngine
{

Texture::Texture(const std::string& type, const std::string& filePath)
    : m_RenderId(0), m_Type(type), m_FilePath(filePath)
{
    
}

Texture::~Texture()
{
    GLCALL(glDeleteTextures(1, &m_RenderId));
}

void Texture::Bind(unsigned int textureIndex) const
{
    GLCALL(glActiveTexture(GL_TEXTURE0 + textureIndex));
    GLCALL(glBindTexture(GL_TEXTURE_2D, m_RenderId));
}

void Texture::UnBind() const
{
    GLCALL(glBindTexture(GL_TEXTURE_2D, 0));
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
        fclose(file);
        return 0;
    }
    if(header[0] != 'B' || header[1] != 'M'){
        MessageBoxA(NULL, "Not a correct BMP file", "Error", 0);
        fclose(file);
        return 0;
    }

    // Read ints from the byte array
    unsigned int dataPos   = *(int*)&(header[0x0A]); // Position in the file where the actual data begins
    unsigned int imageSize = *(int*)&(header[0x22]); // = width*height*3
    unsigned int width     = *(int*)&(header[0x12]);
    unsigned int height    = *(int*)&(header[0x16]);

    // Some bmp files are misformatted, guess missing information
    if(imageSize == 0) imageSize=width*height*3; // 3: one byte for each color(red, green, blue) component
    if(dataPos == 0) dataPos=54;

    // Create a buffer
    // Actual RGB data
    unsigned char* data = new unsigned char [imageSize];

    // Read the actual data from the file into the buffer
    fread(data, 1, imageSize, file);

    // Everything is in memory now, the file can be closed
    fclose(file);

    unsigned int textureId = 0;
    GLCALL(glGenTextures(1, &textureId));
    GLCALL(glBindTexture(GL_TEXTURE_2D, textureId));
    GLCALL(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, data));

    // Open GL has now copied the data. Free our own version
    delete [] data;

    // Low Texture Quality
    // GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
    // GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));

    // Nice trilinear filtering
    GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
    GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));
    GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
    GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR));
    GLCALL(glGenerateMipmap(GL_TEXTURE_2D));

    return textureId;
}


}