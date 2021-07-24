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
    if(models.size() == 0)
    {
        MessageBoxA(NULL, "The scene is empty. No models to export.", "Error", 0);
        return false;
    }

    std::string fileType = filePath.substr(filePath.find_last_of(".")+1);

    // Export based on file type
    if(fileType == "obj")
    {
        if(!ExportModelOBJ(filePath, models)) return false;

        std::string fileName = filePath.substr(filePath.find_last_of("\\")+1);
        fileName = fileName.substr(0, fileName.find_last_of("."));
        std::string mtlFilePath = filePath.substr(0, filePath.find_last_of("\\")) + "\\" + fileName + ".mtl";
        if(!ExportModelMTL(mtlFilePath, models)) return false;

        return true;
    }

    std::string errorMessage = "ERROR\t\tFile Type\t\tPossibly incorrect filetype: " 
                               + std::string(filePath) + ".Currently only exports .obj files\n";
    OutputDebugStringA(errorMessage.c_str());

    return false;
}

bool Exporter::ExportModelOBJ(const std::string& filePath, std::vector<Model>& models)
{
    std::fstream fileOut;

    fileOut.open(filePath, std::ios_base::out);
    if(!fileOut.is_open())
    {
        std::string errorMessage = "ERROR\t\tFileStream\t\tCould not read file path: " 
                                   + std::string(filePath) + ".\n";
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

    uint32 positionFaceIndex  = 0;
    uint32 textureUVFaceIndex = 0;
    uint32 normalFaceIndex    = 0;
    for(uint32 meshIndex = 0; meshIndex < models[0].m_Meshes.size(); meshIndex++)
    {
        // o name_of_object
        fileOut << "o" << " " << models[0].m_Meshes[meshIndex].m_Name << std::endl;

        // v reverse the index and only output unique vertex positions
        std::map<std::string, int32> distinctPositions;
        int32 positionCount = 1;
        for(uint32 positionIndex = 0; 
            positionIndex < models[0].m_Meshes[meshIndex].m_Vertices.size(); 
            positionIndex++)
        {
            Vertex* vertex;
            vertex = &models[0].m_Meshes[meshIndex].m_Vertices[positionIndex];

            // TODO(neil): write a compare function specific for struct. 
            // So no conversion to string has to happen.
            const std::string positionKey = std::to_string(vertex->position[0]) + " " + 
                                            std::to_string(vertex->position[1]) + " " + 
                                            std::to_string(vertex->position[2]);
            if(distinctPositions.find(positionKey) == distinctPositions.end())
            {
                distinctPositions.insert(std::pair<std::string, int32>(positionKey, positionCount));
                ++positionCount;
            }
            else
            {
                continue;
            }

            // output to file
            fileOut << "v";
            for(int32 i = 0; i < vertex->position.length(); i++)
            {
                fileOut << " " << std::to_string(vertex->position[i]);
            }
            fileOut << std::endl;
        }

        // vt reverse the index and only output unique texture coordinates
        std::map<std::string, int32> distinctTexturesUV;
        int32 textureUVCount = 1;
        for(uint32 textureUVIndex = 0; 
            textureUVIndex < models[0].m_Meshes[meshIndex].m_Vertices.size(); 
            textureUVIndex++)
        {
            Vertex* vertex;
            vertex = &models[0].m_Meshes[meshIndex].m_Vertices[textureUVIndex];

            const std::string textureCoordKey = std::to_string(vertex->textureUV[0]) + " " + 
                                                std::to_string(vertex->textureUV[1]);
            if(distinctTexturesUV.find(textureCoordKey) == distinctTexturesUV.end())
            {
                distinctTexturesUV.insert(std::pair<std::string, int32>(textureCoordKey, textureUVCount));
                ++textureUVCount;
            }
            else
            {
                continue;
            }

            // outout to file
            fileOut << "vt";
            for(int32 i = 0; i < vertex->textureUV.length(); i++)
            {
                fileOut << " " << std::to_string(vertex->textureUV[i]);
            }
            fileOut << std::endl;
        }

        // vn reverse the index and only output unique vertex normals
        std::map<std::string, int32> distinctNormals;
        int32 normalCount = 1;
        for(uint32 normalIndex = 0; 
            normalIndex < models[0].m_Meshes[meshIndex].m_Vertices.size(); 
            normalIndex++)
        {
            Vertex* vertex;
            vertex = &models[0].m_Meshes[meshIndex].m_Vertices[normalIndex];

            const std::string normalKey = std::to_string(vertex->normal[0]) + " " + 
                                          std::to_string(vertex->normal[1]) + " " + 
                                          std::to_string(vertex->normal[2]);
            if(distinctNormals.find(normalKey) == distinctNormals.end())
            {
                distinctNormals.insert(std::pair<std::string, int32>(normalKey, normalCount));
                ++normalCount;
            }
            else
            {
                continue;
            }

            // outout to file
            fileOut << "vn";
            for(int32 i = 0; i < vertex->normal.length(); i++)
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
        for(uint32 indicesIndex = 0; 
            indicesIndex < models[0].m_Meshes[meshIndex].m_Indices.size(); 
            indicesIndex++)     
        {
            uint32 vertexIndex = models[0].m_Meshes[meshIndex].m_Indices[indicesIndex];
            Vertex* vertex;
            vertex = &models[0].m_Meshes[meshIndex].m_Vertices[vertexIndex];

            // Position
            std::string positionKey = std::to_string(vertex->position[0]) + " " + 
                                      std::to_string(vertex->position[1]) + " " + 
                                      std::to_string(vertex->position[2]);   
            std::map<std::string, int32>::iterator positionIter;
            positionIter = distinctPositions.find(positionKey);

            // Texture UV
            std::string textureCoordKey = std::to_string(vertex->textureUV[0]) + " " + 
                                          std::to_string(vertex->textureUV[1]);
            std::map<std::string, int32>::iterator textureUVIter;
            textureUVIter = distinctTexturesUV.find(textureCoordKey);

            // Normal
            std::string normalKey = std::to_string(vertex->normal[0]) + " " + 
                                    std::to_string(vertex->normal[1]) + " " + 
                                    std::to_string(vertex->normal[2]);
            std::map<std::string, int32>::iterator normalKeyIter;
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
        positionFaceIndex  += static_cast<uint32>(distinctPositions.size());
        textureUVFaceIndex += static_cast<uint32>(distinctTexturesUV.size());
        normalFaceIndex    += static_cast<uint32>(distinctNormals.size());
    }
    fileOut.close();

    return true;
}

bool Exporter::ExportModelMTL(const std::string& filePath, std::vector<Model>& models)
{
    std::fstream fileOut;

    fileOut.open(filePath, std::ios_base::out);
    if(!fileOut.is_open())
    {
        std::string errorMessage = "ERROR\t\tFileStream\t\tCould not read file path: " 
                                   + std::string(filePath) + ".\n";
        OutputDebugStringA(errorMessage.c_str());
        return false;
    }

    fileOut << "# Bananas Import Export Tool MTL File" << std::endl;
    // TODO(neil): Change static Material Count to dynamic
    fileOut << "# Material Count: " << "1" << std::endl;
    fileOut << std::endl;
    
    for(uint32 meshIndex = 0; 
        meshIndex < models[0].m_Meshes.size(); 
        meshIndex++)
    {
        fileOut << "newmtl" << " " << models[0].m_Meshes[meshIndex].m_Material.m_Name << std::endl;

        fileOut << "Ns" << " " << std::to_string(models[0].m_Meshes[meshIndex].m_Material.m_Shininess) << std::endl;

        fileOut << "Ka" << " ";
        fileOut << std::to_string(models[0].m_Meshes[meshIndex].m_Material.m_Ambient[0]) << " ";
        fileOut << std::to_string(models[0].m_Meshes[meshIndex].m_Material.m_Ambient[1]) << " ";
        fileOut << std::to_string(models[0].m_Meshes[meshIndex].m_Material.m_Ambient[2]);
        fileOut << std::endl;

        fileOut << "Kd" << " "; 
        fileOut << std::to_string(models[0].m_Meshes[meshIndex].m_Material.m_Diffuse[0]) << " ";
        fileOut << std::to_string(models[0].m_Meshes[meshIndex].m_Material.m_Diffuse[1]) << " ";
        fileOut << std::to_string(models[0].m_Meshes[meshIndex].m_Material.m_Diffuse[2]);
        fileOut << std::endl;

        fileOut << "Ks" << " "; 
        fileOut << std::to_string(models[0].m_Meshes[meshIndex].m_Material.m_Specular[0]) << " ";
        fileOut << std::to_string(models[0].m_Meshes[meshIndex].m_Material.m_Specular[1]) << " ";
        fileOut << std::to_string(models[0].m_Meshes[meshIndex].m_Material.m_Specular[2]);
        fileOut << std::endl;

        fileOut << "Ke" << " "; 
        fileOut << std::to_string(models[0].m_Meshes[meshIndex].m_Material.m_Emissive[0]) << " ";
        fileOut << std::to_string(models[0].m_Meshes[meshIndex].m_Material.m_Emissive[1]) << " ";
        fileOut << std::to_string(models[0].m_Meshes[meshIndex].m_Material.m_Emissive[2]);
        fileOut << std::endl;

        // TODO(neil): add to material on import to be able to export the values
        fileOut << "Ni" << " " << "1.450000" << std::endl;
        fileOut << "d" << " " << "1.000000" << std::endl;
        fileOut << "illum" << " " << "2" << std::endl;

        for(uint32 textureIndex = 0; 
            textureIndex < models[0].m_Meshes[meshIndex].m_Material.m_Textures.size(); 
            textureIndex++)
        {
            std::string materialType = models[0].m_Meshes[meshIndex].m_Material.m_Textures[textureIndex].m_Type;
            if(materialType == "texture_diffuse")
                fileOut << "map_Kd" << " " 
                        << models[0].m_Meshes[meshIndex].m_Material.m_Textures[textureIndex].m_FilePath 
                        << std::endl;    
            else if(materialType == "texture_specular")
                fileOut << "map_Ks" << " " 
                        << models[0].m_Meshes[meshIndex].m_Material.m_Textures[textureIndex].m_FilePath 
                        << std::endl;
        }
        fileOut << std::endl;

    }
    fileOut.close();

    return true;
}


}