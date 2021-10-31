#include "Serialize.h"

#include <algorithm>

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
    // Output to structure of a xml file
    /////////////////////////////////////

    int32 id = 0;
    int32 version = 1;

    fileOut << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>" << std::endl;
    
    /////////////////////////////////////
    // Scene - Root
    /////////////////////////////////////

    SerializeBeginTagXMLAttributes(fileOut, 0, "Scene", XMLAttribute{ 2, {"id", "version"}, {id, version} });

    /////////////////////////////////////
    // Camera
    /////////////////////////////////////

    GraphicsEngine::Camera* camera = &scene->m_Camera;
    SerializeBeginTagXMLAttributes(fileOut, 1, "Camera", XMLAttribute{ 1, {"id"}, {++id} });
    
    SerializeBeginTagXML(fileOut, 2, "Position");
    SerializeItemXML(fileOut, 3, "x", camera->m_Position.x);
    SerializeItemXML(fileOut, 3, "y", camera->m_Position.y);
    SerializeItemXML(fileOut, 3, "z", camera->m_Position.z);
    SerializeEndTagXML(fileOut, 2, "Position");

    SerializeBeginTagXML(fileOut, 2, "Front");
    SerializeItemXML(fileOut, 3, "x", camera->m_Front.x);
    SerializeItemXML(fileOut, 3, "y", camera->m_Front.y);
    SerializeItemXML(fileOut, 3, "z", camera->m_Front.z);
    SerializeEndTagXML(fileOut, 2, "Front");

    SerializeBeginTagXML(fileOut, 2, "Up");
    SerializeItemXML(fileOut, 3, "x", camera->m_Up.x);
    SerializeItemXML(fileOut, 3, "y", camera->m_Up.y);
    SerializeItemXML(fileOut, 3, "z", camera->m_Up.z);
    SerializeEndTagXML(fileOut, 2, "Up");

    SerializeBeginTagXML(fileOut, 2, "Right");
    SerializeItemXML(fileOut, 3, "x", camera->m_Right.x);
    SerializeItemXML(fileOut, 3, "y", camera->m_Right.y);
    SerializeItemXML(fileOut, 3, "z", camera->m_Right.z);
    SerializeEndTagXML(fileOut, 2, "Right");

    SerializeItemXML(fileOut, 2, "Yaw", camera->m_Yaw);
    SerializeItemXML(fileOut, 2, "Pitch", camera->m_Pitch);
    SerializeItemXML(fileOut, 2, "Fov", camera->m_Fov);
    SerializeItemXML(fileOut, 2, "MovementSpeed", camera->m_MovementSpeed);
    SerializeItemXML(fileOut, 2, "Sensitivity", camera->m_Sensitivity);

    SerializeEndTagXML(fileOut, 1, "Camera");

    /////////////////////////////////////
    // Models
    /////////////////////////////////////

    for (uint32 modelIndex = 0; modelIndex < scene->m_Models.size(); modelIndex++)
    {
        SerializeBeginTagXMLAttributes(fileOut, 1, "Model", XMLAttribute{ 1, {"id"}, {++id} });
        SerializeBeginTagXMLAttributes(fileOut, 2, "Array", XMLAttribute{ 1, {"size"}, {static_cast<int32>(scene->m_Models[modelIndex].m_Meshes.size())} });

        for (uint32 meshIndex = 0; meshIndex < scene->m_Models[modelIndex].m_Meshes.size(); meshIndex++)
        {
            GraphicsEngine::Mesh *mesh = &scene->m_Models[modelIndex].m_Meshes[meshIndex];

            SerializeBeginTagXMLAttributes(fileOut, 3, "Mesh", XMLAttribute{ 1, {"id"}, {++id} });

            SerializeItemXML(fileOut, 4, "Name", mesh->m_Name);

            SerializeBeginTagXML(fileOut, 4, "Vertices");
            for (const auto vertex : mesh->m_Vertices)
            {
                SerializeBeginTagXML(fileOut, 5, "Vertex");

                SerializeBeginTagXML(fileOut, 6, "Position");
                SerializeItemXML(fileOut, 7, "x", vertex.position.x);
                SerializeItemXML(fileOut, 7, "y", vertex.position.y);
                SerializeItemXML(fileOut, 7, "z", vertex.position.z);
                SerializeEndTagXML(fileOut, 6, "Position");

                SerializeBeginTagXML(fileOut, 6, "Normal");
                SerializeItemXML(fileOut, 7, "x", vertex.normal.x);
                SerializeItemXML(fileOut, 7, "y", vertex.normal.y);
                SerializeItemXML(fileOut, 7, "z", vertex.normal.z);
                SerializeEndTagXML(fileOut, 6, "Normal");

                SerializeBeginTagXML(fileOut, 6, "TextureUV");
                SerializeItemXML(fileOut, 7, "x", vertex.textureUV.x);
                SerializeItemXML(fileOut, 7, "y", vertex.textureUV.y);
                SerializeEndTagXML(fileOut, 6, "TextureUV");

                SerializeEndTagXML(fileOut, 5, "Vertex");
            }
            SerializeEndTagXML(fileOut, 4, "Vertices");

            SerializeBeginTagXML(fileOut, 4, "Indices");
            for (uint32 indicesIndex = 0; 
                 indicesIndex < mesh->m_Indices.size();
                 indicesIndex++)
            {
                SerializeItemXML(fileOut, 5, "Item", mesh->m_Indices[indicesIndex]);
            }
            SerializeEndTagXML(fileOut, 4, "Indices");

            // Material
            SerializeBeginTagXML(fileOut, 4, "Material");

            SerializeItemXML(fileOut, 5, "Name", mesh->m_Material.m_Name);

            SerializeBeginTagXML(fileOut, 5, "Ambient");
            SerializeItemXML(fileOut, 6, "x", mesh->m_Material.m_Ambient.x);
            SerializeItemXML(fileOut, 6, "y", mesh->m_Material.m_Ambient.y);
            SerializeItemXML(fileOut, 6, "z", mesh->m_Material.m_Ambient.z);
            SerializeEndTagXML(fileOut, 5, "Ambient");

            SerializeBeginTagXML(fileOut, 5, "Diffuse");
            SerializeItemXML(fileOut, 6, "x", mesh->m_Material.m_Diffuse.x);
            SerializeItemXML(fileOut, 6, "y", mesh->m_Material.m_Diffuse.y);
            SerializeItemXML(fileOut, 6, "z", mesh->m_Material.m_Diffuse.z);
            SerializeEndTagXML(fileOut, 5, "Diffuse");

            SerializeBeginTagXML(fileOut, 5, "Specular");
            SerializeItemXML(fileOut, 6, "x", mesh->m_Material.m_Specular.x);
            SerializeItemXML(fileOut, 6, "y", mesh->m_Material.m_Specular.y);
            SerializeItemXML(fileOut, 6, "z", mesh->m_Material.m_Specular.z);
            SerializeEndTagXML(fileOut, 5, "Specular");

            SerializeBeginTagXML(fileOut, 5, "Emissive");
            SerializeItemXML(fileOut, 6, "x", mesh->m_Material.m_Emissive.x);
            SerializeItemXML(fileOut, 6, "y", mesh->m_Material.m_Emissive.y);
            SerializeItemXML(fileOut, 6, "z", mesh->m_Material.m_Emissive.z);
            SerializeEndTagXML(fileOut, 5, "Emissive");

            SerializeItemXML(fileOut, 5, "Shininess", mesh->m_Material.m_Shininess);

            for (uint32 textureIndex = 0; 
                 textureIndex < mesh->m_Material.m_Textures.size(); 
                 textureIndex++)
            {
                SerializeBeginTagXML(fileOut, 5, "Texture");
                SerializeItemXML(fileOut, 6, "Type", mesh->m_Material.m_Textures[textureIndex].m_Type);

                std::string path = "";
                SerializeInsertAfter(mesh->m_Material.m_Textures[textureIndex].m_FilePath, path, '\\', '\\');
                SerializeItemXML(fileOut, 6, "FilePath", path);

                SerializeEndTagXML(fileOut, 5, "Texture");
            }
            SerializeEndTagXML(fileOut, 4, "Material");

            SerializeEndTagXML(fileOut, 3, "Mesh");
        }
        SerializeEndTagXML(fileOut, 2, "Array");
        SerializeEndTagXML(fileOut, 1, "Model");
    }
    SerializeEndTagXML(fileOut, 0, "Scene");

    return true;
}

void SerializeBeginTagXML(std::fstream &output, int32 indents, const std::string &tag)
{
    for (int32 i = 0; i < indents; ++i)
        output << "\t";
    
    output << "<";
    output << tag;
    output << ">";
    output << std::endl;
}

void SerializeBeginTagXMLAttributes(std::fstream& output, int32 indents, const std::string& tag, XMLAttribute attribute)
{
    ASSERT(attribute.attributeCount <= XML_ATTRIBUTE_MAX_SIZE);

    for (int32 i = 0; i < indents; ++i)
        output << "\t";
    
    output << "<";
    output << tag;

    for (int32 i = 0; i < attribute.attributeCount; ++i)
    {
        output << " ";
        output << attribute.name[i];
        output << "=\"";
        output << attribute.value[i];
        output << "\"";
    }

    output << ">";
    output << std::endl;
}

void SerializeEndTagXML(std::fstream& output, int32 indents, const std::string& tag)
{
    for (int32 i = 0; i < indents; ++i)
        output << "\t";
    
    output << "</";
    output << tag;
    output << ">";
    output << std::endl;
}

void SerializeItemXML(std::fstream& output, int32 indents, const std::string& tag, const std::string& value)
{
    for (int32 i = 0; i < indents; ++i)
        output << "\t";
    
    output << "<" << tag;
    output << ">";
    output << value;
    output << "</" << tag;
    output << ">" << std::endl;
}

void SerializeItemXML(std::fstream& output, int32 indents, const std::string& tag, const uint32 value)
{
    for (int32 i = 0; i < indents; ++i)
        output << "\t";
    
    output << "<" << tag;
    output << ">";
    output << value;
    output << "</" << tag;
    output << ">" << std::endl;
}

void SerializeItemXML(std::fstream& output, int32 indents, const std::string& tag, const real32 value)
{
    for(int32 i = 0; i < indents; ++i)
        output << "\t";
    
    output << "<" << tag;
    output << ">";
    output << value;
    output << "</" << tag;
    output << ">" << std::endl;
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

    GraphicsEngine::Vertex tempVertex;
    GraphicsEngine::Material tempMaterial;
    GraphicsEngine::Texture tempTexture;

    std::vector<GraphicsEngine::Vertex> tempVertices;
    std::vector<uint32> tempIndices;

    std::vector<std::string> meshNames;
    std::vector<std::vector<GraphicsEngine::Vertex>> meshVertices;
    std::vector<std::vector<uint32>> meshIndices;
    std::vector<GraphicsEngine::Material> meshMaterials;
    
    int32 meshCount = 0;

    std::string currentScope = "";

    if (scene->m_Models.size() > 0)
        scene->m_Models.clear();
    
    // Parse each line and de-serialize data
    while(!fileStream.eof())
    {
        std::string line = "";
        std::getline(fileStream, line);

        if (line.compare("<?xml version=\"1.0\" encoding=\"UTF-8\"?>") == 0)
            continue;

        // Parse line into tokens
        std::string token = "";
        std::vector<std::string> tokens;
        for (char c : line)
        {
            if (c != '\t')
            {
                if (c != '<'
                    && c != '>' 
                    && c != '"'
                    && c != ' ' 
                    && c != '='
                    )
                {
                    token.push_back(c);
                }
                else if (!token.empty())
                {
                    tokens.push_back(token);
                    token.clear();
                }
            }
        }

        ///////////////////////////////////////////
        // Go through tokens and de-serialize them
        ///////////////////////////////////////////

        // Opening XML tag
        std::string tag;
        if (!tokens.empty())
        {
            tag = tokens[0];
            xmlStack->Push(tag);
        }
        else
            break;
            
        // Mark current scope
        if (tag.compare("Camera") == 0)
            currentScope = "Camera";
        else if (tag.compare("/Camera") == 0)
        {
            currentScope = "";
            xmlStack->Pop();
        }
        else if (tag.compare("Mesh") == 0)
            currentScope = "Mesh";
        else if (tag.compare("/Mesh") == 0)
        {
            currentScope = "";
            xmlStack->Pop();
        }

        // Assign values to Camera
        else if (currentScope.compare("Camera") == 0)
        {
            if ((tag.compare("/Position") == 0) ||
                (tag.compare("/Front") == 0) ||
                (tag.compare("/Up") == 0) ||
                (tag.compare("/Right") == 0)
                )
            {
                xmlStack->Pop();
                xmlStack->Pop();
            }
            else if (xmlStack->Parent().compare("Position") == 0)
            {
                real32 value = std::stof(tokens[1]);
                if (tag.compare("x") == 0)
                    scene->m_Camera.m_Position[0] = value;
                else if (tag.compare("y") == 0)
                    scene->m_Camera.m_Position[1] = value;
                else if (tag.compare("z") == 0)
                    scene->m_Camera.m_Position[2] = value;
                xmlStack->Pop();
            }
            else if (xmlStack->Parent().compare("Front") == 0)
            {
                real32 value = std::stof(tokens[1]);
                if (tag.compare("x") == 0)
                    scene->m_Camera.m_Front[0] = value;
                else if (tag.compare("y") == 0)
                    scene->m_Camera.m_Front[1] = value;
                else if (tag.compare("z") == 0)
                    scene->m_Camera.m_Front[2] = value;
                xmlStack->Pop();
            }
            else if (xmlStack->Parent().compare("Up") == 0)
            {
                real32 value = std::stof(tokens[1]);
                if (tag.compare("x") == 0)
                    scene->m_Camera.m_Up[0] = value;
                else if (tag.compare("y") == 0)
                    scene->m_Camera.m_Up[1] = value;
                else if (tag.compare("z") == 0)
                    scene->m_Camera.m_Up[2] = value;
                xmlStack->Pop();
            }
            else if (xmlStack->Parent().compare("Right") == 0)
            {
                real32 value = std::stof(tokens[1]);
                if (tag.compare("x") == 0)
                    scene->m_Camera.m_Right[0] = value;
                else if (tag.compare("y") == 0)
                    scene->m_Camera.m_Right[1] = value;
                else if (tag.compare("z") == 0)
                    scene->m_Camera.m_Right[2] = value;
                xmlStack->Pop();
            }
            else if (tag.compare("Yaw") == 0)
            {
                scene->m_Camera.m_Yaw = std::stof(tokens[1]);
                xmlStack->Pop();
            }
            else if (tag.compare("Pitch") == 0)
            {
                scene->m_Camera.m_Pitch = std::stof(tokens[1]);
                xmlStack->Pop();
            }
            else if (tag.compare("Fov") == 0)
            {
                scene->m_Camera.m_Fov = std::stof(tokens[1]);
                xmlStack->Pop();
            }
            else if (tag.compare("MovementSpeed") == 0)
            {
                scene->m_Camera.m_MovementSpeed = std::stof(tokens[1]);
                xmlStack->Pop();
            }
            else if (tag.compare("Sensitivity") == 0)
            {
                scene->m_Camera.m_Sensitivity = std::stof(tokens[1]);
                xmlStack->Pop();
            }            
        }   // end of Camera

        // Assign values to Mesh
        else if (currentScope.compare("Mesh") == 0)
        {
            if ((tag.compare("/Position") == 0) ||
                (tag.compare("/Normal") == 0) ||
                (tag.compare("/TextureUV") == 0) ||
                (tag.compare("/Ambient") == 0) ||
                (tag.compare("/Diffuse") == 0) ||
                (tag.compare("/Specular") == 0) ||
                (tag.compare("/Emissive") == 0)
                )
            {
                xmlStack->Pop();
                xmlStack->Pop();
            }
            else if (tag.compare("/Texture") == 0)
            {
                tempMaterial.m_Textures.push_back(tempTexture);
                xmlStack->Pop();
                xmlStack->Pop();
            }
            else if (tag.compare("/Material") == 0)
            {
                meshMaterials.push_back(tempMaterial);
                xmlStack->Pop();
                xmlStack->Pop();
            }
            else if (tag.compare("/Vertex") == 0)
            {
                tempVertices.push_back(tempVertex);
                xmlStack->Pop();
                xmlStack->Pop();
            }
            else if (tag.compare("/Vertices") == 0)
            {
                meshVertices.push_back(tempVertices);
                tempVertices.clear();
                xmlStack->Pop();
                xmlStack->Pop();
            }
            else if (tag.compare("/Indices") == 0)
            {
                meshIndices.push_back(tempIndices);
                tempIndices.clear();
                xmlStack->Pop();
                xmlStack->Pop();
            }
            else if ((tag.compare("Name") == 0) && 
                     (xmlStack->Parent().compare("Mesh") == 0))
            {
                meshNames.push_back(tokens[1]);
                xmlStack->Pop();
            }
            else if (xmlStack->Parent().compare("Position") == 0)
            {
                real32 value = std::stof(tokens[1]);
                if (tag.compare("x") == 0)
                    tempVertex.position.x = value;
                else if (tag.compare("y") == 0)
                    tempVertex.position.y = value;
                else if (tag.compare("z") == 0)
                    tempVertex.position.z = value;
                xmlStack->Pop();
            }
            else if (xmlStack->Parent().compare("Normal") == 0)
            {
                real32 value = std::stof(tokens[1]);
                if (tag.compare("x") == 0)
                    tempVertex.normal.x = value;
                else if (tag.compare("y") == 0)
                    tempVertex.normal.y = value;
                else if (tag.compare("z") == 0)
                    tempVertex.normal.z = value;
                xmlStack->Pop();
            }
            else if (xmlStack->Parent().compare("TextureUV") == 0)
            {
                real32 value = std::stof(tokens[1]);
                if (tag.compare("x") == 0)
                    tempVertex.textureUV.x = value;
                else if (tag.compare("y") == 0)
                    tempVertex.textureUV.y = value;
                xmlStack->Pop();
            }
            else if (xmlStack->Parent().compare("Indices") == 0)
            {
                tempIndices.push_back(std::stoi(tokens[1]));
                xmlStack->Pop();
            }
            else if ((tag.compare("Name") == 0) && 
                     (xmlStack->Parent().compare("Material") == 0))
            {
                tempMaterial.m_Name = tokens[1];
                xmlStack->Pop();
            }
            else if (xmlStack->Parent().compare("Ambient") == 0)
            {
                real32 value = std::stof(tokens[1]);
                if (tag.compare("x") == 0)
                    tempMaterial.m_Ambient.x = value;
                else if (tag.compare("y") == 0)
                    tempMaterial.m_Ambient.y = value;
                else if (tag.compare("z") == 0)
                    tempMaterial.m_Ambient.z = value;
                xmlStack->Pop();
            }
            else if (xmlStack->Parent().compare("Diffuse") == 0)
            {
                real32 value = std::stof(tokens[1]);
                if (tag.compare("x") == 0)
                    tempMaterial.m_Diffuse.x = value;
                else if (tag.compare("y") == 0)
                    tempMaterial.m_Diffuse.y = value;
                else if (tag.compare("z") == 0)
                    tempMaterial.m_Diffuse.z = value;
                xmlStack->Pop();
            }
            else if (xmlStack->Parent().compare("Specular") == 0)
            {
                real32 value = std::stof(tokens[1]);
                if (tag.compare("x") == 0)
                    tempMaterial.m_Specular.x = value;
                else if (tag.compare("y") == 0)
                    tempMaterial.m_Specular.y = value;
                else if (tag.compare("z") == 0)
                    tempMaterial.m_Specular.z = value;
                xmlStack->Pop();
            }
            else if (xmlStack->Parent().compare("Emissive") == 0)
            {
                real32 value = std::stof(tokens[1]);
                if (tag.compare("x") == 0)
                    tempMaterial.m_Emissive.x = value;
                else if (tag.compare("y") == 0)
                    tempMaterial.m_Emissive.y = value;
                else if (tag.compare("z") == 0)
                    tempMaterial.m_Emissive.z = value;
                xmlStack->Pop();
            }
            else if ((tag.compare("Shininess") == 0) && 
                     (xmlStack->Parent().compare("Material") == 0))
            {
                tempMaterial.m_Shininess = std::stof(tokens[1]);
                xmlStack->Pop();
            }
            else if ((tag.compare("Type") == 0) && 
                     (xmlStack->Parent().compare("Texture") == 0))
            {
                tempTexture.m_Type = tokens[1];
                xmlStack->Pop();
            }
            else if ((tag.compare("FilePath") == 0) && 
                     (xmlStack->Parent().compare("Texture") == 0))
            {
                tempTexture.m_FilePath = tokens[1];
                xmlStack->Pop();
            }            
        }   // end of Mesh

        // Array check
        else if ((tag.compare("Array") == 0) && 
                 (xmlStack->Parent().compare("Model") == 0))
            meshCount = std::stoi(tokens[2]);
        else if ((tag.compare("/Array") == 0) &&
                 (xmlStack->Parent().compare("Model") == 0))
            xmlStack->Pop();

        // Add Mesh to models
        else if (tag.compare("/Model") == 0)
        {
            std::vector<GraphicsEngine::Mesh> meshes;
            GraphicsEngine::Model model;
            for(int32 i = 0; i < meshCount; i++)
            {
                GraphicsEngine::Mesh mesh{meshNames[i], meshVertices[i], meshIndices[i], meshMaterials[i]};
                model.m_Meshes.push_back(mesh);
            }
            scene->m_Models.push_back(model);
            xmlStack->Pop();
        }

    }   // end of fileStream 

    delete xmlStack;
    scene->m_IsModelLoaded = GraphicsEngine::ModelLoadState::FILE_LOADED;
    fileStream.close();
    return true;
}

bool SerializeToJSON(const std::string &filePath, GraphicsEngine::Scene *scene)
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
    // Output to structure of a json file
    /////////////////////////////////////

    uint32 id = 0;
    uint32 version = 1;

    // Opening
    fileOut << CURLY_BRACKET_START << std::endl;

    /////////////////////////////////////
    // Camera
    /////////////////////////////////////

    GraphicsEngine::Camera* camera = &scene->m_Camera;
    
    SerializeBeginObjectJSON(fileOut, 1, "Camera", CURLY_BRACKET_START);

    SerializeItemJSON(fileOut, 2, "Id", id++, true);

    SerializeBeginObjectJSON(fileOut, 2, "Position", CURLY_BRACKET_START);
    SerializeItemJSON(fileOut, 3, "x", camera->m_Position.x, true);
    SerializeItemJSON(fileOut, 3, "y", camera->m_Position.y, true);
    SerializeItemJSON(fileOut, 3, "z", camera->m_Position.z, false);
    SerializeEndJSON(fileOut, 2, CURLY_BRACKET_END, true); // Position

    SerializeBeginObjectJSON(fileOut, 2, "Front", CURLY_BRACKET_START);
    SerializeItemJSON(fileOut, 3, "x", camera->m_Front.x, true);
    SerializeItemJSON(fileOut, 3, "y", camera->m_Front.y, true);
    SerializeItemJSON(fileOut, 3, "z", camera->m_Front.z, false);
    SerializeEndJSON(fileOut, 2, CURLY_BRACKET_END, true); // Front

    SerializeBeginObjectJSON(fileOut, 2, "Up", CURLY_BRACKET_START);
    SerializeItemJSON(fileOut, 3, "x", camera->m_Up.x, true);
    SerializeItemJSON(fileOut, 3, "y", camera->m_Up.y, true);
    SerializeItemJSON(fileOut, 3, "z", camera->m_Up.z, false);
    SerializeEndJSON(fileOut, 2, CURLY_BRACKET_END, true); // Up

    SerializeBeginObjectJSON(fileOut, 2, "Right", CURLY_BRACKET_START);
    SerializeItemJSON(fileOut, 3, "x", camera->m_Right.x, true);
    SerializeItemJSON(fileOut, 3, "y", camera->m_Right.y, true);
    SerializeItemJSON(fileOut, 3, "z", camera->m_Right.z, false);
    SerializeEndJSON(fileOut, 2, CURLY_BRACKET_END, true); // Right

    SerializeItemJSON(fileOut, 2, "Yaw", camera->m_Yaw, true);
    SerializeItemJSON(fileOut, 2, "Pitch", camera->m_Pitch, true);
    SerializeItemJSON(fileOut, 2, "Fov", camera->m_Fov, true);
    SerializeItemJSON(fileOut, 2, "MovementSpeed", camera->m_MovementSpeed, true);
    SerializeItemJSON(fileOut, 2, "Sensitivity", camera->m_Sensitivity, false);

    SerializeEndJSON(fileOut, 1, CURLY_BRACKET_END, true); // Camera

    /////////////////////////////////////
    // Models
    /////////////////////////////////////

    SerializeBeginObjectJSON(fileOut, 1, "Model", SQUARE_BRACKET_START);

    for (uint32 modelIndex = 0; modelIndex < scene->m_Models.size(); modelIndex++)
    {
        SerializeBeginJSON(fileOut, 2, CURLY_BRACKET_START);
        SerializeItemJSON(fileOut, 3, "Id", id++, true);
        SerializeItemJSON(fileOut, 3, "Size", static_cast<uint32>(scene->m_Models[modelIndex].m_Meshes.size()), true);

        SerializeBeginObjectJSON(fileOut, 3, "Mesh", SQUARE_BRACKET_START);
        for (uint32 meshIndex = 0; meshIndex < scene->m_Models[modelIndex].m_Meshes.size(); meshIndex++)
        {
            GraphicsEngine::Mesh *mesh = &scene->m_Models[modelIndex].m_Meshes[meshIndex];
            
            SerializeBeginJSON(fileOut, 4, CURLY_BRACKET_START);
            SerializeItemJSON(fileOut, 5, "Id", id++, true);
            SerializeItemJSON(fileOut, 5, "Name", mesh->m_Name, true);

            SerializeBeginObjectJSON(fileOut, 5, "Vertices", SQUARE_BRACKET_START);
            for (int vIndex = 0; vIndex < mesh->m_Vertices.size(); vIndex++)
            {
                SerializeBeginJSON(fileOut, 6, CURLY_BRACKET_START);

                SerializeBeginObjectJSON(fileOut, 7, "Position", CURLY_BRACKET_START);
                SerializeItemJSON(fileOut, 8, "x", mesh->m_Vertices[vIndex].position.x, true);
                SerializeItemJSON(fileOut, 8, "y", mesh->m_Vertices[vIndex].position.y, true);
                SerializeItemJSON(fileOut, 8, "z", mesh->m_Vertices[vIndex].position.z, false);
                SerializeEndJSON(fileOut, 7, CURLY_BRACKET_END, true);

                SerializeBeginObjectJSON(fileOut, 7, "Normal", CURLY_BRACKET_START);
                SerializeItemJSON(fileOut, 8, "x", mesh->m_Vertices[vIndex].normal.x, true);
                SerializeItemJSON(fileOut, 8, "y", mesh->m_Vertices[vIndex].normal.y, true);
                SerializeItemJSON(fileOut, 8, "z", mesh->m_Vertices[vIndex].normal.z, false);
                SerializeEndJSON(fileOut, 7, CURLY_BRACKET_END, true);

                SerializeBeginObjectJSON(fileOut, 7, "TextureUV", CURLY_BRACKET_START);
                SerializeItemJSON(fileOut, 8, "x", mesh->m_Vertices[vIndex].textureUV.x, true);
                SerializeItemJSON(fileOut, 8, "y", mesh->m_Vertices[vIndex].textureUV.y, false);
                SerializeEndJSON(fileOut, 7, CURLY_BRACKET_END, false);

                if (vIndex != mesh->m_Vertices.size()-1)
                    SerializeEndJSON(fileOut, 6, CURLY_BRACKET_END, true);
                else
                    SerializeEndJSON(fileOut, 6, CURLY_BRACKET_END, false);
            }
            SerializeEndJSON(fileOut, 5, SQUARE_BRACKET_END, true); // Vertices

            // Indices
            SerializeBeginObjectJSON(fileOut, 5, "Indices", SQUARE_BRACKET_START);
            for (uint32 indicesIndex = 0;
                 indicesIndex < mesh->m_Indices.size()-1;
                 indicesIndex++)
            {
                SerializeItemNoKeyJSON(fileOut, 6, mesh->m_Indices[indicesIndex], true);
            }
            // Last index
            SerializeItemNoKeyJSON(fileOut, 6, mesh->m_Indices[mesh->m_Indices.size()-1], false);
            SerializeEndJSON(fileOut, 5, SQUARE_BRACKET_END, true); // indices

            // Material
            SerializeBeginObjectJSON(fileOut, 5, "Material", CURLY_BRACKET_START);
            SerializeItemJSON(fileOut, 6, "Name", mesh->m_Material.m_Name, true);

            SerializeBeginObjectJSON(fileOut, 6, "Ambient", CURLY_BRACKET_START);
            SerializeItemJSON(fileOut, 7, "x", mesh->m_Material.m_Ambient.x, true);
            SerializeItemJSON(fileOut, 7, "y", mesh->m_Material.m_Ambient.y, true);
            SerializeItemJSON(fileOut, 7, "z", mesh->m_Material.m_Ambient.z, false);
            SerializeEndJSON(fileOut, 6, CURLY_BRACKET_END, true); // Ambient

            SerializeBeginObjectJSON(fileOut, 6, "Diffuse", CURLY_BRACKET_START);
            SerializeItemJSON(fileOut, 7, "x", mesh->m_Material.m_Diffuse.x, true);
            SerializeItemJSON(fileOut, 7, "y", mesh->m_Material.m_Diffuse.y, true);
            SerializeItemJSON(fileOut, 7, "z", mesh->m_Material.m_Diffuse.z, false);
            SerializeEndJSON(fileOut, 6, CURLY_BRACKET_END, true); // Diffuse

            SerializeBeginObjectJSON(fileOut, 6, "Specular", CURLY_BRACKET_START);
            SerializeItemJSON(fileOut, 7, "x", mesh->m_Material.m_Specular.x, true);
            SerializeItemJSON(fileOut, 7, "y", mesh->m_Material.m_Specular.y, true);
            SerializeItemJSON(fileOut, 7, "z", mesh->m_Material.m_Specular.z, false);
            SerializeEndJSON(fileOut, 6, CURLY_BRACKET_END, true); // Specular

            SerializeBeginObjectJSON(fileOut, 6, "Emissive", CURLY_BRACKET_START);
            SerializeItemJSON(fileOut, 7, "x", mesh->m_Material.m_Emissive.x, true);
            SerializeItemJSON(fileOut, 7, "y", mesh->m_Material.m_Emissive.y, true);
            SerializeItemJSON(fileOut, 7, "z", mesh->m_Material.m_Emissive.z, false);
            SerializeEndJSON(fileOut, 6, CURLY_BRACKET_END, true); // Emissive

            SerializeItemJSON(fileOut, 6, "Shininess", mesh->m_Material.m_Shininess, true);

            SerializeBeginObjectJSON(fileOut, 6, "Textures", SQUARE_BRACKET_START);
            std::string path = "";
            for (uint32 textureIndex = 0;
                 textureIndex < mesh->m_Material.m_Textures.size()-1;
                 textureIndex++)
            {
                SerializeInsertAfter(mesh->m_Material.m_Textures[textureIndex].m_FilePath, path, '\\', '\\');
                SerializeBeginJSON(fileOut, 7, CURLY_BRACKET_START);
                SerializeItemJSON(fileOut, 8, "Type", mesh->m_Material.m_Textures[textureIndex].m_Type, true);
                SerializeItemJSON(fileOut, 8, "FilePath", path, false);
                SerializeEndJSON(fileOut, 7, CURLY_BRACKET_END, true);
            }
            // Last index
            SerializeInsertAfter(mesh->m_Material.m_Textures[mesh->m_Material.m_Textures.size()-1].m_FilePath, path, '\\', '\\');
            SerializeBeginJSON(fileOut, 7, CURLY_BRACKET_START);
            SerializeItemJSON(fileOut, 8, "Type", mesh->m_Material.m_Textures[mesh->m_Material.m_Textures.size()-1].m_Type, true);
            SerializeItemJSON(fileOut, 8, "FilePath", path, false);
            SerializeEndJSON(fileOut, 7, CURLY_BRACKET_END, false);

            SerializeEndJSON(fileOut, 6, SQUARE_BRACKET_END, false); // textures
            SerializeEndJSON(fileOut, 5, CURLY_BRACKET_END, false); // material

            if (meshIndex != scene->m_Models[modelIndex].m_Meshes.size()-1)
                SerializeEndJSON(fileOut, 4, CURLY_BRACKET_END, true); // mesh
            else
                SerializeEndJSON(fileOut, 4, CURLY_BRACKET_END, false); // mesh
        }
        SerializeEndJSON(fileOut, 3, SQUARE_BRACKET_END, false); // model
        if (modelIndex != scene->m_Models.size()-1)
            SerializeEndJSON(fileOut, 2, CURLY_BRACKET_END, true); // model
        else
            SerializeEndJSON(fileOut, 2, CURLY_BRACKET_END, false); // model
    }
    SerializeEndJSON(fileOut, 1, SQUARE_BRACKET_END, false); // model

    // Closing
    fileOut << CURLY_BRACKET_END << std::endl;

    return true;
}

void SerializeBeginJSON(std::fstream& output, int32 indents, char bracket)
{
    for (int32 i = 0; i < indents; ++i)
        output << "\t";

    output << bracket;
    output << std::endl;
}

void SerializeBeginObjectJSON(std::fstream& output, int32 indents, const std::string& object, char bracket)
{
    for (int32 i = 0; i < indents; ++i)
        output << "\t";

    output << "\"";
    output << object;
    output << "\"";
    output << ":";
    output << " ";
    output << bracket;
    output << std::endl;
}

void SerializeEndJSON(std::fstream& output, int32 indents, char bracket, bool comma)
{
    for (int32 i = 0; i < indents; ++i)
        output << "\t";

    output << bracket;
    if (comma)
        output << ",";
    output << std::endl;
}

void SerializeItemJSON(std::fstream& output, int32 indents, const std::string& key, const std::string& value, bool comma)
{
    for (int32 i = 0; i < indents; ++i)
        output << "\t";

    output << "\"";
    output << key;
    output << "\"";
    output << ":";
    output << " ";
    output << "\"";
    output << value;
    output <<  "\"";
    if (comma)
        output << ",";
    output << std::endl;
}

void SerializeItemJSON(std::fstream& output, int32 indents, const std::string& key, const uint32 value, bool comma)
{
    for (int32 i = 0; i < indents; ++i)
        output << "\t";

    output << "\"";
    output << key;
    output << "\"";
    output << ":";
    output << " ";
    output << value;
    if (comma)
        output << ",";
    output << std::endl;
}

void SerializeItemJSON(std::fstream& output, int32 indents, const std::string& key, const real32 value, bool comma)
{
    for (int32 i = 0; i < indents; ++i)
        output << "\t";

    output << "\"";
    output << key;
    output << "\"";
    output << ":";
    output << " ";
    output << value;
    if (comma)
        output << ",";
    output << std::endl;
}

void SerializeItemNoKeyJSON(std::fstream& output, int32 indents, const uint32 value, bool comma)
{
    for (int32 i = 0; i < indents; ++i)
        output << "\t";

    output << value;
    if (comma)
        output << ",";
    output << std::endl;
}


bool SerializeToYAML(const std::string& filePath, GraphicsEngine::Scene* scene)
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
    // Output to structure of a yaml file
    /////////////////////////////////////

    uint32 id = 0;
    uint32 version = 1;

    fileOut << "# Bananas Import/Export v1.0" << std::endl;

    /////////////////////////////////////
    // Camera
    /////////////////////////////////////

    SerializeBeginYAML(fileOut, 0, "Camera");
    SerializeItemYAML(fileOut, 1, "Id", id++, YAML_DASH_START);

    SerializeVector3DYAML(fileOut, 1, "Position", scene->m_Camera.m_Position.x, 
                                                  scene->m_Camera.m_Position.y, 
                                                  scene->m_Camera.m_Position.z, 
                                                  YAML_DASH_START);
    SerializeVector3DYAML(fileOut, 1, "Front", scene->m_Camera.m_Front.x, 
                                               scene->m_Camera.m_Front.y, 
                                               scene->m_Camera.m_Front.z, 
                                               YAML_DASH_START);
    SerializeVector3DYAML(fileOut, 1, "Up", scene->m_Camera.m_Up.x, 
                                            scene->m_Camera.m_Up.y, 
                                            scene->m_Camera.m_Up.z, 
                                            YAML_DASH_START);
    SerializeVector3DYAML(fileOut, 1, "Right", scene->m_Camera.m_Right.x, 
                                               scene->m_Camera.m_Right.y, 
                                               scene->m_Camera.m_Right.z, 
                                               YAML_DASH_START);

    SerializeItemYAML(fileOut, 1, "Yaw", scene->m_Camera.m_Yaw, YAML_DASH_START);
    SerializeItemYAML(fileOut, 1, "Pitch", scene->m_Camera.m_Pitch, YAML_DASH_START);
    SerializeItemYAML(fileOut, 1, "Fov", scene->m_Camera.m_Fov, YAML_DASH_START);
    SerializeItemYAML(fileOut, 1, "MovementSpeed", scene->m_Camera.m_MovementSpeed, YAML_DASH_START);
    SerializeItemYAML(fileOut, 1, "Sensitivity", scene->m_Camera.m_Sensitivity, YAML_DASH_START);

    /////////////////////////////////////
    // Models
    /////////////////////////////////////


    for (uint32 modelIndex = 0; modelIndex < scene->m_Models.size(); modelIndex++)
    {
        SerializeBeginYAML(fileOut, 0, "Model");
        SerializeItemYAML(fileOut, 1, "Id", id++, YAML_DASH_START);
        SerializeItemYAML(fileOut, 1, "Size", static_cast<uint32>(scene->m_Models[modelIndex].m_Meshes.size()), YAML_DASH_START);

        for (uint32 meshIndex = 0; meshIndex < scene->m_Models[modelIndex].m_Meshes.size(); meshIndex++)
        {
            GraphicsEngine::Mesh *mesh = &scene->m_Models[modelIndex].m_Meshes[meshIndex];

            SerializeBeginYAML(fileOut, 1, "Mesh");
            SerializeItemYAML(fileOut, 2, "Id", id++, YAML_DASH_START);
            SerializeItemYAML(fileOut, 2, "Name", mesh->m_Name, YAML_DASH_START);
            
            SerializeBeginYAML(fileOut, 2, "Vertices");
            for (const auto vertex : mesh->m_Vertices)
            {
                SerializeVector3DYAML(fileOut, 3, "Position", vertex.position.x, vertex.position.y, vertex.position.z, YAML_DASH_START);
                SerializeVector3DYAML(fileOut, 3, "Normal", vertex.normal.x, vertex.normal.y, vertex.normal.z, YAML_SPACE_START);
                SerializeVector2DYAML(fileOut, 3, "TextureUV", vertex.textureUV.x, vertex.textureUV.y, YAML_SPACE_START);
            }

            // Indices
            fileOut << "    " << "- Indices: [";
            for (uint32 indicesIndex = 0;
                 indicesIndex < mesh->m_Indices.size()-1;
                 indicesIndex++)
            {
                fileOut << mesh->m_Indices[indicesIndex] << ", ";
            }
            fileOut << mesh->m_Indices[mesh->m_Indices.size()-1];
            fileOut << "]" << std::endl;

            SerializeBeginYAML(fileOut, 2, "Material");
            SerializeItemYAML(fileOut, 3, "Name", mesh->m_Material.m_Name, YAML_DASH_START);
            SerializeVector3DYAML(fileOut, 3, "Ambient", mesh->m_Material.m_Ambient.x, 
                                                         mesh->m_Material.m_Ambient.y, 
                                                         mesh->m_Material.m_Ambient.z, 
                                                         YAML_DASH_START);
            SerializeVector3DYAML(fileOut, 3, "Diffuse", mesh->m_Material.m_Diffuse.x, 
                                                         mesh->m_Material.m_Diffuse.y, 
                                                         mesh->m_Material.m_Diffuse.z, 
                                                         YAML_DASH_START);
            SerializeVector3DYAML(fileOut, 3, "Specular", mesh->m_Material.m_Specular.x, 
                                                          mesh->m_Material.m_Specular.y, 
                                                          mesh->m_Material.m_Specular.z, 
                                                          YAML_DASH_START);
            SerializeVector3DYAML(fileOut, 3, "Emissive", mesh->m_Material.m_Emissive.x, 
                                                          mesh->m_Material.m_Emissive.y, 
                                                          mesh->m_Material.m_Emissive.z, 
                                                          YAML_DASH_START);
            SerializeItemYAML(fileOut, 3, "Shininess", mesh->m_Material.m_Shininess, YAML_DASH_START);

            SerializeBeginYAML(fileOut, 3, "Textures");
            std::string path = "";
            for (uint32 textureIndex = 0;
                 textureIndex < mesh->m_Material.m_Textures.size();
                 textureIndex++)
            {
                SerializeItemYAML(fileOut, 4, "Type", mesh->m_Material.m_Textures[textureIndex].m_Type, YAML_DASH_START);
                
                SerializeInsertAfter(mesh->m_Material.m_Textures[textureIndex].m_FilePath, path, '\\', '\\');
                SerializeItemYAML(fileOut, 4, "FilePath", path, YAML_SPACE_START);
            }
        }
    }

    return true;
}

void SerializeBeginYAML(std::fstream& output, int32 indents, const std::string& name)
{
    for (int32 i = 0; i < indents * TAB_SIZE; ++i)
        output << " ";

    output << "- ";
    output << name;
    output << ":";
    output << std::endl;
}

void SerializeItemYAML(std::fstream& output, int32 indents, const std::string& key, const std::string& value, const char* start)
{
    for (int32 i = 0; i < indents * TAB_SIZE; ++i)
        output << " ";

    output << start;
    output << key;
    output << ": ";
    output << value;
    output << std::endl;
}

void SerializeItemYAML(std::fstream& output, int32 indents, const std::string& key, uint32 value, const char* start)
{
    for (int32 i = 0; i < indents * TAB_SIZE; ++i)
        output << " ";

    output << start;
    output << key;
    output << ": ";
    output << value;
    output << std::endl;
}

void SerializeItemYAML(std::fstream& output, int32 indents, const std::string& key, real32 value, const char* start)
{
    for (int32 i = 0; i < indents * TAB_SIZE; ++i)
        output << " ";

    output << start;
    output << key;
    output << ": ";
    output << value;
    output << std::endl;
}

void SerializeVector3DYAML(std::fstream& output, int32 indents, const std::string& key, real32 x, real32 y, real32 z, const char* start)
{
    for (int32 i = 0; i < indents * TAB_SIZE; ++i)
        output << " ";

    output << start;
    output << key;
    output << ": ";
    
    output << CURLY_BRACKET_START;
    output << "x:";
    output << x;
    output << ", ";
    output << "y:";
    output << y;
    output << ", ";
    output << "z:";
    output << z;
    output << CURLY_BRACKET_END;
    
    output << std::endl;
}

void SerializeVector2DYAML(std::fstream& output, int32 indents, const std::string& key, real32 x, real32 y, const char* start)
{
    for (int32 i = 0; i < indents * TAB_SIZE; ++i)
        output << " ";

    output << start;
    output << key;
    output << ": ";
    
    output << CURLY_BRACKET_START;
    output << "x:";
    output << x;
    output << ", ";
    output << "y:";
    output << y;
    output << CURLY_BRACKET_END;
    
    output << std::endl;
}

void SerializeInsertAfter(const std::string& lhs, std::string& rhs, const char r, const char i)
{
    for (const char c : lhs)
    {
        rhs.push_back(c);
        if (c == r)
            rhs.push_back(i);
    }    
}


}