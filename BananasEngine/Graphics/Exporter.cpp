#include "Exporter.h"

#include <set>

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

    // o name_of_object
    fileOut << "o" << " " << models[0].m_Meshes[0].m_Name << std::endl;

    // TODO(neil): order matters. Possibly use index to output vertices in order
    // v reverse the index and only output unique vertex positions
    std::set<std::string> distinctPositions;
    for(int meshIndex = 0; meshIndex < models[0].m_Meshes[0].m_Vertices.size(); meshIndex++)
    {
        Vertex* vertex;
        vertex = &models[0].m_Meshes[0].m_Vertices[meshIndex];

        // check for duplicates (if duplicate then continue)
        // TODO(neil): write a compare function specific for struct. So no conversion to string has to happen.
        const std::string positionKey = std::to_string(vertex->position[0]) + " " + std::to_string(vertex->position[1]) + " " + std::to_string(vertex->position[2]);
        if(distinctPositions.find(positionKey) == distinctPositions.end())
        {
            distinctPositions.insert(positionKey);
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

    // TODO(neil): order matters. Possibly use index to output vertices in order
    // vt reverse the index and only output unique texture coordinates
    std::set<std::string> distinctTexturesUV;
    for(int meshIndex = 0; meshIndex < models[0].m_Meshes[0].m_Vertices.size(); meshIndex++)
    {
        Vertex* vertex;
        vertex = &models[0].m_Meshes[0].m_Vertices[meshIndex];

        const std::string textureCoordKey = std::to_string(vertex->textureUV[0]) + " " + std::to_string(vertex->textureUV[1]);
        if(distinctTexturesUV.find(textureCoordKey) == distinctTexturesUV.end())
        {
            distinctTexturesUV.insert(textureCoordKey);
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
    std::set<std::string> distinctNormals;
    for(int meshIndex = 0; meshIndex < models[0].m_Meshes[0].m_Vertices.size(); meshIndex++)
    {
        Vertex* vertex;
        vertex = &models[0].m_Meshes[0].m_Vertices[meshIndex];

        const std::string normalsKey = std::to_string(vertex->normal[0]) + " " + std::to_string(vertex->normal[1]) + " " + std::to_string(vertex->normal[2]);
        if(distinctNormals.find(normalsKey) == distinctNormals.end())
        {
            distinctNormals.insert(normalsKey);
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
    std::string materialName = models[0].m_Meshes[0].m_Material.m_Name;
    fileOut << "usemtl" << " " << materialName << std::endl;

    // s off
    fileOut << "s" << " " << "off" << std::endl;

    // f output with / in between to get face indices

    return true;
}

}