#include "Model.h"

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

bool Model::Import(const std::string& filePath, std::string& outName, std::vector<Vertex>& outVertices, std::vector<unsigned int>& outIndices)
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
        ImportOBJ(fileStream, outName, outVertices, outIndices);
        return true;
    }
    
    return false;
}

void Model::ImportOBJ(std::ifstream& fileStream, std::string& outName, std::vector<Vertex>& outVertices, std::vector<unsigned int>& outIndices)
{
    std::vector<Face> faces;
    std::vector<glm::vec3> positions;
    std::vector<glm::vec2> textureCoordinates;
    std::vector<glm::vec3> normals;

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
        if(tokens.size() == 0) break; 

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
            outName = tokens[1];
    }

    // Fill vertices based on index coordinates
    for(auto &&f : faces)
    {
        for(auto &&i : f.indices)
        {
            Vertex v;
            v.position   = positions[i.positionIndex-1];
            v.textureUV  = textureCoordinates[i.textureIndex-1];
            v.normal     = normals[i.normalIndex-1];
            outVertices.push_back(v);
            outIndices.push_back(i.positionIndex-1);
        }
    }
}

}