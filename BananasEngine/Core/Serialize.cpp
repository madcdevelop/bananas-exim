#include "Serialize.h"

#include "Stack.h"
#include "../Graphics/Mesh.h"

namespace CoreEngine
{

bool SerializeToXML(const std::string& filePath, GraphicsEngine::Scene* scene)
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
    // Output to structure of .xml file
    /////////////////////////////////////

    int32 id = 0;
    int32 version = 1;

    fileOut << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>" << std::endl;
    
    /////////////////////////////////////
    // Scene - Root
    /////////////////////////////////////
    fileOut << "<" << "scene" << " " << "id=\"" << id << "\"" << " " << "version=\"" << version << "\"" << ">" << std::endl; 

    /////////////////////////////////////
    // Camera
    /////////////////////////////////////

    // Start of Camera and Item
    GraphicsEngine::Camera* camera = &scene->m_Camera;
    fileOut << "\t" << "<" << camera->m_Name << " " << "id=\"" << ++id << "\"" << " " << "version=\"" << version << "\"" << ">" << std::endl; // <camera>
    fileOut << "\t" << "\t" << "<" << "count" << ">" << 1 << "</" << "count" << ">" << std::endl; // count
    fileOut << "\t" << "\t" << "<" << "item" << " " << "id=\"" << ++id << "\"" << " " << "version" << "=\"" << version << "\"" << ">" << std::endl;
    
    // Camera.Position
    fileOut << "\t" << "\t" << "\t" << "<" << camera->m_Name << "." << "position" << ">" << std::endl;
    SerializeItemXML(fileOut, 4, camera->m_Name, camera->m_Position.x, "position.x");
    SerializeItemXML(fileOut, 4, camera->m_Name, camera->m_Position.y, "position.y");
    SerializeItemXML(fileOut, 4, camera->m_Name, camera->m_Position.z, "position.z");
    fileOut << "\t" << "\t" << "\t" << "</" << camera->m_Name << "." << "position" << ">" << std::endl;

    // Camera.Front
    fileOut << "\t" << "\t" << "\t" << "<" << camera->m_Name << "." << "front" << ">" << std::endl;
    SerializeItemXML(fileOut, 4, camera->m_Name, camera->m_Front.x, "front.x");
    SerializeItemXML(fileOut, 4, camera->m_Name, camera->m_Front.y, "front.y");
    SerializeItemXML(fileOut, 4, camera->m_Name, camera->m_Front.z, "front.z");
    fileOut << "\t" << "\t" << "\t" << "</" << camera->m_Name << "." << "front" << ">" << std::endl;

    // Camera.Up    
    fileOut << "\t" << "\t" << "\t" << "<" << camera->m_Name << "." << "up" << ">" << std::endl;
    SerializeItemXML(fileOut, 4, camera->m_Name, camera->m_Up.x, "up.x");
    SerializeItemXML(fileOut, 4, camera->m_Name, camera->m_Up.y, "up.y");
    SerializeItemXML(fileOut, 4, camera->m_Name, camera->m_Up.z, "up.z");
    fileOut << "\t" << "\t" << "\t" << "</" << camera->m_Name << "." << "up" << ">" << std::endl;

    // Camera.Right
    fileOut << "\t" << "\t" << "\t" << "<" << camera->m_Name << "." << "right" << ">" << std::endl;
    SerializeItemXML(fileOut, 4, camera->m_Name, camera->m_Right.x, "right.x");
    SerializeItemXML(fileOut, 4, camera->m_Name, camera->m_Right.y, "right.y");
    SerializeItemXML(fileOut, 4, camera->m_Name, camera->m_Right.z, "right.z");
    fileOut << "\t" << "\t" << "\t" << "</" << camera->m_Name << "." << "right" << ">" << std::endl;

    SerializeItemXML(fileOut, 3, camera->m_Name, camera->m_Yaw, "yaw");
    SerializeItemXML(fileOut, 3, camera->m_Name, camera->m_Pitch, "pitch");
    SerializeItemXML(fileOut, 3, camera->m_Name, camera->m_Fov, "fov");
    SerializeItemXML(fileOut, 3, camera->m_Name, camera->m_MovementSpeed, "movementSpeed");
    SerializeItemXML(fileOut, 3, camera->m_Name, camera->m_Sensitivity, "sensitivity");

    // End of Camera 
    fileOut << "\t" << "\t" << "</" << "item" << ">" << std::endl; 
    fileOut << "\t" << "</" << camera->m_Name << ">" << std::endl; 

    /////////////////////////////////////
    // Models
    /////////////////////////////////////

    for (uint32 modelIndex = 0; modelIndex < scene->m_Models.size(); modelIndex++)
    {
        fileOut << "\t" << "<" << "model" << " " << "id=\"" << ++id << "\"" << " " << "version=\"" << version << "\"" << ">" << std::endl;
        fileOut << "\t" << "\t" << "<" << "count" << ">" << scene->m_Models.size() << "</" << "count" << ">" << std::endl;
        fileOut << "\t" << "\t" << "<" << "item" << " " << "id=\"" << ++id << "\"" << " " << "version" << "=\"" << version << "\"" << ">" << std::endl;
        
        // Start of Mesh
        fileOut << "\t" << "\t" << "\t" << "<" << "mesh" << " " << "id=\"" << ++id << "\"" << " " << "version" << "=\"" << version << "\"" << ">" << std::endl;
        fileOut << "\t" << "\t" << "\t" << "\t" << "<" << "count" << ">" << scene->m_Models[modelIndex].m_Meshes.size() << "</" << "count" << ">" << std::endl;

        for (uint32 meshIndex = 0; meshIndex < scene->m_Models[modelIndex].m_Meshes.size(); meshIndex++)
        {
            fileOut << "\t" << "\t" << "\t" << "\t" << "<" << "item" << " " << "id=\"" << ++id << "\"" << " " << "version" << "=\"" << version << "\"" << ">" << std::endl;

            SerializeItemXML(fileOut, 5, "mesh", scene->m_Models[modelIndex].m_Meshes[meshIndex].m_Name, "name");

            // Vertices
            fileOut << "\t" << "\t" << "\t" << "\t" << "\t" << "<" << "mesh" << "." << "vertices" << ">" << std::endl;
            for (const auto vertex : scene->m_Models[modelIndex].m_Meshes[meshIndex].m_Vertices)
            {
                SerializeItemXML(fileOut, 6, "mesh", vertex.position.x, "vertices.position.x");
                SerializeItemXML(fileOut, 6, "mesh", vertex.position.y, "vertices.position.y");
                SerializeItemXML(fileOut, 6, "mesh", vertex.position.z, "vertices.position.z");
                SerializeItemXML(fileOut, 6, "mesh", vertex.normal.x, "vertices.normal.x");
                SerializeItemXML(fileOut, 6, "mesh", vertex.normal.y, "vertices.normal.y");
                SerializeItemXML(fileOut, 6, "mesh", vertex.normal.z, "vertices.normal.z");
                SerializeItemXML(fileOut, 6, "mesh", vertex.textureUV.x, "vertices.textureUV.x");
                SerializeItemXML(fileOut, 6, "mesh", vertex.textureUV.y, "vertices.textureUV.y");
            }
            fileOut << "\t" << "\t" << "\t" << "\t" << "\t" << "</" << "mesh" << "." << "vertices" << ">" << std::endl;

            // Indices
            fileOut << "\t" << "\t" << "\t" << "\t" << "\t" << "<" << "mesh" << "." << "indices" << ">" << std::endl;
            for (int32 indicesIndex = 0; 
                 indicesIndex < scene->m_Models[modelIndex].m_Meshes[meshIndex].m_Indices.size();
                 indicesIndex++)
            {
                SerializeItemXML(fileOut, 6, "mesh", scene->m_Models[modelIndex].m_Meshes[meshIndex].m_Indices[indicesIndex], std::string("indices." + std::to_string(indicesIndex)));
            }
            fileOut << "\t" << "\t" << "\t" << "\t" << "\t" << "</" << "mesh" << "." << "indices" << ">" << std::endl;

            // Material
            fileOut << "\t" << "\t" << "\t" << "\t" << "\t" << "<" << "mesh" << "." << "material" << ">" << std::endl;

            SerializeItemXML(fileOut, 6, "mesh", scene->m_Models[modelIndex].m_Meshes[meshIndex].m_Material.m_Name, "material.name");

            // Material - Ambient
            fileOut << "\t" << "\t" << "\t" << "\t" << "\t" << "\t" << "<" << "mesh" << "." << "material.ambient" << ">" << std::endl;
            SerializeItemXML(fileOut, 7, "mesh", scene->m_Models[modelIndex].m_Meshes[meshIndex].m_Material.m_Ambient.x, "material.ambient.x");
            SerializeItemXML(fileOut, 7, "mesh", scene->m_Models[modelIndex].m_Meshes[meshIndex].m_Material.m_Ambient.y, "material.ambient.y");
            SerializeItemXML(fileOut, 7, "mesh", scene->m_Models[modelIndex].m_Meshes[meshIndex].m_Material.m_Ambient.z, "material.ambient.z");
            fileOut << "\t" << "\t" << "\t" << "\t" << "\t" << "\t" << "</" << "mesh" << "." << "material.ambient" << ">" << std::endl;

            // Material - Diffuse
            fileOut << "\t" << "\t" << "\t" << "\t" << "\t" << "\t" << "<" << "mesh" << "." << "material.diffuse" << ">" << std::endl;
            SerializeItemXML(fileOut, 7, "mesh", scene->m_Models[modelIndex].m_Meshes[meshIndex].m_Material.m_Diffuse.x, "material.diffuse.x");
            SerializeItemXML(fileOut, 7, "mesh", scene->m_Models[modelIndex].m_Meshes[meshIndex].m_Material.m_Diffuse.y, "material.diffuse.y");
            SerializeItemXML(fileOut, 7, "mesh", scene->m_Models[modelIndex].m_Meshes[meshIndex].m_Material.m_Diffuse.z, "material.diffuse.z");
            fileOut << "\t" << "\t" << "\t" << "\t" << "\t" << "\t" << "</" << "mesh" << "." << "material.diffuse" << ">" << std::endl;

            // Material - Specular
            fileOut << "\t" << "\t" << "\t" << "\t" << "\t" << "\t" << "<" << "mesh" << "." << "material.specular" << ">" << std::endl;
            SerializeItemXML(fileOut, 7, "mesh", scene->m_Models[modelIndex].m_Meshes[meshIndex].m_Material.m_Specular.x, "material.specular.x");
            SerializeItemXML(fileOut, 7, "mesh", scene->m_Models[modelIndex].m_Meshes[meshIndex].m_Material.m_Specular.y, "material.specular.y");
            SerializeItemXML(fileOut, 7, "mesh", scene->m_Models[modelIndex].m_Meshes[meshIndex].m_Material.m_Specular.z, "material.specular.z");
            fileOut << "\t" << "\t" << "\t" << "\t" << "\t" << "\t" << "</" << "mesh" << "." << "material.specular" << ">" << std::endl;

            // Material - Emissive
            fileOut << "\t" << "\t" << "\t" << "\t" << "\t" << "\t" << "<" << "mesh" << "." << "material.emissive" << ">" << std::endl;
            SerializeItemXML(fileOut, 7, "mesh", scene->m_Models[modelIndex].m_Meshes[meshIndex].m_Material.m_Emissive.x, "material.emissive.x");
            SerializeItemXML(fileOut, 7, "mesh", scene->m_Models[modelIndex].m_Meshes[meshIndex].m_Material.m_Emissive.y, "material.emissive.y");
            SerializeItemXML(fileOut, 7, "mesh", scene->m_Models[modelIndex].m_Meshes[meshIndex].m_Material.m_Emissive.z, "material.emissive.z");
            fileOut << "\t" << "\t" << "\t" << "\t" << "\t" << "\t" << "</" << "mesh" << "." << "material.emissive" << ">" << std::endl;

            SerializeItemXML(fileOut, 6, "mesh", scene->m_Models[modelIndex].m_Meshes[meshIndex].m_Material.m_Shininess, "material.shininess");

            // Textures
            for (uint32 textureIndex = 0; 
                 textureIndex < scene->m_Models[modelIndex].m_Meshes[meshIndex].m_Material.m_Textures.size(); 
                 textureIndex++)
            {
                fileOut << "\t" << "\t" << "\t" << "\t" << "\t" << "\t" << "<" << "mesh" << "." << "material.texture" << ">" << std::endl;
                SerializeItemXML(fileOut, 7, "mesh", scene->m_Models[modelIndex].m_Meshes[meshIndex].m_Material.m_Textures[textureIndex].m_Type, "material.texture.type");
                SerializeItemXML(fileOut, 7, "mesh", scene->m_Models[modelIndex].m_Meshes[meshIndex].m_Material.m_Textures[textureIndex].m_FilePath, "material.texture.filepath");
                fileOut << "\t" << "\t" << "\t" << "\t" << "\t" << "\t" << "</" << "mesh" << "." << "material.texture" << ">" << std::endl;
            }
            // End of Material
            fileOut << "\t" << "\t" << "\t" << "\t" << "\t" << "</" << "mesh" << "." << "material" << ">" << std::endl;

            // End of Mesh
            fileOut << "\t" << "\t" << "\t" << "\t" << "</" << "item" << ">" << std::endl;
        }
        // End of Model
        fileOut << "\t" << "\t" << "\t" << "</" << "mesh" << ">" << std::endl;
        fileOut << "\t" << "\t" << "</" << "item" << ">" << std::endl;
        fileOut << "\t" << "</" << "model" << ">" << std::endl;
    }
    // End of Scene - Root
    fileOut << "</" << "scene" << ">" << std::endl; 

    return true;
}

void SerializeItemXML(std::fstream& output, int32 indents, const std::string& name, const std::string& value, const std::string& attributes)
{
    for (int i = 0; i < indents; ++i)
        output << "\t";
    output << "<" << name;
    if (attributes != "")
        SerializeAttributesXML(output, attributes);
    output << ">";
    output << value;
    output << "</" << name;
    if (attributes != "")
        SerializeAttributesXML(output, attributes);
    output << ">" << std::endl;
}

void SerializeItemXML(std::fstream& output, int32 indents, const std::string& name, const uint32 value, const std::string& attributes)
{
    for (int i = 0; i < indents; ++i)
        output << "\t";
    output << "<" << name;
    if (attributes != "")
        SerializeAttributesXML(output, attributes);
    output << ">";
    output << value;
    output << "</" << name;
    if (attributes != "")
        SerializeAttributesXML(output, attributes);
    output << ">" << std::endl;
}

void SerializeItemXML(std::fstream& output, int32 indents, const std::string& name, const real32 value, const std::string& attributes)
{
    for(int i = 0; i < indents; ++i)
        output << "\t";
    output << "<" << name;
    if(attributes != "")
        SerializeAttributesXML(output, attributes);
    output << ">";
    output << value;
    output << "</" << name;
    if(attributes != "")
        SerializeAttributesXML(output, attributes);
    output << ">" << std::endl;
}

void SerializeAttributesXML(std::fstream& output, const std::string& attributes)
{
    std::string token;
    std::stringstream ss(attributes);
    while(std::getline(ss, token, '.'))
        output << "." << token;
}

bool DeSerializeFromXML(const std::string& filePath, GraphicsEngine::Scene* scene)
{
    std::ifstream fileStream(filePath, std::ios::in);
    if(!fileStream.is_open())
    {
        std::string errorMessage = "ERROR\t\tFileStream\t\tCould not read file path: " 
                                   + std::string(filePath) + ".\n";
        OutputDebugStringA(errorMessage.c_str());
        return false;
    }

    Stack* xmlStack = new Stack();

    if (scene->m_Models.size() > 0)
        scene->m_Models.clear();

    std::vector<std::string> outNames;
    std::vector<std::vector<GraphicsEngine::Vertex>> outVertices;
    std::vector<std::vector<uint32>> outIndices;
    std::vector<GraphicsEngine::Material> outMaterials;

    std::vector<GraphicsEngine::Vertex> tempVertices;
    std::vector<uint32> tempIndices;
    int32 indicesIndex = 0;
    int32 meshCount = 0;
    
    GraphicsEngine::Vertex tempVertex;
    GraphicsEngine::Material tempMaterial;
    GraphicsEngine::Texture tempTexture;

    int32 cameraCount = 0;

    while(!fileStream.eof())
    {
        std::string line = "";
        std::string token = "";
        std::string value = "";
        std::getline(fileStream, line);

        if (line.compare("<?xml version=\"1.0\" encoding=\"UTF-8\"?>") == 0)
            continue;

        char prevChar;
        for (auto c : line)
        {
            if(c == '\t') continue;
            // XML Node Start
            else if (c == '<')
            {
                prevChar = c;
                continue;
            }
            // XML Node End
            else if (prevChar == '<' && c == '/')
            {
                // Camera.Position
                if (token.compare("camera.position.x") == 0)
                    scene->m_Camera.m_Position[0] = std::stof(value);
                else if (token.compare("camera.position.y") == 0)
                    scene->m_Camera.m_Position[1] = std::stof(value);
                else if (token.compare("camera.position.z") == 0)
                    scene->m_Camera.m_Position[2] = std::stof(value);
                // Camera.Front
                else if (token.compare("camera.front.x") == 0)
                    scene->m_Camera.m_Front[0] = std::stof(value);
                else if (token.compare("camera.front.y") == 0)
                    scene->m_Camera.m_Front[1] = std::stof(value);
                else if (token.compare("camera.front.z") == 0)
                    scene->m_Camera.m_Front[2] = std::stof(value);
                // Camera.Up                
                else if (token.compare("camera.up.x") == 0)
                    scene->m_Camera.m_Up[0] = std::stof(value);
                else if (token.compare("camera.up.y") == 0)
                    scene->m_Camera.m_Up[1] = std::stof(value);
                else if (token.compare("camera.up.z") == 0)
                    scene->m_Camera.m_Up[2] = std::stof(value);
                // Camera.Right
                else if (token.compare("camera.right.x") == 0)
                    scene->m_Camera.m_Right[0] = std::stof(value);
                else if (token.compare("camera.right.y") == 0)
                    scene->m_Camera.m_Right[1] = std::stof(value);
                else if (token.compare("camera.right.z") == 0)
                    scene->m_Camera.m_Right[2] = std::stof(value);
                // Camera Attributes
                else if (token.compare("camera.yaw") == 0)
                    scene->m_Camera.m_Yaw = std::stof(value);
                else if (token.compare("camera.pitch") == 0) 
                    scene->m_Camera.m_Pitch = std::stof(value);
                else if (token.compare("camera.fov") == 0) 
                    scene->m_Camera.m_Fov = std::stof(value);
                else if (token.compare("camera.movementSpeed") == 0) 
                    scene->m_Camera.m_MovementSpeed = std::stof(value);
                else if (token.compare("camera.sensitivity") == 0) 
                    scene->m_Camera.m_Sensitivity = std::stof(value);
                
                // Mesh Data
                if (token.compare("mesh.name") == 0)
                    outNames.push_back(value);
                
                // Mesh Vertices
                // Mesh Position
                else if (token.compare("mesh.vertices.position.x") == 0)
                    tempVertex.position.x = std::stof(value);
                else if (token.compare("mesh.vertices.position.y") == 0)
                    tempVertex.position.y = std::stof(value);
                else if (token.compare("mesh.vertices.position.z") == 0)
                    tempVertex.position.z = std::stof(value);
                // Mesh Normal
                else if (token.compare("mesh.vertices.normal.x") == 0)
                    tempVertex.normal.x = std::stof(value);
                else if (token.compare("mesh.vertices.normal.y") == 0)
                    tempVertex.normal.y = std::stof(value);
                else if (token.compare("mesh.vertices.normal.z") == 0)
                    tempVertex.normal.z = std::stof(value);
                // Mesh Texture UV
                else if (token.compare("mesh.vertices.textureUV.x") == 0)
                    tempVertex.textureUV.x = std::stof(value);
                else if (token.compare("mesh.vertices.textureUV.y") == 0)
                {
                    tempVertex.textureUV.y = std::stof(value);
                    tempVertices.push_back(tempVertex);
                }
                else if (xmlStack->Peek().compare("mesh.vertices") == 0)
                {
                    outVertices.push_back(tempVertices);
                    tempVertices.clear();
                }
                
                // Mesh Indices
                else if (token.compare(std::string("mesh.indices." + std::to_string(indicesIndex))) == 0)
                {
                    tempIndices.push_back(std::stoi(value));
                    indicesIndex++;
                }
                else if (xmlStack->Peek().compare("mesh.indices") == 0)
                {
                    outIndices.push_back(tempIndices);
                    indicesIndex = 0;
                    tempIndices.clear();
                }
                
                // Mesh Material
                else if (token.compare("mesh.material.name") == 0)
                    tempMaterial.m_Name = value;
                // Mesh Material Ambient
                else if (token.compare("mesh.material.ambient.x") == 0)
                    tempMaterial.m_Ambient.x = std::stof(value);
                else if (token.compare("mesh.material.ambient.y") == 0)
                    tempMaterial.m_Ambient.y = std::stof(value);
                else if (token.compare("mesh.material.ambient.z") == 0)
                    tempMaterial.m_Ambient.z = std::stof(value);
                // Mesh Material Diffuse
                else if (token.compare("mesh.material.diffuse.x") == 0)
                    tempMaterial.m_Diffuse.x = std::stof(value);
                else if (token.compare("mesh.material.diffuse.y") == 0)
                    tempMaterial.m_Diffuse.y = std::stof(value);
                else if (token.compare("mesh.material.diffuse.z") == 0)
                    tempMaterial.m_Diffuse.z = std::stof(value);
                // Mesh Material Specular
                else if (token.compare("mesh.material.specular.x") == 0)
                    tempMaterial.m_Specular.x = std::stof(value);
                else if (token.compare("mesh.material.specular.y") == 0)
                    tempMaterial.m_Specular.y = std::stof(value);
                else if (token.compare("mesh.material.specular.z") == 0)
                    tempMaterial.m_Specular.z = std::stof(value);
                // Mesh Material
                else if (token.compare("mesh.material.shininess") == 0)
                    tempMaterial.m_Shininess = std::stof(value);
                // Mesh Material Texture
                else if (token.compare("mesh.material.texture.type") == 0)
                    tempTexture.m_Type = value;
                else if (token.compare("mesh.material.texture.filepath") == 0)
                    tempTexture.m_FilePath = value;
                else if (xmlStack->Peek().compare("mesh.material.texture") == 0)
                    tempMaterial.m_Textures.push_back(tempTexture);
                // Mesh Material End
                else if (xmlStack->Peek().compare("mesh.material") == 0)
                    outMaterials.push_back(tempMaterial);
                // Count
                if (token.compare("count") == 0 && xmlStack->Parent().compare("camera") == 0)
                    cameraCount = std::stoi(value);
                else if (token.compare("count") == 0 && xmlStack->Parent().compare("mesh") == 0)
                    meshCount = std::stoi(value);

                // Model End
                if (xmlStack->Peek().compare("model") == 0)
                {
                    std::vector<GraphicsEngine::Mesh> meshes;
                    GraphicsEngine::Model model;
                    for(int32 i = 0; i < meshCount; i++)
                    {
                        GraphicsEngine::Mesh mesh{outNames[i], outVertices[i], outIndices[i], outMaterials[i]};
                        model.m_Meshes.push_back(mesh);
                    }
                    scene->m_Models.push_back(model);
                }

                token = "";
                value = "";
                prevChar = '/';
            }
            // XML Node Attributes
            else if (c == ' ')
            {
                // NOTE(neil): currently ignoring attributes for xml with break
                prevChar = c;
                xmlStack->Push(token);
                token = "";
                break;
            }
            // XML Node Start Close
            else if (c == '>')
            {
                if (xmlStack->Peek().compare(token) == 0)
                    xmlStack->Pop();
                else
                    xmlStack->Push(token);

                prevChar = c;
            }
            // XML Node End Close
            else if (prevChar == '>')
            {
                value.push_back(c);
            }
            else
            {
                token.push_back(c);
            }
        }
    }

    delete xmlStack;

    scene->m_IsModelLoaded = GraphicsEngine::ModelLoadState::FILE_LOADED;

    fileStream.close();

    return true;
}

}