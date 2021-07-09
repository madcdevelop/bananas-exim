#include "Exporter.h"

#include <map>

namespace GraphicsEngine
{

Exporter::Exporter()
{

}

Exporter::~Exporter()
{
    
}

bool Exporter::ExportModel(const std::string& filePath, std::vector<Model>& models)
{
    ASSERT(models.size() != 0)

    std::fstream fileOut;

    fileOut.open(filePath, std::ios_base::out);
    if(!fileOut.is_open())
    {
        std::string errorMessage = "ERROR\t\tFileStream\t\tCould not read file path: " + std::string(filePath) + ".\n";
        OutputDebugStringA(errorMessage.c_str());
        return false;
    }

    /////////////////////////////////////
    // Output to structure of .obj file
    /////////////////////////////////////

    // First line is comment about the program and version the obj file came from
    fileOut << "# Bananas Import Export Tool v1.00.0 OBJ File" << std::endl;
    fileOut << "# https://github.com/madcdevelop/bananas-exim" << std::endl;
    
    // mtllib name_of_obj_file.mtl
    std::string fileName = filePath.substr(filePath.find_last_of("\\")+1);
    fileName = fileName.substr(0, fileName.find_last_of("."));
    fileOut << "mtllib" << " " << fileName << ".mtl" << std::endl;

    unsigned int positionFaceIndex  = 0;
    unsigned int textureUVFaceIndex = 0;
    unsigned int normalFaceIndex    = 0;
    for(unsigned int meshIndex = 0; meshIndex < models[0].m_Meshes.size(); meshIndex++)
    {
        // o name_of_object
        fileOut << "o" << " " << models[0].m_Meshes[meshIndex].m_Name << std::endl;

        // v reverse the index and only output unique vertex positions
        std::map<std::string, int> distinctPositions;
        int positionCount = 1;
        for(unsigned int positionIndex = 0; positionIndex < models[0].m_Meshes[meshIndex].m_Vertices.size(); positionIndex++)
        {
            Vertex* vertex;
            vertex = &models[0].m_Meshes[meshIndex].m_Vertices[positionIndex];

            // check for duplicates (if duplicate then continue)
            // TODO(neil): write a compare function specific for struct. So no conversion to string has to happen.
            const std::string positionKey = std::to_string(vertex->position[0]) + " " + 
                                            std::to_string(vertex->position[1]) + " " + 
                                            std::to_string(vertex->position[2]);
            if(distinctPositions.find(positionKey) == distinctPositions.end())
            {
                distinctPositions.insert(std::pair<std::string, int>(positionKey, positionCount));
                ++positionCount;
            }
            else
            {
                continue;
            }

            // output to file
            fileOut << "v";
            for(int i = 0; i < vertex->position.length(); i++)
            {
                fileOut << " " << std::to_string(vertex->position[i]);
            }
            fileOut << std::endl;
        }

        // vt reverse the index and only output unique texture coordinates
        std::map<std::string, int> distinctTexturesUV;
        int textureUVCount = 1;
        for(unsigned int textureUVIndex = 0; textureUVIndex < models[0].m_Meshes[meshIndex].m_Vertices.size(); textureUVIndex++)
        {
            Vertex* vertex;
            vertex = &models[0].m_Meshes[meshIndex].m_Vertices[textureUVIndex];

            const std::string textureCoordKey = std::to_string(vertex->textureUV[0]) + " " + 
                                                std::to_string(vertex->textureUV[1]);
            if(distinctTexturesUV.find(textureCoordKey) == distinctTexturesUV.end())
            {
                distinctTexturesUV.insert(std::pair<std::string, int>(textureCoordKey, textureUVCount));
                ++textureUVCount;
            }
            else
            {
                continue;
            }

            // outout to file
            fileOut << "vt";
            for(int i = 0; i < vertex->textureUV.length(); i++)
            {
                fileOut << " " << std::to_string(vertex->textureUV[i]);
            }
            fileOut << std::endl;
        }

        // vn reverse the index and only output unique vertex normals
        std::map<std::string, int> distinctNormals;
        int normalCount = 1;
        for(unsigned int normalIndex = 0; normalIndex < models[0].m_Meshes[meshIndex].m_Vertices.size(); normalIndex++)
        {
            Vertex* vertex;
            vertex = &models[0].m_Meshes[meshIndex].m_Vertices[normalIndex];

            const std::string normalKey = std::to_string(vertex->normal[0]) + " " + 
                                          std::to_string(vertex->normal[1]) + " " + 
                                          std::to_string(vertex->normal[2]);
            if(distinctNormals.find(normalKey) == distinctNormals.end())
            {
                distinctNormals.insert(std::pair<std::string, int>(normalKey, normalCount));
                ++normalCount;
            }
            else
            {
                continue;
            }

            // outout to file
            fileOut << "vn";
            for(int i = 0; i < vertex->normal.length(); i++)
            {
                fileOut << " " << std::to_string(vertex->normal[i]);
            }
            fileOut << std::endl;
        }

        // usemtl name_of_material_for_mesh
        fileOut << "usemtl" << " " << models[0].m_Meshes[meshIndex].m_Material.m_Name << std::endl;

        // s off
        fileOut << "s" << " " << "off" << std::endl;

        // f output with / in between to get face indices
        fileOut << "f" << " ";
        for(unsigned int indicesIndex = 0; indicesIndex < models[0].m_Meshes[meshIndex].m_Indices.size(); indicesIndex++)     
        {
            unsigned int vertexIndex = models[0].m_Meshes[meshIndex].m_Indices[indicesIndex];
            Vertex* vertex;
            vertex = &models[0].m_Meshes[meshIndex].m_Vertices[vertexIndex];

            // Position
            std::string positionKey = std::to_string(vertex->position[0]) + " " + 
                                      std::to_string(vertex->position[1]) + " " + 
                                      std::to_string(vertex->position[2]);   
            std::map<std::string, int>::iterator positionIter;
            positionIter = distinctPositions.find(positionKey);

            // Texture UV
            std::string textureCoordKey = std::to_string(vertex->textureUV[0]) + " " + 
                                          std::to_string(vertex->textureUV[1]);
            std::map<std::string, int>::iterator textureUVIter;
            textureUVIter = distinctTexturesUV.find(textureCoordKey);

            // Normal
            std::string normalKey = std::to_string(vertex->normal[0]) + " " + 
                                    std::to_string(vertex->normal[1]) + " " + 
                                    std::to_string(vertex->normal[2]);
            std::map<std::string, int>::iterator normalKeyIter;
            normalKeyIter = distinctNormals.find(normalKey);

            fileOut << (positionIter->second + positionFaceIndex) << "/" << 
                       (textureUVIter->second + textureUVFaceIndex) << "/" << 
                       (normalKeyIter->second + normalFaceIndex) << " ";
            // Output new line every 3 vertices
            if((indicesIndex+1) % 3 == 0)
            {
                fileOut << std::endl;
                if(indicesIndex+1 == models[0].m_Meshes[meshIndex].m_Indices.size())
                {
                    break;
                }
                else
                {
                    fileOut << "f" << " ";
                }
            }
        }
        positionFaceIndex  += static_cast<unsigned int>(distinctPositions.size());
        textureUVFaceIndex += static_cast<unsigned int>(distinctTexturesUV.size());
        normalFaceIndex    += static_cast<unsigned int>(distinctNormals.size());
    }

    // Export MTL file
    std::string mtlFilePath = filePath.substr(0, filePath.find_last_of("\\")) + "\\" + fileName + ".mtl";
    ExportModelMTL(mtlFilePath, models);

    return true;
}

bool Exporter::ExportModelMTL(const std::string& filePath, std::vector<Model>& models)
{
    ASSERT(models.size() != 0)

    std::fstream fileOut;

    fileOut.open(filePath, std::ios_base::out);
    if(!fileOut.is_open())
    {
        std::string errorMessage = "ERROR\t\tFileStream\t\tCould not read file path: " + std::string(filePath) + ".\n";
        OutputDebugStringA(errorMessage.c_str());
        return false;
    }

    fileOut << "# Bananas Import Export Tool MTL File" << std::endl;
    fileOut << "# Material Count: " << "1" << std::endl;
    fileOut << std::endl;
    
    for(unsigned int meshIndex = 0; meshIndex < models[0].m_Meshes.size(); meshIndex++)
    {
        fileOut << "newmtl" << " " << models[0].m_Meshes[meshIndex].m_Material.m_Name << std::endl;

        fileOut << "Ns" << " " << std::to_string(models[0].m_Meshes[meshIndex].m_Material.m_Shininess) << std::endl;

        fileOut << "Ka" << " " << std::to_string(models[0].m_Meshes[meshIndex].m_Material.m_Ambient[0]) << " ";
        fileOut << std::to_string(models[0].m_Meshes[meshIndex].m_Material.m_Ambient[1]) << " ";
        fileOut << std::to_string(models[0].m_Meshes[meshIndex].m_Material.m_Ambient[2]);
        fileOut << std::endl;

        fileOut << "Kd" << " " << std::to_string(models[0].m_Meshes[meshIndex].m_Material.m_Diffuse[0]) << " ";
        fileOut << std::to_string(models[0].m_Meshes[meshIndex].m_Material.m_Diffuse[1]) << " ";
        fileOut << std::to_string(models[0].m_Meshes[meshIndex].m_Material.m_Diffuse[2]);
        fileOut << std::endl;

        fileOut << "Ks" << " " << std::to_string(models[0].m_Meshes[meshIndex].m_Material.m_Specular[0]) << " ";
        fileOut << std::to_string(models[0].m_Meshes[meshIndex].m_Material.m_Specular[1]) << " ";
        fileOut << std::to_string(models[0].m_Meshes[meshIndex].m_Material.m_Specular[2]);
        fileOut << std::endl;

        fileOut << "Ke" << " " << std::to_string(models[0].m_Meshes[meshIndex].m_Material.m_Emissive[0]) << " ";
        fileOut << std::to_string(models[0].m_Meshes[meshIndex].m_Material.m_Emissive[1]) << " ";
        fileOut << std::to_string(models[0].m_Meshes[meshIndex].m_Material.m_Emissive[2]);
        fileOut << std::endl;

        // TODO(neil): add to material on import to be able to export the values
        fileOut << "Ni" << " " << "1.450000" << std::endl;
        fileOut << "d" << " " << "1.000000" << std::endl;
        fileOut << "illum" << " " << "2" << std::endl;

        for(unsigned int textureIndex = 0; textureIndex < models[0].m_Meshes[meshIndex].m_Material.m_Textures.size(); textureIndex++)
        {
            if(models[0].m_Meshes[meshIndex].m_Material.m_Textures[textureIndex].m_Type == "texture_diffuse")
                fileOut << "map_Kd" << " " << models[0].m_Meshes[meshIndex].m_Material.m_Textures[textureIndex].m_FilePath << std::endl;    
            else if(models[0].m_Meshes[meshIndex].m_Material.m_Textures[textureIndex].m_Type == "texture_specular")
                fileOut << "map_Ks" << " " << models[0].m_Meshes[meshIndex].m_Material.m_Textures[textureIndex].m_FilePath << std::endl;
        }

        fileOut << std::endl;

    }

    return true;
}


}