#include "Model.h"

namespace Core
{

Model::Model(const std::vector<Mesh> meshes)
    : m_Meshes(meshes)
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

bool Model::Import(const char* filePath)
{
    // Open the file
    std::string fileType;
    std::string fileContents = OpenFile(filePath);

    // Condition: What is the file type?
        // Call function specific to opening each file type
    if (fileType == ".obj") {
        std::string objContents = ImportOBJ(fileContents);
        return true;
    }
    return false;
}

std::string ImportOBJ(const std::string& content)
{
    // Read in data line by line
    
    // Condition: check start of line for type of data.
    
    // Add each vertex data to struct
    
    // Add each struct to array

    return "";
}

}