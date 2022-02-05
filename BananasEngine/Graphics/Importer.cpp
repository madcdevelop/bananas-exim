#include "Importer.h"

namespace GraphicsEngine
{

Importer::Importer()
{

}

Importer::~Importer()
{

}

bool Importer::LoadModel(const std::string& filePath, std::vector<std::string>& outNames, std::vector<std::vector<Vertex>>& outVertices, std::vector<std::vector<uint32>>& outIndices, std::vector<uint32>& outMeshSizes, std::vector<Material>& outMaterials)
{
    // Import based on file type
    std::string fileType = filePath.substr(filePath.find_last_of(".")+1);
    std::string path = filePath.substr(0, filePath.find_last_of("\\"));
    if (fileType == "obj") {
        LoadModelOBJ(filePath, outNames, outVertices, outIndices, outMeshSizes, path, outMaterials);
        return true;
    }

    return false;
}

void Importer::LoadModelOBJ(const std::string& filePath, std::vector<std::string>& outNames, std::vector<std::vector<Vertex>>& outVertices, std::vector<std::vector<uint32>>& outIndices, std::vector<uint32>& outMeshSizes, std::string& mtlPath, std::vector<Material>& outMaterials)
{
    // Open the file
    std::ifstream fileStream(filePath, std::ios::in);
    if(!fileStream.is_open())
    {
        std::string errorMessage = "ERROR\t\tFileStream\t\tCould not read file path: " + std::string(filePath) + ". File does not exist.\n";
        OutputDebugStringA(errorMessage.c_str());
        return;
    }

    std::vector<glm::vec3> positions;
    std::vector<glm::vec2> textureCoordinates;
    std::vector<glm::vec3> normals;
    std::vector<Face> faces;
    
    uint32 meshCount = 0;
    uint32 meshSize = 0;

    std::string mtllib;
    std::vector<std::string> usemtl;

    while(!fileStream.eof())
    {
        std::string line = "";
        std::string token = "";
        std::getline(fileStream, line);

        std::vector<std::string> tokens;
        std::stringstream ss(line);

        while(ss >> token)
        {
            tokens.push_back(token);
        }
        
        // Skip Empty line
        if(tokens.size() == 0) 
        {
            outMeshSizes.push_back(meshSize);
            continue;
        } 

        std::string start = tokens[0];
        // Skip commented line
        if (start == "#")
        {
            continue;
        }
        // Texture coordinates
        else if(start == "vt")
        {
            glm::vec2 textureCoordinate(std::stof(tokens[1]), std::stof(tokens[2]));
            textureCoordinates.push_back(textureCoordinate); 
        }
        // Indices f v1/vt1/vn1 
        // TODO(neil): Currently only able to triangulate faces on import
        else if(start == "f")
        {
            Face face;
            
            int32 faceIndex = 0;
            for (const auto& innerToken : tokens)
            {
                if(innerToken == start) continue;
                
                std::string faceToken;
                std::string faceTokens[3];
                std::stringstream faceSS(innerToken);
                for(int innerTokenIndex=0; std::getline(faceSS, faceToken, '/'); innerTokenIndex++)
                {
                    faceTokens[innerTokenIndex] = faceToken;
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
        // .MTL file
        else if(start == "mtllib")
        {
            mtllib = tokens[1];
        }
        else if(start == "usemtl")
        {
            usemtl.push_back(tokens[1]);
        }
    }

    // Fill vertices based on index coordinates
    std::map<Index, int32> distinctVertices;
    std::vector<Vertex> vertices;
    std::vector<uint32> indices;

    int32 indicesIndex = 0;
    int32 meshProcessedSize = 0;
    int32 meshProcessedIndex = 0;
    for(auto &&face : faces)
    {
        for(auto &&faceIndex : face.indices)
        {
            // Face format: v1/vt1/vn1
            // Only add keys that are unique. Re-use vertex data that already exists
            auto found = distinctVertices.find(faceIndex);
            if(found != distinctVertices.end()) 
            {
                indices.push_back(found->second);
            }
            else
            {
                distinctVertices.insert(std::pair<Index, int32>(faceIndex, indicesIndex));

                Vertex v;
                v.position   = positions[faceIndex.positionIndex-1];
                v.textureUV  = textureCoordinates[faceIndex.textureIndex-1];
                v.normal     = normals[faceIndex.normalIndex-1];

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
    fileStream.close();

    // Load MTL file
    std::string mtlFilePath = mtlPath + "\\" + mtllib;
    LoadModelMTL(mtlFilePath, usemtl, outMaterials);
}

bool Importer::LoadModelMTL(std::string& filePath, std::vector<std::string>& usemtl, std::vector<Material>& outMaterials)
{
    std::ifstream fileStream(filePath, std::ios::in);
    if(!fileStream.is_open())
    {
        std::string errorMessage = "ERROR\t\tFileStream\t\tCould not read file path: " + std::string(filePath) + ". File does not exist.\n";
        OutputDebugStringA(errorMessage.c_str());
        return false;
    }

    std::map<std::string, int32> names;
    std::vector<glm::vec3> ambient;
    std::vector<glm::vec3> diffuse;
    std::vector<glm::vec3> specular;
    std::vector<glm::vec3> emissive;
    std::vector<real32> shininess;
    std::vector<Texture> textures;
    std::vector<std::vector<Texture>> texturesPerMesh;

    uint32 materialCount = 0;

    while(!fileStream.eof())
    {
        std::string line = "";
        std::string token = "";
        std::getline(fileStream, line);

        std::vector<std::string> tokens;
        std::stringstream ss(line);

        while(ss >> token)
        {
            tokens.push_back(token);
        }
        
        // Skip Empty line
        if(tokens.size() == 0) 
        {
            continue;
        } 

        std::string start = tokens[0];
        // Skip commented line
        if (start == "#")
        {
            continue;
        }
        // Name of material
        else if(start == "newmtl")
        {
            names.insert(std::pair<std::string, int32>(tokens[1], materialCount));
            materialCount++;
        }
        // Specular Highlights
        else if(start == "Ns")
        {
            // TODO(neil): Rounds up number from 323.999994 to 324.000000
            shininess.push_back(std::stof(tokens[1]));
        }
        // Ambient Color
        else if(start == "Ka")
        {
            ambient.push_back(glm::vec3(std::stof(tokens[1]), std::stof(tokens[2]), std::stof(tokens[3])));
        }
        // Diffuse Color
        else if(start == "Kd")
        {
            diffuse.push_back(glm::vec3(std::stof(tokens[1]), std::stof(tokens[2]), std::stof(tokens[3])));
        }
        // Specular Color
        else if(start == "Ks")
        {
            specular.push_back(glm::vec3(std::stof(tokens[1]), std::stof(tokens[2]), std::stof(tokens[3])));
        }
        // Emissive Color
        else if(start == "Ke")
        {
            emissive.push_back(glm::vec3(std::stof(tokens[1]), std::stof(tokens[2]), std::stof(tokens[3])));
        }
        // Diffuse Texture File Path
        else if(start == "map_Kd")
        {
            std::string textureFileName = tokens[1].substr(tokens[1].find_last_of("\\")+1);
            textures.push_back(Texture("texture_diffuse", "../../../Content/Textures/" + textureFileName));
        }
        // Specular Texture File Path
        else if(start == "map_Ks")
        {
            std::string textureFileName = tokens[1].substr(tokens[1].find_last_of("\\")+1);
            textures.push_back(Texture("texture_specular", "../../../Content/Textures/" + textureFileName));
            texturesPerMesh.push_back(textures);
            // Last texture file to load. Clear for next material.
            textures.clear();
        }
    }

    std::vector<int32> materialIndices;

    // Go through usemtl array. Create an index array to lookup which material to add to outMaterials
    for(uint32 matIndex = 0; matIndex < usemtl.size(); matIndex++)
    {
        auto found = names.find(usemtl[matIndex]);
        materialIndices.push_back(found->second);
    }

    for(uint32 matIndex = 0; matIndex < materialIndices.size(); matIndex++)
    {
        uint32 materialIndex = materialIndices[matIndex];
        Material material(usemtl[matIndex], ambient[materialIndex], diffuse[materialIndex], specular[materialIndex], emissive[materialIndex], shininess[materialIndex], texturesPerMesh[materialIndex]);
        outMaterials.push_back(material);
    }

    fileStream.close();

    return true;
}

}
