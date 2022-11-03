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

    fileOut << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>" << '\n';
    
    /////////////////////////////////////
    // Scene - Root
    /////////////////////////////////////

    SerializeBeginTagXMLAttributes(fileOut, 0, "Scene", XMLAttribute{ 2, {"id", "version"}, {id, version} });

    /////////////////////////////////////
    // Camera
    /////////////////////////////////////

    GraphicsEngine::Camera* camera = &scene->m_camera;
    SerializeBeginTagXMLAttributes(fileOut, 1, "Camera", XMLAttribute{ 1, {"id"}, {++id} });
    
    SerializeBeginTagXML(fileOut, 2, "Position");
    SerializeItemXML(fileOut, 3, "x", camera->m_position.x);
    SerializeItemXML(fileOut, 3, "y", camera->m_position.y);
    SerializeItemXML(fileOut, 3, "z", camera->m_position.z);
    SerializeEndTagXML(fileOut, 2, "Position");

    SerializeBeginTagXML(fileOut, 2, "Front");
    SerializeItemXML(fileOut, 3, "x", camera->m_front.x);
    SerializeItemXML(fileOut, 3, "y", camera->m_front.y);
    SerializeItemXML(fileOut, 3, "z", camera->m_front.z);
    SerializeEndTagXML(fileOut, 2, "Front");

    SerializeBeginTagXML(fileOut, 2, "Up");
    SerializeItemXML(fileOut, 3, "x", camera->m_up.x);
    SerializeItemXML(fileOut, 3, "y", camera->m_up.y);
    SerializeItemXML(fileOut, 3, "z", camera->m_up.z);
    SerializeEndTagXML(fileOut, 2, "Up");

    SerializeBeginTagXML(fileOut, 2, "Right");
    SerializeItemXML(fileOut, 3, "x", camera->m_right.x);
    SerializeItemXML(fileOut, 3, "y", camera->m_right.y);
    SerializeItemXML(fileOut, 3, "z", camera->m_right.z);
    SerializeEndTagXML(fileOut, 2, "Right");

    SerializeItemXML(fileOut, 2, "Yaw", camera->m_yaw);
    SerializeItemXML(fileOut, 2, "Pitch", camera->m_pitch);
    SerializeItemXML(fileOut, 2, "Fov", camera->m_fov);
    SerializeItemXML(fileOut, 2, "MovementSpeed", camera->m_movementSpeed);
    SerializeItemXML(fileOut, 2, "Sensitivity", camera->m_sensitivity);

    SerializeEndTagXML(fileOut, 1, "Camera");

    /////////////////////////////////////
    // Models
    /////////////////////////////////////

    for (uint32 modelIndex = 0; modelIndex < scene->m_models.size(); modelIndex++)
    {
        SerializeBeginTagXMLAttributes(fileOut, 1, "Model", XMLAttribute{ 1, {"id"}, {++id} });
        // TODO(neil): Add in model name to Serialize.
        SerializeBeginTagXMLAttributes(fileOut, 2, "Array", XMLAttribute{ 1, {"size"}, {static_cast<int32>(scene->m_models[modelIndex].m_meshes.size())} });

        for (uint32 meshIndex = 0; meshIndex < scene->m_models[modelIndex].m_meshes.size(); meshIndex++)
        {
            GraphicsEngine::Mesh *mesh = &scene->m_models[modelIndex].m_meshes[meshIndex];

            SerializeBeginTagXMLAttributes(fileOut, 3, "Mesh", XMLAttribute{ 1, {"id"}, {++id} });

            SerializeItemXML(fileOut, 4, "Name", mesh->m_name);

            SerializeBeginTagXML(fileOut, 4, "Vertices");
            for (const auto& vertex : mesh->m_vertices)
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
                 indicesIndex < mesh->m_indices.size();
                 indicesIndex++)
            {
                SerializeItemXML(fileOut, 5, "Item", mesh->m_indices[indicesIndex]);
            }
            SerializeEndTagXML(fileOut, 4, "Indices");

            // Material
            SerializeBeginTagXML(fileOut, 4, "Material");

            SerializeItemXML(fileOut, 5, "Name", mesh->m_material.m_name);

            SerializeBeginTagXML(fileOut, 5, "Ambient");
            SerializeItemXML(fileOut, 6, "x", mesh->m_material.m_ambient.x);
            SerializeItemXML(fileOut, 6, "y", mesh->m_material.m_ambient.y);
            SerializeItemXML(fileOut, 6, "z", mesh->m_material.m_ambient.z);
            SerializeEndTagXML(fileOut, 5, "Ambient");

            SerializeBeginTagXML(fileOut, 5, "Diffuse");
            SerializeItemXML(fileOut, 6, "x", mesh->m_material.m_diffuse.x);
            SerializeItemXML(fileOut, 6, "y", mesh->m_material.m_diffuse.y);
            SerializeItemXML(fileOut, 6, "z", mesh->m_material.m_diffuse.z);
            SerializeEndTagXML(fileOut, 5, "Diffuse");

            SerializeBeginTagXML(fileOut, 5, "Specular");
            SerializeItemXML(fileOut, 6, "x", mesh->m_material.m_specular.x);
            SerializeItemXML(fileOut, 6, "y", mesh->m_material.m_specular.y);
            SerializeItemXML(fileOut, 6, "z", mesh->m_material.m_specular.z);
            SerializeEndTagXML(fileOut, 5, "Specular");

            SerializeBeginTagXML(fileOut, 5, "Emissive");
            SerializeItemXML(fileOut, 6, "x", mesh->m_material.m_emissive.x);
            SerializeItemXML(fileOut, 6, "y", mesh->m_material.m_emissive.y);
            SerializeItemXML(fileOut, 6, "z", mesh->m_material.m_emissive.z);
            SerializeEndTagXML(fileOut, 5, "Emissive");

            SerializeItemXML(fileOut, 5, "Shininess", mesh->m_material.m_shininess);

            for (uint32 textureIndex = 0; 
                 textureIndex < mesh->m_material.m_textures.size(); 
                 textureIndex++)
            {
                SerializeBeginTagXML(fileOut, 5, "Texture");
                SerializeItemXML(fileOut, 6, "Type", mesh->m_material.m_textures[textureIndex].m_type);
                SerializeItemXML(fileOut, 6, "FilePath", mesh->m_material.m_textures[textureIndex].m_filePath);
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
    output << '\n';
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
    output << '\n';
}

void SerializeEndTagXML(std::fstream& output, int32 indents, const std::string& tag)
{
    for (int32 i = 0; i < indents; ++i)
        output << "\t";
    
    output << "</";
    output << tag;
    output << ">";
    output << '\n';
}

void SerializeItemXML(std::fstream& output, int32 indents, const std::string& tag, const std::string& value)
{
    for (int32 i = 0; i < indents; ++i)
        output << "\t";
    
    output << "<" << tag;
    output << ">";
    output << value;
    output << "</" << tag;
    output << ">" << '\n';
}

void SerializeItemXML(std::fstream& output, int32 indents, const std::string& tag, const uint32 value)
{
    for (int32 i = 0; i < indents; ++i)
        output << "\t";
    
    output << "<" << tag;
    output << ">";
    output << value;
    output << "</" << tag;
    output << ">" << '\n';
}

void SerializeItemXML(std::fstream& output, int32 indents, const std::string& tag, const real32 value)
{
    for(int32 i = 0; i < indents; ++i)
        output << "\t";
    
    output << "<" << tag;
    output << ">";
    output << value;
    output << "</" << tag;
    output << ">" << '\n';
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

    Stack* stack = new Stack();

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

    if (scene->m_models.size() > 0)
        scene->m_models.clear();
    
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
            stack->Push(tag);
        }
        else
            continue;

        // TODO(neil): Add in model name to Deserialize.
            
        // Mark current scope
        if (tag.compare("Camera") == 0)
            currentScope = "Camera";
        else if (tag.compare("/Camera") == 0)
        {
            currentScope.clear();
            stack->Pop();
        }
        else if (tag.compare("Mesh") == 0)
            currentScope = "Mesh";
        else if (tag.compare("/Mesh") == 0)
        {
            currentScope.clear();
            stack->Pop();
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
                stack->Pop(2);
            }
            else if (stack->Parent().compare("Position") == 0)
            {
                real32 value = std::stof(tokens[1]);
                if (tag.compare("x") == 0)
                    scene->m_camera.m_position[0] = value;
                else if (tag.compare("y") == 0)
                    scene->m_camera.m_position[1] = value;
                else if (tag.compare("z") == 0)
                    scene->m_camera.m_position[2] = value;
                stack->Pop();
            }
            else if (stack->Parent().compare("Front") == 0)
            {
                real32 value = std::stof(tokens[1]);
                if (tag.compare("x") == 0)
                    scene->m_camera.m_front[0] = value;
                else if (tag.compare("y") == 0)
                    scene->m_camera.m_front[1] = value;
                else if (tag.compare("z") == 0)
                    scene->m_camera.m_front[2] = value;
                stack->Pop();
            }
            else if (stack->Parent().compare("Up") == 0)
            {
                real32 value = std::stof(tokens[1]);
                if (tag.compare("x") == 0)
                    scene->m_camera.m_up[0] = value;
                else if (tag.compare("y") == 0)
                    scene->m_camera.m_up[1] = value;
                else if (tag.compare("z") == 0)
                    scene->m_camera.m_up[2] = value;
                stack->Pop();
            }
            else if (stack->Parent().compare("Right") == 0)
            {
                real32 value = std::stof(tokens[1]);
                if (tag.compare("x") == 0)
                    scene->m_camera.m_right[0] = value;
                else if (tag.compare("y") == 0)
                    scene->m_camera.m_right[1] = value;
                else if (tag.compare("z") == 0)
                    scene->m_camera.m_right[2] = value;
                stack->Pop();
            }
            else if (tag.compare("Yaw") == 0)
            {
                scene->m_camera.m_yaw = std::stof(tokens[1]);
                stack->Pop();
            }
            else if (tag.compare("Pitch") == 0)
            {
                scene->m_camera.m_pitch = std::stof(tokens[1]);
                stack->Pop();
            }
            else if (tag.compare("Fov") == 0)
            {
                scene->m_camera.m_fov = std::stof(tokens[1]);
                stack->Pop();
            }
            else if (tag.compare("MovementSpeed") == 0)
            {
                scene->m_camera.m_movementSpeed = std::stof(tokens[1]);
                stack->Pop();
            }
            else if (tag.compare("Sensitivity") == 0)
            {
                scene->m_camera.m_sensitivity = std::stof(tokens[1]);
                stack->Pop();
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
                stack->Pop(2);
            }
            else if (tag.compare("/Texture") == 0)
            {
                tempMaterial.m_textures.push_back(tempTexture);
                stack->Pop(2);
            }
            else if (tag.compare("/Material") == 0)
            {
                meshMaterials.push_back(tempMaterial);
                tempMaterial.m_textures.clear();
                stack->Pop(2);
            }
            else if (tag.compare("/Vertex") == 0)
            {
                tempVertices.push_back(tempVertex);
                stack->Pop(2);
            }
            else if (tag.compare("/Vertices") == 0)
            {
                meshVertices.push_back(tempVertices);
                tempVertices.clear();
                stack->Pop(2);
            }
            else if (tag.compare("/Indices") == 0)
            {
                meshIndices.push_back(tempIndices);
                tempIndices.clear();
                stack->Pop(2);
            }
            else if ((tag.compare("Name") == 0) && 
                     (stack->Parent().compare("Mesh") == 0))
            {
                meshNames.push_back(tokens[1]);
                stack->Pop();
            }
            else if (stack->Parent().compare("Position") == 0)
            {
                real32 value = std::stof(tokens[1]);
                if (tag.compare("x") == 0)
                    tempVertex.position.x = value;
                else if (tag.compare("y") == 0)
                    tempVertex.position.y = value;
                else if (tag.compare("z") == 0)
                    tempVertex.position.z = value;
                stack->Pop();
            }
            else if (stack->Parent().compare("Normal") == 0)
            {
                real32 value = std::stof(tokens[1]);
                if (tag.compare("x") == 0)
                    tempVertex.normal.x = value;
                else if (tag.compare("y") == 0)
                    tempVertex.normal.y = value;
                else if (tag.compare("z") == 0)
                    tempVertex.normal.z = value;
                stack->Pop();
            }
            else if (stack->Parent().compare("TextureUV") == 0)
            {
                real32 value = std::stof(tokens[1]);
                if (tag.compare("x") == 0)
                    tempVertex.textureUV.x = value;
                else if (tag.compare("y") == 0)
                    tempVertex.textureUV.y = value;
                stack->Pop();
            }
            else if (stack->Parent().compare("Indices") == 0)
            {
                tempIndices.push_back(std::stoi(tokens[1]));
                stack->Pop();
            }
            else if ((tag.compare("Name") == 0) && 
                     (stack->Parent().compare("Material") == 0))
            {
                tempMaterial.m_name = tokens[1];
                stack->Pop();
            }
            else if (stack->Parent().compare("Ambient") == 0)
            {
                real32 value = std::stof(tokens[1]);
                if (tag.compare("x") == 0)
                    tempMaterial.m_ambient.x = value;
                else if (tag.compare("y") == 0)
                    tempMaterial.m_ambient.y = value;
                else if (tag.compare("z") == 0)
                    tempMaterial.m_ambient.z = value;
                stack->Pop();
            }
            else if (stack->Parent().compare("Diffuse") == 0)
            {
                real32 value = std::stof(tokens[1]);
                if (tag.compare("x") == 0)
                    tempMaterial.m_diffuse.x = value;
                else if (tag.compare("y") == 0)
                    tempMaterial.m_diffuse.y = value;
                else if (tag.compare("z") == 0)
                    tempMaterial.m_diffuse.z = value;
                stack->Pop();
            }
            else if (stack->Parent().compare("Specular") == 0)
            {
                real32 value = std::stof(tokens[1]);
                if (tag.compare("x") == 0)
                    tempMaterial.m_specular.x = value;
                else if (tag.compare("y") == 0)
                    tempMaterial.m_specular.y = value;
                else if (tag.compare("z") == 0)
                    tempMaterial.m_specular.z = value;
                stack->Pop();
            }
            else if (stack->Parent().compare("Emissive") == 0)
            {
                real32 value = std::stof(tokens[1]);
                if (tag.compare("x") == 0)
                    tempMaterial.m_emissive.x = value;
                else if (tag.compare("y") == 0)
                    tempMaterial.m_emissive.y = value;
                else if (tag.compare("z") == 0)
                    tempMaterial.m_emissive.z = value;
                stack->Pop();
            }
            else if ((tag.compare("Shininess") == 0) && 
                     (stack->Parent().compare("Material") == 0))
            {
                tempMaterial.m_shininess = std::stof(tokens[1]);
                stack->Pop();
            }
            else if ((tag.compare("Type") == 0) && 
                     (stack->Parent().compare("Texture") == 0))
            {
                tempTexture.m_type = tokens[1];
                stack->Pop();
            }
            else if ((tag.compare("FilePath") == 0) && 
                     (stack->Parent().compare("Texture") == 0))
            {
                tempTexture.m_filePath = tokens[1];
                stack->Pop();
            }            
        }   // end of Mesh

        // Array check
        else if ((tag.compare("Array") == 0) && 
                 (stack->Parent().compare("Model") == 0))
            meshCount = std::stoi(tokens[2]);
        else if ((tag.compare("/Array") == 0) &&
                 (stack->Parent().compare("Model") == 0))
            stack->Pop();

        // Add Mesh to models
        else if (tag.compare("/Model") == 0)
        {
            std::vector<GraphicsEngine::Mesh> meshes;
            GraphicsEngine::Model model;
            for(int32 i = 0; i < meshCount; i++)
            {
                GraphicsEngine::Mesh mesh{meshNames[i], meshVertices[i], meshIndices[i], meshMaterials[i]};
                model.m_meshes.push_back(mesh);
            }
            scene->m_models.push_back(model);
            stack->Pop();

            // Reset
            meshCount = 0;
            meshNames.clear();
            meshVertices.clear();
            meshIndices.clear();
            meshMaterials.clear();
        }

    }   // end of fileStream 

    delete stack;
    scene->m_modelLoadState = GraphicsEngine::ModelLoadState::FILE_LOADED;
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
    fileOut << CURLY_BRACKET_START << '\n';

    /////////////////////////////////////
    // Camera
    /////////////////////////////////////

    GraphicsEngine::Camera* camera = &scene->m_camera;
    
    SerializeBeginObjectJSON(fileOut, 1, "Camera", CURLY_BRACKET_START);

    SerializeItemJSON(fileOut, 2, "Id", id++, true);

    SerializeBeginObjectJSON(fileOut, 2, "Position", CURLY_BRACKET_START);
    SerializeItemJSON(fileOut, 3, "x", camera->m_position.x, true);
    SerializeItemJSON(fileOut, 3, "y", camera->m_position.y, true);
    SerializeItemJSON(fileOut, 3, "z", camera->m_position.z, false);
    SerializeEndJSON(fileOut, 2, CURLY_BRACKET_END, true); // Position

    SerializeBeginObjectJSON(fileOut, 2, "Front", CURLY_BRACKET_START);
    SerializeItemJSON(fileOut, 3, "x", camera->m_front.x, true);
    SerializeItemJSON(fileOut, 3, "y", camera->m_front.y, true);
    SerializeItemJSON(fileOut, 3, "z", camera->m_front.z, false);
    SerializeEndJSON(fileOut, 2, CURLY_BRACKET_END, true); // Front

    SerializeBeginObjectJSON(fileOut, 2, "Up", CURLY_BRACKET_START);
    SerializeItemJSON(fileOut, 3, "x", camera->m_up.x, true);
    SerializeItemJSON(fileOut, 3, "y", camera->m_up.y, true);
    SerializeItemJSON(fileOut, 3, "z", camera->m_up.z, false);
    SerializeEndJSON(fileOut, 2, CURLY_BRACKET_END, true); // Up

    SerializeBeginObjectJSON(fileOut, 2, "Right", CURLY_BRACKET_START);
    SerializeItemJSON(fileOut, 3, "x", camera->m_right.x, true);
    SerializeItemJSON(fileOut, 3, "y", camera->m_right.y, true);
    SerializeItemJSON(fileOut, 3, "z", camera->m_right.z, false);
    SerializeEndJSON(fileOut, 2, CURLY_BRACKET_END, true); // Right

    SerializeItemJSON(fileOut, 2, "Yaw", camera->m_yaw, true);
    SerializeItemJSON(fileOut, 2, "Pitch", camera->m_pitch, true);
    SerializeItemJSON(fileOut, 2, "Fov", camera->m_fov, true);
    SerializeItemJSON(fileOut, 2, "MovementSpeed", camera->m_movementSpeed, true);
    SerializeItemJSON(fileOut, 2, "Sensitivity", camera->m_sensitivity, false);

    SerializeEndJSON(fileOut, 1, CURLY_BRACKET_END, true); // Camera

    /////////////////////////////////////
    // Models
    /////////////////////////////////////

    SerializeBeginObjectJSON(fileOut, 1, "Model", SQUARE_BRACKET_START);

    for (uint32 modelIndex = 0; modelIndex < scene->m_models.size(); modelIndex++)
    {
        SerializeBeginJSON(fileOut, 2, CURLY_BRACKET_START);
        SerializeItemJSON(fileOut, 3, "Id", id++, true);
        // TODO(neil): Add in model name to Serialize.
        SerializeItemJSON(fileOut, 3, "Size", static_cast<uint32>(scene->m_models[modelIndex].m_meshes.size()), true);

        SerializeBeginObjectJSON(fileOut, 3, "Mesh", SQUARE_BRACKET_START);
        for (uint32 meshIndex = 0; meshIndex < scene->m_models[modelIndex].m_meshes.size(); meshIndex++)
        {
            GraphicsEngine::Mesh *mesh = &scene->m_models[modelIndex].m_meshes[meshIndex];
            
            SerializeBeginJSON(fileOut, 4, CURLY_BRACKET_START);
            SerializeItemJSON(fileOut, 5, "Id", id++, true);
            SerializeItemJSON(fileOut, 5, "Name", mesh->m_name, true);

            SerializeBeginObjectJSON(fileOut, 5, "Vertices", SQUARE_BRACKET_START);
            for (int vIndex = 0; vIndex < mesh->m_vertices.size(); vIndex++)
            {
                SerializeBeginJSON(fileOut, 6, CURLY_BRACKET_START);

                SerializeBeginObjectJSON(fileOut, 7, "Position", CURLY_BRACKET_START);
                SerializeItemJSON(fileOut, 8, "x", mesh->m_vertices[vIndex].position.x, true);
                SerializeItemJSON(fileOut, 8, "y", mesh->m_vertices[vIndex].position.y, true);
                SerializeItemJSON(fileOut, 8, "z", mesh->m_vertices[vIndex].position.z, false);
                SerializeEndJSON(fileOut, 7, CURLY_BRACKET_END, true);

                SerializeBeginObjectJSON(fileOut, 7, "Normal", CURLY_BRACKET_START);
                SerializeItemJSON(fileOut, 8, "x", mesh->m_vertices[vIndex].normal.x, true);
                SerializeItemJSON(fileOut, 8, "y", mesh->m_vertices[vIndex].normal.y, true);
                SerializeItemJSON(fileOut, 8, "z", mesh->m_vertices[vIndex].normal.z, false);
                SerializeEndJSON(fileOut, 7, CURLY_BRACKET_END, true);

                SerializeBeginObjectJSON(fileOut, 7, "TextureUV", CURLY_BRACKET_START);
                SerializeItemJSON(fileOut, 8, "x", mesh->m_vertices[vIndex].textureUV.x, true);
                SerializeItemJSON(fileOut, 8, "y", mesh->m_vertices[vIndex].textureUV.y, false);
                SerializeEndJSON(fileOut, 7, CURLY_BRACKET_END, false);

                if (vIndex != mesh->m_vertices.size()-1)
                    SerializeEndJSON(fileOut, 6, CURLY_BRACKET_END, true);
                else
                    SerializeEndJSON(fileOut, 6, CURLY_BRACKET_END, false);
            }
            SerializeEndJSON(fileOut, 5, SQUARE_BRACKET_END, true); // Vertices

            // Indices
            SerializeBeginObjectJSON(fileOut, 5, "Indices", SQUARE_BRACKET_START);
            for (uint32 indicesIndex = 0;
                 indicesIndex < mesh->m_indices.size()-1;
                 indicesIndex++)
            {
                SerializeItemNoKeyJSON(fileOut, 6, mesh->m_indices[indicesIndex], true);
            }
            // Last index
            SerializeItemNoKeyJSON(fileOut, 6, mesh->m_indices[mesh->m_indices.size()-1], false);
            SerializeEndJSON(fileOut, 5, SQUARE_BRACKET_END, true); // indices

            // Material
            SerializeBeginObjectJSON(fileOut, 5, "Material", CURLY_BRACKET_START);
            SerializeItemJSON(fileOut, 6, "Name", mesh->m_material.m_name, true);

            SerializeBeginObjectJSON(fileOut, 6, "Ambient", CURLY_BRACKET_START);
            SerializeItemJSON(fileOut, 7, "x", mesh->m_material.m_ambient.x, true);
            SerializeItemJSON(fileOut, 7, "y", mesh->m_material.m_ambient.y, true);
            SerializeItemJSON(fileOut, 7, "z", mesh->m_material.m_ambient.z, false);
            SerializeEndJSON(fileOut, 6, CURLY_BRACKET_END, true); // Ambient

            SerializeBeginObjectJSON(fileOut, 6, "Diffuse", CURLY_BRACKET_START);
            SerializeItemJSON(fileOut, 7, "x", mesh->m_material.m_diffuse.x, true);
            SerializeItemJSON(fileOut, 7, "y", mesh->m_material.m_diffuse.y, true);
            SerializeItemJSON(fileOut, 7, "z", mesh->m_material.m_diffuse.z, false);
            SerializeEndJSON(fileOut, 6, CURLY_BRACKET_END, true); // Diffuse

            SerializeBeginObjectJSON(fileOut, 6, "Specular", CURLY_BRACKET_START);
            SerializeItemJSON(fileOut, 7, "x", mesh->m_material.m_specular.x, true);
            SerializeItemJSON(fileOut, 7, "y", mesh->m_material.m_specular.y, true);
            SerializeItemJSON(fileOut, 7, "z", mesh->m_material.m_specular.z, false);
            SerializeEndJSON(fileOut, 6, CURLY_BRACKET_END, true); // Specular

            SerializeBeginObjectJSON(fileOut, 6, "Emissive", CURLY_BRACKET_START);
            SerializeItemJSON(fileOut, 7, "x", mesh->m_material.m_emissive.x, true);
            SerializeItemJSON(fileOut, 7, "y", mesh->m_material.m_emissive.y, true);
            SerializeItemJSON(fileOut, 7, "z", mesh->m_material.m_emissive.z, false);
            SerializeEndJSON(fileOut, 6, CURLY_BRACKET_END, true); // Emissive

            SerializeItemJSON(fileOut, 6, "Shininess", mesh->m_material.m_shininess, true);

            SerializeBeginObjectJSON(fileOut, 6, "Textures", SQUARE_BRACKET_START);
            for (uint32 textureIndex = 0;
                 textureIndex < mesh->m_material.m_textures.size()-1;
                 textureIndex++)
            {
                SerializeBeginJSON(fileOut, 7, CURLY_BRACKET_START);
                SerializeItemJSON(fileOut, 8, "Type", mesh->m_material.m_textures[textureIndex].m_type, true);
                SerializeItemJSON(fileOut, 8, "FilePath", mesh->m_material.m_textures[textureIndex].m_filePath, false);
                SerializeEndJSON(fileOut, 7, CURLY_BRACKET_END, true);
            }
            // Last index
            SerializeBeginJSON(fileOut, 7, CURLY_BRACKET_START);
            SerializeItemJSON(fileOut, 8, "Type", mesh->m_material.m_textures[mesh->m_material.m_textures.size()-1].m_type, true);
            SerializeItemJSON(fileOut, 8, "FilePath", mesh->m_material.m_textures[mesh->m_material.m_textures.size()-1].m_filePath, false);
            SerializeEndJSON(fileOut, 7, CURLY_BRACKET_END, false);

            SerializeEndJSON(fileOut, 6, SQUARE_BRACKET_END, false); // textures
            SerializeEndJSON(fileOut, 5, CURLY_BRACKET_END, false); // material

            if (meshIndex != scene->m_models[modelIndex].m_meshes.size()-1)
                SerializeEndJSON(fileOut, 4, CURLY_BRACKET_END, true); // mesh
            else
                SerializeEndJSON(fileOut, 4, CURLY_BRACKET_END, false); // mesh
        }
        SerializeEndJSON(fileOut, 3, SQUARE_BRACKET_END, false); // model
        if (modelIndex != scene->m_models.size()-1)
            SerializeEndJSON(fileOut, 2, CURLY_BRACKET_END, true); // model
        else
            SerializeEndJSON(fileOut, 2, CURLY_BRACKET_END, false); // model
    }
    SerializeEndJSON(fileOut, 1, SQUARE_BRACKET_END, false); // model

    // Closing
    fileOut << CURLY_BRACKET_END << '\n';

    return true;
}

void SerializeBeginJSON(std::fstream& output, int32 indents, char bracket)
{
    for (int32 i = 0; i < indents; ++i)
        output << "\t";

    output << bracket;
    output << '\n';
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
    output << '\n';
}

void SerializeEndJSON(std::fstream& output, int32 indents, char bracket, bool comma)
{
    for (int32 i = 0; i < indents; ++i)
        output << "\t";

    output << bracket;
    if (comma)
        output << ",";
    output << '\n';
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
    output << '\n';
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
    output << '\n';
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
    output << '\n';
}

void SerializeItemNoKeyJSON(std::fstream& output, int32 indents, const uint32 value, bool comma)
{
    for (int32 i = 0; i < indents; ++i)
        output << "\t";

    output << value;
    if (comma)
        output << ",";
    output << '\n';
}

bool DeSerializeFromJSON(const std::string& filePath, GraphicsEngine::Scene* scene)
{
    std::ifstream fileStream(filePath, std::ios::in);
    if(!fileStream.is_open())
    {
        std::string errorMessage = "ERROR\t\tFileStream\t\tCould not read file path: " 
                                   + std::string(filePath) + ".\n";
        OutputDebugStringA(errorMessage.c_str());
        return false;
    }

    Stack* stack = new Stack();

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

    if (scene->m_models.size() > 0)
        scene->m_models.clear();

    // Parse each line and de-serialize data
    while(!fileStream.eof())
    {
        std::string line = "";
        std::getline(fileStream, line);

        // Parse line into tokens
        std::string token = "";
        std::vector<std::string> tokens;
        char p = '\0';
        for (char c : line)
        {
            if (c != '\t')
            {
                if (c != CURLY_BRACKET_START
                    && c != SQUARE_BRACKET_START
                    && (c != ':' || p != '"') // filepath
                    && c != '"'
                    && c != ' '
                    && c != ','
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
            p = c;
        }
        // Check last token (no comma as end of line)
        if (!token.empty())
        {
            tokens.push_back(token);
            token.clear();
        }

        ///////////////////////////////////////////
        // Go through tokens and de-serialize them
        ///////////////////////////////////////////

        // Key
        std::string key;
        if (!tokens.empty())
        {
            key = tokens[0];
            stack->Push(key);
        }
        else
            continue;
        
        // Mark current scope
        if (key.compare("Camera") == 0)
            currentScope = "Camera";
        else if (key.compare("Mesh") == 0)
            currentScope = "Mesh";
        // Unmark current scope
        else if ((stack->Parent().compare("Camera") == 0) &&
                (key.compare("}") == 0)
                )
        {
            currentScope.clear();
            stack->Pop(2);
        }
        else if ((stack->Parent().compare("Mesh") == 0) &&
                (key.compare("}") == 0)
                )
        {
            stack->Pop();
        }
        else if ((stack->Parent().compare("Mesh") == 0) &&
                (key.compare("]") == 0)
                )
        {
            currentScope.clear();
            stack->Pop(2);
        }


        else if (key.compare("Id") == 0)
            stack->Pop();
        // TODO(neil): Add in model name to Deserialize.
        
        else if ((key.compare("Size") == 0) && 
                 (stack->Parent().compare("Model") == 0))
        {
            meshCount = std::stoi(tokens[1]);
            stack->Pop();
        }

        // Assign values to Camera
        else if (currentScope.compare("Camera") == 0)
        {
            if ((key.compare("}") == 0) ||
                (key.compare("]") == 0)
                )
            {
                stack->Pop(2);
            }
            else if (stack->Parent().compare("Position") == 0)
            {
                real32 value = std::stof(tokens[1]);
                if (key.compare("x") == 0)
                    scene->m_camera.m_position[0] = value;
                else if (key.compare("y") == 0)
                    scene->m_camera.m_position[1] = value;
                else if (key.compare("z") == 0)
                    scene->m_camera.m_position[2] = value;
                stack->Pop();
            }
            else if (stack->Parent().compare("Front") == 0)
            {
                real32 value = std::stof(tokens[1]);
                if (key.compare("x") == 0)
                    scene->m_camera.m_front[0] = value;
                else if (key.compare("y") == 0)
                    scene->m_camera.m_front[1] = value;
                else if (key.compare("z") == 0)
                    scene->m_camera.m_front[2] = value;
                stack->Pop();
            }
            else if (stack->Parent().compare("Up") == 0)
            {
                real32 value = std::stof(tokens[1]);
                if (key.compare("x") == 0)
                    scene->m_camera.m_up[0] = value;
                else if (key.compare("y") == 0)
                    scene->m_camera.m_up[1] = value;
                else if (key.compare("z") == 0)
                    scene->m_camera.m_up[2] = value;
                stack->Pop();
            }
            else if (stack->Parent().compare("Right") == 0)
            {
                real32 value = std::stof(tokens[1]);
                if (key.compare("x") == 0)
                    scene->m_camera.m_right[0] = value;
                else if (key.compare("y") == 0)
                    scene->m_camera.m_right[1] = value;
                else if (key.compare("z") == 0)
                    scene->m_camera.m_right[2] = value;
                stack->Pop();
            }
            else if (key.compare("Yaw") == 0)
            {
                scene->m_camera.m_yaw = std::stof(tokens[1]);
                stack->Pop();
            }
            else if (key.compare("Pitch") == 0)
            {
                scene->m_camera.m_pitch = std::stof(tokens[1]);
                stack->Pop();
            }
            else if (key.compare("Fov") == 0)
            {
                scene->m_camera.m_fov = std::stof(tokens[1]);
                stack->Pop();
            }
            else if (key.compare("MovementSpeed") == 0)
            {
                scene->m_camera.m_movementSpeed = std::stof(tokens[1]);
                stack->Pop();
            }
            else if (key.compare("Sensitivity") == 0)
            {
                scene->m_camera.m_sensitivity = std::stof(tokens[1]);
                stack->Pop();
            }            
        }   // end of Camera
        
        // Assign values to Mesh
        else if (currentScope.compare("Mesh") == 0)
        {
            if ((stack->Parent().compare("Position") == 0 && key.compare("}") == 0) ||
                (stack->Parent().compare("Normal") == 0 && key.compare("}") == 0) ||
                (stack->Parent().compare("TextureUV") == 0 && key.compare("}") == 0) ||
                (stack->Parent().compare("Ambient") == 0 && key.compare("}") == 0) ||
                (stack->Parent().compare("Diffuse") == 0 && key.compare("}") == 0) ||
                (stack->Parent().compare("Specular") == 0 && key.compare("}") == 0) ||
                (stack->Parent().compare("Emissive") == 0 && key.compare("}") == 0)
                )
            {
                stack->Pop(2);
            }
            else if ((stack->Parent().compare("Textures") == 0) &&
                     (key.compare("}") == 0)
                )
            {
                tempMaterial.m_textures.push_back(tempTexture);
                stack->Pop();
            }
            else if ((stack->Parent().compare("Textures") == 0) &&
                     (key.compare("]") == 0)
                    )
            {
                stack->Pop(2);
            }
            else if ((stack->Parent().compare("Material") == 0) &&
                     (key.compare("}") == 0)
                    )
            {
                meshMaterials.push_back(tempMaterial);
                tempMaterial.m_textures.clear();
                stack->Pop(2);
            }
            else if ((stack->Parent().compare("Vertices") == 0) &&
                     (key.compare("}") == 0)
                    )
            {
                tempVertices.push_back(tempVertex);
                stack->Pop();
            }
            else if ((stack->Parent().compare("Vertices") == 0) &&
                     (key.compare("]") == 0)
                    )
            {
                meshVertices.push_back(tempVertices);
                tempVertices.clear();
                stack->Pop(2);
            }
            else if ((stack->Parent().compare("Indices") == 0) &&
                     (key.compare("]") == 0)
                    )
            {
                meshIndices.push_back(tempIndices);
                tempIndices.clear();
                stack->Pop(2);
            }
            else if ((key.compare("Name") == 0) && 
                     (stack->Parent().compare("Mesh") == 0))
            {
                meshNames.push_back(tokens[1]);
                stack->Pop();
            }
            else if (stack->Parent().compare("Position") == 0)
            {
                real32 value = std::stof(tokens[1]);
                if (key.compare("x") == 0)
                    tempVertex.position.x = value;
                else if (key.compare("y") == 0)
                    tempVertex.position.y = value;
                else if (key.compare("z") == 0)
                    tempVertex.position.z = value;
                stack->Pop();
            }
            else if (stack->Parent().compare("Normal") == 0)
            {
                real32 value = std::stof(tokens[1]);
                if (key.compare("x") == 0)
                    tempVertex.normal.x = value;
                else if (key.compare("y") == 0)
                    tempVertex.normal.y = value;
                else if (key.compare("z") == 0)
                    tempVertex.normal.z = value;
                stack->Pop();
            }
            else if (stack->Parent().compare("TextureUV") == 0)
            {
                real32 value = std::stof(tokens[1]);
                if (key.compare("x") == 0)
                    tempVertex.textureUV.x = value;
                else if (key.compare("y") == 0)
                    tempVertex.textureUV.y = value;
                stack->Pop();
            }
            else if (stack->Parent().compare("Indices") == 0)
            {
                tempIndices.push_back(std::stoi(tokens[0]));
                stack->Pop();
            }
            else if ((key.compare("Name") == 0) && 
                     (stack->Parent().compare("Material") == 0))
            {
                tempMaterial.m_name = tokens[1];
                stack->Pop();
            }
            else if (stack->Parent().compare("Ambient") == 0)
            {
                real32 value = std::stof(tokens[1]);
                if (key.compare("x") == 0)
                    tempMaterial.m_ambient.x = value;
                else if (key.compare("y") == 0)
                    tempMaterial.m_ambient.y = value;
                else if (key.compare("z") == 0)
                    tempMaterial.m_ambient.z = value;
                stack->Pop();
            }
            else if (stack->Parent().compare("Diffuse") == 0)
            {
                real32 value = std::stof(tokens[1]);
                if (key.compare("x") == 0)
                    tempMaterial.m_diffuse.x = value;
                else if (key.compare("y") == 0)
                    tempMaterial.m_diffuse.y = value;
                else if (key.compare("z") == 0)
                    tempMaterial.m_diffuse.z = value;
                stack->Pop();
            }
            else if (stack->Parent().compare("Specular") == 0)
            {
                real32 value = std::stof(tokens[1]);
                if (key.compare("x") == 0)
                    tempMaterial.m_specular.x = value;
                else if (key.compare("y") == 0)
                    tempMaterial.m_specular.y = value;
                else if (key.compare("z") == 0)
                    tempMaterial.m_specular.z = value;
                stack->Pop();
            }
            else if (stack->Parent().compare("Emissive") == 0)
            {
                real32 value = std::stof(tokens[1]);
                if (key.compare("x") == 0)
                    tempMaterial.m_emissive.x = value;
                else if (key.compare("y") == 0)
                    tempMaterial.m_emissive.y = value;
                else if (key.compare("z") == 0)
                    tempMaterial.m_emissive.z = value;
                stack->Pop();
            }
            else if ((key.compare("Shininess") == 0) && 
                     (stack->Parent().compare("Material") == 0))
            {
                tempMaterial.m_shininess = std::stof(tokens[1]);
                stack->Pop();
            }
            else if ((key.compare("Type") == 0) && 
                     (stack->Parent().compare("Textures") == 0))
            {
                tempTexture.m_type = tokens[1];
                stack->Pop();
            }
            else if ((key.compare("FilePath") == 0) && 
                     (stack->Parent().compare("Textures") == 0))
            {
                tempTexture.m_filePath = tokens[1];
                stack->Pop();
            }            
        }   // end of Mesh

        // Add Mesh to models
        else if ((stack->Parent().compare("Model") == 0) &&
                 (key.compare("}") == 0)
                 )
        {
            std::vector<GraphicsEngine::Mesh> meshes;
            GraphicsEngine::Model model;
            for(int32 i = 0; i < meshCount; i++)
            {
                GraphicsEngine::Mesh mesh{meshNames[i], meshVertices[i], meshIndices[i], meshMaterials[i]};
                model.m_meshes.push_back(mesh);
            }
            scene->m_models.push_back(model);
            stack->Pop();

            // Reset
            meshCount = 0;
            meshNames.clear();
            meshVertices.clear();
            meshIndices.clear();
            meshMaterials.clear();
        }

    } // end of fileStream
    
    delete stack;
    scene->m_modelLoadState = GraphicsEngine::ModelLoadState::FILE_LOADED;
    fileStream.close();
    return true;
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

    fileOut << "# Bananas Import/Export v1.0" << '\n';

    /////////////////////////////////////
    // Camera
    /////////////////////////////////////

    SerializeBeginYAML(fileOut, 0, "Camera");
    SerializeItemYAML(fileOut, 1, "Id", id++, YAML_DASH_START);

    SerializeVector3DYAML(fileOut, 1, "Position", scene->m_camera.m_position.x, 
                                                  scene->m_camera.m_position.y, 
                                                  scene->m_camera.m_position.z, 
                                                  YAML_DASH_START);
    SerializeVector3DYAML(fileOut, 1, "Front", scene->m_camera.m_front.x, 
                                               scene->m_camera.m_front.y, 
                                               scene->m_camera.m_front.z, 
                                               YAML_DASH_START);
    SerializeVector3DYAML(fileOut, 1, "Up", scene->m_camera.m_up.x, 
                                            scene->m_camera.m_up.y, 
                                            scene->m_camera.m_up.z, 
                                            YAML_DASH_START);
    SerializeVector3DYAML(fileOut, 1, "Right", scene->m_camera.m_right.x, 
                                               scene->m_camera.m_right.y, 
                                               scene->m_camera.m_right.z, 
                                               YAML_DASH_START);

    SerializeItemYAML(fileOut, 1, "Yaw", scene->m_camera.m_yaw, YAML_DASH_START);
    SerializeItemYAML(fileOut, 1, "Pitch", scene->m_camera.m_pitch, YAML_DASH_START);
    SerializeItemYAML(fileOut, 1, "Fov", scene->m_camera.m_fov, YAML_DASH_START);
    SerializeItemYAML(fileOut, 1, "MovementSpeed", scene->m_camera.m_movementSpeed, YAML_DASH_START);
    SerializeItemYAML(fileOut, 1, "Sensitivity", scene->m_camera.m_sensitivity, YAML_DASH_START);

    /////////////////////////////////////
    // Models
    /////////////////////////////////////


    for (uint32 modelIndex = 0; modelIndex < scene->m_models.size(); modelIndex++)
    {
        SerializeBeginYAML(fileOut, 0, "Model");
        SerializeItemYAML(fileOut, 1, "Id", id++, YAML_DASH_START);
        SerializeItemYAML(fileOut, 1, "Name", scene->m_models[modelIndex].m_name, YAML_DASH_START);
        SerializeItemYAML(fileOut, 1, "Size", static_cast<uint32>(scene->m_models[modelIndex].m_meshes.size()), YAML_DASH_START);

        for (uint32 meshIndex = 0; meshIndex < scene->m_models[modelIndex].m_meshes.size(); meshIndex++)
        {
            GraphicsEngine::Mesh *mesh = &scene->m_models[modelIndex].m_meshes[meshIndex];

            SerializeBeginYAML(fileOut, 1, "Mesh");
            SerializeItemYAML(fileOut, 2, "Id", id++, YAML_DASH_START);
            SerializeItemYAML(fileOut, 2, "Name", mesh->m_name, YAML_DASH_START);
            
            SerializeBeginYAML(fileOut, 2, "Vertices");
            for (const auto& vertex : mesh->m_vertices)
            {
                SerializeVector3DYAML(fileOut, 3, "Position", vertex.position.x, vertex.position.y, vertex.position.z, YAML_DASH_START);
                SerializeVector3DYAML(fileOut, 3, "Normal", vertex.normal.x, vertex.normal.y, vertex.normal.z, YAML_SPACE_START);
                SerializeVector2DYAML(fileOut, 3, "TextureUV", vertex.textureUV.x, vertex.textureUV.y, YAML_SPACE_START);
            }

            // Indices
            fileOut << "    " << "- Indices: [";
            for (uint32 indicesIndex = 0;
                 indicesIndex < mesh->m_indices.size()-1;
                 indicesIndex++)
            {
                fileOut << mesh->m_indices[indicesIndex] << ", ";
            }
            fileOut << mesh->m_indices[mesh->m_indices.size()-1];
            fileOut << "]" << '\n';

            SerializeBeginYAML(fileOut, 2, "Material");
            SerializeItemYAML(fileOut, 3, "Name", mesh->m_material.m_name, YAML_DASH_START);
            SerializeVector3DYAML(fileOut, 3, "Ambient", mesh->m_material.m_ambient.x, 
                                                         mesh->m_material.m_ambient.y, 
                                                         mesh->m_material.m_ambient.z, 
                                                         YAML_DASH_START);
            SerializeVector3DYAML(fileOut, 3, "Diffuse", mesh->m_material.m_diffuse.x, 
                                                         mesh->m_material.m_diffuse.y, 
                                                         mesh->m_material.m_diffuse.z, 
                                                         YAML_DASH_START);
            SerializeVector3DYAML(fileOut, 3, "Specular", mesh->m_material.m_specular.x, 
                                                          mesh->m_material.m_specular.y, 
                                                          mesh->m_material.m_specular.z, 
                                                          YAML_DASH_START);
            SerializeVector3DYAML(fileOut, 3, "Emissive", mesh->m_material.m_emissive.x, 
                                                          mesh->m_material.m_emissive.y, 
                                                          mesh->m_material.m_emissive.z, 
                                                          YAML_DASH_START);
            SerializeItemYAML(fileOut, 3, "Shininess", mesh->m_material.m_shininess, YAML_DASH_START);

            SerializeBeginYAML(fileOut, 3, "Textures");
            for (uint32 textureIndex = 0;
                 textureIndex < mesh->m_material.m_textures.size();
                 textureIndex++)
            {
                SerializeItemYAML(fileOut, 4, "Type", mesh->m_material.m_textures[textureIndex].m_type, YAML_DASH_START);
                SerializeItemYAML(fileOut, 4, "FilePath", mesh->m_material.m_textures[textureIndex].m_filePath, YAML_SPACE_START);
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
    output << '\n';
}

void SerializeItemYAML(std::fstream& output, int32 indents, const std::string& key, const std::string& value, const char* start)
{
    for (int32 i = 0; i < indents * TAB_SIZE; ++i)
        output << " ";

    output << start;
    output << key;
    output << ": ";
    output << value;
    output << '\n';
}

void SerializeItemYAML(std::fstream& output, int32 indents, const std::string& key, uint32 value, const char* start)
{
    for (int32 i = 0; i < indents * TAB_SIZE; ++i)
        output << " ";

    output << start;
    output << key;
    output << ": ";
    output << value;
    output << '\n';
}

void SerializeItemYAML(std::fstream& output, int32 indents, const std::string& key, real32 value, const char* start)
{
    for (int32 i = 0; i < indents * TAB_SIZE; ++i)
        output << " ";

    output << start;
    output << key;
    output << ": ";
    output << value;
    output << '\n';
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
    
    output << '\n';
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
    
    output << '\n';
}

bool DeSerializeFromYAML(const std::string& filePath, GraphicsEngine::Scene* scene)
{
    std::ifstream fileStream(filePath, std::ios::in);
    if(!fileStream.is_open())
    {
        std::string errorMessage = "ERROR\t\tFileStream\t\tCould not read file path: " 
                                   + std::string(filePath) + ".\n";
        OutputDebugStringA(errorMessage.c_str());
        return false;
    }

    Stack* stack = new Stack();

    GraphicsEngine::Vertex tempVertex;
    GraphicsEngine::Material tempMaterial;
    GraphicsEngine::Texture tempTexture;

    std::vector<GraphicsEngine::Vertex> tempVertices;
    std::vector<uint32> tempIndices;

    std::vector<std::string> meshNames;
    std::vector<std::string> modelNames;
    std::vector<std::vector<GraphicsEngine::Vertex>> meshVertices;
    std::vector<std::vector<uint32>> meshIndices;
    std::vector<GraphicsEngine::Material> meshMaterials;

    int32 meshCount = 0;
    int32 modelIndex = 0;

    std::string currentScope = "";

    if (scene->m_models.size() > 0)
        scene->m_models.clear();

    // Parse each line and de-serialize data
    while(!fileStream.eof())
    {
        std::string line = "";
        std::getline(fileStream, line);

        // commented line, skip
        if (line[0] == '#')
            continue;

        // Parse line into tokens
        std::string token = "";
        std::vector<std::string> tokens;
        char p = '\0';
        for (char c : line)
        {
            if (c != '\t')
            {
                if (c != CURLY_BRACKET_START
                    && c != CURLY_BRACKET_END
                    && c != SQUARE_BRACKET_START
                    && c != SQUARE_BRACKET_END
                    && (c != ':' || (p >= 'A' && p <= 'Z')) // filepath
                    && c != '"'
                    && c != ' '
                    && c != ','
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
            p = c;
        }
        // Last line of file is empty
        if (line.empty())
        {
            // Special case where Mesh ended and nothing is left in file
            if (!tempMaterial.m_textures.empty())
            {
                meshMaterials.push_back(tempMaterial);
                tempMaterial.m_textures.clear();
                stack->Pop(4);
            }
            continue;
        }
        // Last token
        if (!token.empty())
        {
            tokens.push_back(token);
            token.clear();
        }
        if (tokens[0].compare("-") == 0)
        {
            tokens.erase(tokens.begin());
        }
    
        ///////////////////////////////////////////
        // Go through tokens and de-serialize them
        ///////////////////////////////////////////

        // Key
        std::string key;
        // End of Vertices
        if (!tokens.empty() && tokens[0].compare("Indices") == 0)
        {
            stack->Pop();
        }
        // End of Previous Mesh
        else if (!tokens.empty() && tokens[0].compare("Mesh") == 0 && !tempMaterial.m_textures.empty())
        {
            meshMaterials.push_back(tempMaterial);
            tempMaterial.m_textures.clear();
            stack->Pop(3);
        }
        // End of Previous Model
        else if (!tokens.empty() && tokens[0].compare("Model") == 0 && !tempMaterial.m_textures.empty())
        {
            meshMaterials.push_back(tempMaterial);
            tempMaterial.m_textures.clear();
            stack->Pop(4);
        }
        // Push key on stack
        if (!tokens.empty())
        {
            key = tokens[0];
            stack->Push(key);
        }
        else
            continue;

        // Mark current scope
        if (key.compare("Camera") == 0)
            currentScope = "Camera";
        else if (key.compare("Mesh") == 0)
            currentScope = "Mesh";
        else if (key.compare("Model") == 0)
            currentScope = "Model";
        
        if (key.compare("Id") == 0)
            stack->Pop();
        else if ((key.compare("Name") == 0) && 
                 (stack->Parent().compare("Model") == 0))
        {
            modelNames.push_back(tokens[1]);
            stack->Pop();
        }
        else if ((key.compare("Size") == 0) && 
                 (stack->Parent().compare("Model") == 0))
        {
            meshCount = std::stoi(tokens[1]);
            stack->Pop();
        }

        // Assign values to Camera
        if (currentScope.compare("Camera") == 0)
        {
            if (key.compare("Position") == 0)
            {
                scene->m_camera.m_position[0] = std::stof(tokens[2]);
                scene->m_camera.m_position[1] = std::stof(tokens[4]);
                scene->m_camera.m_position[2] = std::stof(tokens[6]);
                stack->Pop();
            }
            else if (key.compare("Front") == 0)
            {
                scene->m_camera.m_front[0] = std::stof(tokens[2]);
                scene->m_camera.m_front[1] = std::stof(tokens[4]);
                scene->m_camera.m_front[2] = std::stof(tokens[6]);
                stack->Pop();
            }
            else if (key.compare("Up") == 0)
            {
                scene->m_camera.m_up[0] = std::stof(tokens[2]);
                scene->m_camera.m_up[1] = std::stof(tokens[4]);
                scene->m_camera.m_up[2] = std::stof(tokens[6]);
                stack->Pop();
            }
            else if (key.compare("Right") == 0)
            {
                scene->m_camera.m_right[0] = std::stof(tokens[2]);
                scene->m_camera.m_right[1] = std::stof(tokens[4]);
                scene->m_camera.m_right[2] = std::stof(tokens[6]);
                stack->Pop();
            }
            else if (key.compare("Yaw") == 0)
            {
                scene->m_camera.m_yaw = std::stof(tokens[1]);
                stack->Pop();
            }
            else if (key.compare("Pitch") == 0)
            {
                scene->m_camera.m_pitch = std::stof(tokens[1]);
                stack->Pop();
            }
            else if (key.compare("Fov") == 0)
            {
                scene->m_camera.m_fov = std::stof(tokens[1]);
                stack->Pop();
            }
            else if (key.compare("MovementSpeed") == 0)
            {
                scene->m_camera.m_movementSpeed = std::stof(tokens[1]);
                stack->Pop();
            }
            else if (key.compare("Sensitivity") == 0)
            {
                scene->m_camera.m_sensitivity = std::stof(tokens[1]);
                stack->Pop();

                // Last parameter
                stack->Pop();
            }            
        }   // end of Camera

        // Assign values to Mesh
        else if (currentScope.compare("Mesh") == 0)
        {
            if ((key.compare("Name") == 0) &&
                (stack->Parent().compare("Mesh") == 0))
            {
                meshNames.push_back(tokens[1]);
                stack->Pop();
            }
            else if (key.compare("Position") == 0)
            {
                tempVertex.position.x = std::stof(tokens[2]);
                tempVertex.position.y = std::stof(tokens[4]);
                tempVertex.position.z = std::stof(tokens[6]);
                stack->Pop();
            }
            else if (key.compare("Normal") == 0)
            {
                tempVertex.normal.x = std::stof(tokens[2]);
                tempVertex.normal.y = std::stof(tokens[4]);
                tempVertex.normal.z = std::stof(tokens[6]);
                stack->Pop();
            }
            // Extra ":" due to logic of splitting string
            else if (key.compare("TextureUV:") == 0)
            {
                tempVertex.textureUV.x = std::stof(tokens[2]);
                tempVertex.textureUV.y = std::stof(tokens[4]);
                stack->Pop();

                // End of Vertex
                tempVertices.push_back(tempVertex);
            }
            else if (key.compare("Indices") == 0)
            {
                // End of Vertices
                meshVertices.push_back(tempVertices);
                tempVertices.clear();

                for (int i = 1; i < tokens.size(); i++)
                {
                    tempIndices.push_back(std::stoi(tokens[i]));
                }
                meshIndices.push_back(tempIndices);
                tempIndices.clear();
                stack->Pop();
            }
            else if ((key.compare("Name") == 0) && 
                     (stack->Parent().compare("Material") == 0))
            {
                tempMaterial.m_name = tokens[1];
                stack->Pop();
            }
            else if (key.compare("Ambient") == 0)
            {
                tempMaterial.m_ambient.x = std::stof(tokens[2]);
                tempMaterial.m_ambient.y = std::stof(tokens[4]);
                tempMaterial.m_ambient.z = std::stof(tokens[6]);
                stack->Pop();
            }
            else if (key.compare("Diffuse") == 0)
            {
                tempMaterial.m_diffuse.x = std::stof(tokens[2]);
                tempMaterial.m_diffuse.y = std::stof(tokens[4]);
                tempMaterial.m_diffuse.z = std::stof(tokens[6]);
                stack->Pop();
            }
            else if (key.compare("Specular") == 0)
            {
                tempMaterial.m_specular.x = std::stof(tokens[2]);
                tempMaterial.m_specular.y = std::stof(tokens[4]);
                tempMaterial.m_specular.z = std::stof(tokens[6]);
                stack->Pop();
            }
            else if (key.compare("Emissive") == 0)
            {
                tempMaterial.m_emissive.x = std::stof(tokens[2]);
                tempMaterial.m_emissive.y = std::stof(tokens[4]);
                tempMaterial.m_emissive.z = std::stof(tokens[6]);
                stack->Pop();
            }
            else if ((key.compare("Shininess") == 0) && 
                     (stack->Parent().compare("Material") == 0))
            {
                tempMaterial.m_shininess = std::stof(tokens[1]);
                stack->Pop();
            }
            else if ((key.compare("Type") == 0) && 
                     (stack->Parent().compare("Textures") == 0))
            {
                tempTexture.m_type = tokens[1];
                stack->Pop();
            }
            else if ((key.compare("FilePath") == 0) && 
                     (stack->Parent().compare("Textures") == 0))
            {
                tempTexture.m_filePath = tokens[1];
                stack->Pop();

                // End of Texture
                tempMaterial.m_textures.push_back(tempTexture);
            }
        }

        // Add Mesh to models
        else if ((key.compare("Model") == 0) &&
                 (meshCount > 0)
                 )
        {
            std::vector<GraphicsEngine::Mesh> meshes;
            GraphicsEngine::Model model;
            // TODO(neil): replace with actual error check later on
            ASSERT(modelIndex <= modelNames.size());
            model.m_name = modelNames[modelIndex++];
            for(int32 i = 0; i < meshCount; i++)
            {
                GraphicsEngine::Mesh mesh{meshNames[i], meshVertices[i], meshIndices[i], meshMaterials[i]};
                model.m_meshes.push_back(mesh);
            }
            scene->m_models.push_back(model);

            // Reset
            meshCount = 0;
            meshNames.clear();
            meshVertices.clear();
            meshIndices.clear();
            meshMaterials.clear();
        }
    }

    // Cleanup at end of file
    if (meshCount > 0)
    {
        std::vector<GraphicsEngine::Mesh> meshes;
        GraphicsEngine::Model model;
        // TODO(neil): replace with actual error check later on
        ASSERT(modelIndex <= modelNames.size());
        model.m_name = modelNames[modelIndex];
        for (int32 i = 0; i < meshCount; i++)
        {
            GraphicsEngine::Mesh mesh{ meshNames[i], meshVertices[i], meshIndices[i], meshMaterials[i] };
            model.m_meshes.push_back(mesh);
        }
        scene->m_models.push_back(model);
    }

    delete stack;
    scene->m_modelLoadState = GraphicsEngine::ModelLoadState::FILE_LOADED;
    fileStream.close();
    return true;
}


}
