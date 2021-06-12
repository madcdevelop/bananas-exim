#include "Model.h"

#include <map>

namespace Core
{

Model::Model()
{

}

Model::~Model()
{

}

void Model::Draw(Shader& shader)
{
    for(unsigned int meshIndex = 0; meshIndex < m_Meshes.size(); meshIndex++)
    {
        m_Meshes[meshIndex].Draw(shader);
    }
}

void Model::LoadTextures()
{
    for(unsigned int meshIndex = 0; meshIndex < m_Meshes.size(); meshIndex++)
    {
        for(unsigned int texIndex = 0; texIndex < m_Meshes[meshIndex].m_Textures.size(); texIndex++)
        {
            Texture* texture = &m_Meshes[meshIndex].m_Textures[texIndex];
            texture->m_RenderId = texture->LoadBMPCustom(texture->m_FilePath.c_str());
        }
    }
}

bool Model::Import(const std::string& filePath, std::vector<std::string>& outNames, std::vector<std::vector<Vertex>>& outVertices, std::vector<std::vector<unsigned int>>& outIndices, std::vector<unsigned int>& outMeshSizes)
{
    // Open the file
    std::string fileType = filePath.substr(filePath.find_last_of(".")+1);

    std::ifstream fileStream(filePath, std::ios::in);
    if(!fileStream.is_open())
    {
        std::string errorMessage = "ERROR\t\tFileStream\t\tCould not read file path: " + std::string(filePath) + ". File does not exist.\n";
        OutputDebugStringA(errorMessage.c_str());
        return false;
    }

    // Import based on file type
    if (fileType == "obj") {
        ImportOBJ(fileStream, outNames, outVertices, outIndices, outMeshSizes);
        return true;
    }

    return false;
}

void Model::ImportOBJ(std::ifstream& fileStream, std::vector<std::string>& outNames, std::vector<std::vector<Vertex>>& outVertices, std::vector<std::vector<unsigned int>>& outIndices, std::vector<unsigned int>& outMeshSizes)
{
    std::vector<glm::vec3> positions;
    std::vector<glm::vec2> textureCoordinates;
    std::vector<glm::vec3> normals;
    std::vector<Face> faces;
    unsigned int meshCount = 0;
    unsigned int meshSize = 0;

    // @TODO: currently only parses 1 object. Needs to be updated to parse more than 1 object.
    while(!fileStream.eof())
    {
        std::string line = "";
        std::string token = "";
        std::getline(fileStream, line);

        std::vector<std::string> tokens;
        std::stringstream ss(line);

        while(std::getline(ss, token, ' '))
        {
            tokens.push_back(token);
        }
        
        // Skip Empty line
        if(tokens.size() == 0) 
        {
            outMeshSizes.push_back(meshSize);
            break;   
        } 

        std::string start = tokens[0];
        // Texture coordinates
        if(start == "vt")
        {
            glm::vec2 textureCoordinate(std::stof(tokens[1]), std::stof(tokens[2]));
            textureCoordinates.push_back(textureCoordinate); 
        }
        // Indices f v1/vt1/vn1 (Specifically triangulate faces on export)
        else if(start == "f")
        {
            Face face;
            
            int faceIndex = 0;
            for (const auto& innerToken : tokens)
            {
                if(innerToken == start) continue;
                std::string faceToken;
                std::vector<std::string> faceTokens;
                std::stringstream faceSS(innerToken);
                while(std::getline(faceSS, faceToken, '/'))
                {
                    faceTokens.push_back(faceToken);
                }

                Index index = { 0 };
                index.positionIndex = std::stoul(faceTokens[0]);
                index.textureIndex  = std::stoul(faceTokens[1]);
                index.normalIndex   = std::stoul(faceTokens[2]);
                face.indices[faceIndex] = index;
                faceIndex++;
                meshSize++;
            }

            faces.push_back(face);
        }
        // Vertex Positions
        else if(start == "v")
        {
            glm::vec3 vertexPosition(std::stof(tokens[1]), std::stof(tokens[2]), std::stof(tokens[3]));
            positions.push_back(vertexPosition);
        }
        // Vertex normals
        else if(start == "vn")
        {
            glm::vec3 vertexNormal(std::stof(tokens[1]), std::stof(tokens[2]), std::stof(tokens[3]));
            normals.push_back(vertexNormal);
        }
        // Object name
        else if(start == "o") 
        {
            outNames.push_back(tokens[1]);
            // Skip first mesh
            if(meshCount > 0)
            {
                outMeshSizes.push_back(meshSize);
                // reset meshSize for new object
                meshSize = 0; 
            }
            meshCount++;
        }
    }

    // Fill vertices based on index coordinates
    std::map<std::string, int> distinctVertices;
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;

    int indicesIndex = 0;
    int meshProcessedSize = 0;
    int meshProcessedIndex = 0;
    for(auto &&f : faces)
    {
        for(auto &&i : f.indices)
        {
            // Face format: v1/vt1/vn1
            // Only add keys that are unique. Re-use vertex data that already exists
            std::string indexKey = std::to_string(i.positionIndex) + "/" + std::to_string(i.textureIndex) + "/" + std::to_string(i.normalIndex); 
            auto found = distinctVertices.find(indexKey);
            if(found != distinctVertices.end()) 
            {
                indices.push_back(found->second);
            }
            else
            {
                distinctVertices.insert(std::pair<std::string, int>(indexKey, indicesIndex));

                Vertex v;
                v.position   = positions[i.positionIndex-1];
                v.textureUV  = textureCoordinates[i.textureIndex-1];
                v.normal     = normals[i.normalIndex-1];

                vertices.push_back(v);
                indices.push_back(indicesIndex);
                indicesIndex++;
            }
            meshProcessedSize++;
        }
        if(meshProcessedSize == outMeshSizes[meshProcessedIndex])
        {
            outVertices.push_back(vertices);
            outIndices.push_back(indices);

            // Clear for processing next mesh
            indicesIndex = 0;
            meshProcessedSize = 0;
            meshProcessedIndex++;
            vertices.clear();
            indices.clear();
        }
    }
}

}