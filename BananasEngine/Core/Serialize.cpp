#include "Serialize.h"

#include "Stack.h"

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
    OutputItemXML(fileOut, 4, camera->m_Name, camera->m_Position.x, "position.x");
    OutputItemXML(fileOut, 4, camera->m_Name, camera->m_Position.y, "position.y");
    OutputItemXML(fileOut, 4, camera->m_Name, camera->m_Position.z, "position.z");
    fileOut << "\t" << "\t" << "\t" << "</" << camera->m_Name << "." << "position" << ">" << std::endl;

    // Camera.Front
    fileOut << "\t" << "\t" << "\t" << "<" << camera->m_Name << "." << "front" << ">" << std::endl;
    OutputItemXML(fileOut, 4, camera->m_Name, camera->m_Front.x, "front.x");
    OutputItemXML(fileOut, 4, camera->m_Name, camera->m_Front.y, "front.y");
    OutputItemXML(fileOut, 4, camera->m_Name, camera->m_Front.z, "front.z");
    fileOut << "\t" << "\t" << "\t" << "</" << camera->m_Name << "." << "front" << ">" << std::endl;

    // Camera.Up    
    fileOut << "\t" << "\t" << "\t" << "<" << camera->m_Name << "." << "up" << ">" << std::endl;
    OutputItemXML(fileOut, 4, camera->m_Name, camera->m_Up.x, "up.x");
    OutputItemXML(fileOut, 4, camera->m_Name, camera->m_Up.y, "up.y");
    OutputItemXML(fileOut, 4, camera->m_Name, camera->m_Up.z, "up.z");
    fileOut << "\t" << "\t" << "\t" << "</" << camera->m_Name << "." << "up" << ">" << std::endl;

    // Camera.Right
    fileOut << "\t" << "\t" << "\t" << "<" << camera->m_Name << "." << "right" << ">" << std::endl;
    OutputItemXML(fileOut, 4, camera->m_Name, camera->m_Right.x, "right.x");
    OutputItemXML(fileOut, 4, camera->m_Name, camera->m_Right.y, "right.y");
    OutputItemXML(fileOut, 4, camera->m_Name, camera->m_Right.z, "right.z");
    fileOut << "\t" << "\t" << "\t" << "</" << camera->m_Name << "." << "right" << ">" << std::endl;

    OutputItemXML(fileOut, 3, camera->m_Name, camera->m_Yaw, "yaw");
    OutputItemXML(fileOut, 3, camera->m_Name, camera->m_Pitch, "pitch");
    OutputItemXML(fileOut, 3, camera->m_Name, camera->m_Fov, "fov");
    OutputItemXML(fileOut, 3, camera->m_Name, camera->m_MovementSpeed, "movementSpeed");
    OutputItemXML(fileOut, 3, camera->m_Name, camera->m_Sensitivity, "sensitivity");

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
        
        for (uint32 meshIndex = 0; meshIndex < scene->m_Models[modelIndex].m_Meshes.size(); meshIndex++)
        {
            // Start of Mesh
            fileOut << "\t" << "\t" << "\t" << "<" << "mesh" << " " << "id=\"" << ++id << "\"" << " " << "version" << "=\"" << version << "\"" << ">" << std::endl;
            fileOut << "\t" << "\t" << "\t" << "\t" << "<" << "count" << ">" << scene->m_Models[modelIndex].m_Meshes.size() << "</" << "count" << ">" << std::endl;
            fileOut << "\t" << "\t" << "\t" << "\t" << "<" << "item" << " " << "id=\"" << ++id << "\"" << " " << "version" << "=\"" << version << "\"" << ">" << std::endl;

            OutputItemXML(fileOut, 5, "mesh", scene->m_Models[modelIndex].m_Meshes[meshIndex].m_Name, "name");

            // Vertices
            fileOut << "\t" << "\t" << "\t" << "\t" << "\t" << "<" << "mesh" << "." << "vertices" << ">" << std::endl;
            for (const auto vertex : scene->m_Models[modelIndex].m_Meshes[meshIndex].m_Vertices)
            {
                OutputItemXML(fileOut, 6, "mesh", vertex.position.x, "vertices.position.x");
                OutputItemXML(fileOut, 6, "mesh", vertex.position.y, "vertices.position.y");
                OutputItemXML(fileOut, 6, "mesh", vertex.position.z, "vertices.position.z");
                OutputItemXML(fileOut, 6, "mesh", vertex.normal.x, "vertices.normal.x");
                OutputItemXML(fileOut, 6, "mesh", vertex.normal.y, "vertices.normal.y");
                OutputItemXML(fileOut, 6, "mesh", vertex.normal.z, "vertices.normal.z");
                OutputItemXML(fileOut, 6, "mesh", vertex.textureUV.x, "vertices.textureUV.x");
                OutputItemXML(fileOut, 6, "mesh", vertex.textureUV.y, "vertices.textureUV.y");
            }
            fileOut << "\t" << "\t" << "\t" << "\t" << "\t" << "</" << "mesh" << "." << "vertices" << ">" << std::endl;

            // Indices
            fileOut << "\t" << "\t" << "\t" << "\t" << "\t" << "<" << "mesh" << "." << "indices" << ">" << std::endl;
            for (const auto index : scene->m_Models[modelIndex].m_Meshes[meshIndex].m_Indices)
            {
                OutputItemXML(fileOut, 6, "mesh", index, "indices");
            }
            fileOut << "\t" << "\t" << "\t" << "\t" << "\t" << "</" << "mesh" << "." << "indices" << ">" << std::endl;

            // Material
            fileOut << "\t" << "\t" << "\t" << "\t" << "\t" << "<" << "mesh" << "." << "material" << ">" << std::endl;

            OutputItemXML(fileOut, 6, "mesh", scene->m_Models[modelIndex].m_Meshes[meshIndex].m_Material.m_Name, "name");

            // Material - Ambient
            fileOut << "\t" << "\t" << "\t" << "\t" << "\t" << "\t" << "<" << "mesh" << "." << "material.ambient" << ">" << std::endl;
            OutputItemXML(fileOut, 7, "mesh", scene->m_Models[modelIndex].m_Meshes[meshIndex].m_Material.m_Ambient.x, "material.ambient.x");
            OutputItemXML(fileOut, 7, "mesh", scene->m_Models[modelIndex].m_Meshes[meshIndex].m_Material.m_Ambient.y, "material.ambient.y");
            OutputItemXML(fileOut, 7, "mesh", scene->m_Models[modelIndex].m_Meshes[meshIndex].m_Material.m_Ambient.z, "material.ambient.z");
            fileOut << "\t" << "\t" << "\t" << "\t" << "\t" << "\t" << "</" << "mesh" << "." << "material.ambient" << ">" << std::endl;

            // Material - Diffuse
            fileOut << "\t" << "\t" << "\t" << "\t" << "\t" << "\t" << "<" << "mesh" << "." << "material.diffuse" << ">" << std::endl;
            OutputItemXML(fileOut, 7, "mesh", scene->m_Models[modelIndex].m_Meshes[meshIndex].m_Material.m_Diffuse.x, "material.diffuse.x");
            OutputItemXML(fileOut, 7, "mesh", scene->m_Models[modelIndex].m_Meshes[meshIndex].m_Material.m_Diffuse.y, "material.diffuse.y");
            OutputItemXML(fileOut, 7, "mesh", scene->m_Models[modelIndex].m_Meshes[meshIndex].m_Material.m_Diffuse.z, "material.diffuse.z");
            fileOut << "\t" << "\t" << "\t" << "\t" << "\t" << "\t" << "</" << "mesh" << "." << "material.diffuse" << ">" << std::endl;

            // Material - Specular
            fileOut << "\t" << "\t" << "\t" << "\t" << "\t" << "\t" << "<" << "mesh" << "." << "material.specular" << ">" << std::endl;
            OutputItemXML(fileOut, 7, "mesh", scene->m_Models[modelIndex].m_Meshes[meshIndex].m_Material.m_Specular.x, "material.specular.x");
            OutputItemXML(fileOut, 7, "mesh", scene->m_Models[modelIndex].m_Meshes[meshIndex].m_Material.m_Specular.y, "material.specular.y");
            OutputItemXML(fileOut, 7, "mesh", scene->m_Models[modelIndex].m_Meshes[meshIndex].m_Material.m_Specular.z, "material.specular.z");
            fileOut << "\t" << "\t" << "\t" << "\t" << "\t" << "\t" << "</" << "mesh" << "." << "material.specular" << ">" << std::endl;

            // Material - Emissive
            fileOut << "\t" << "\t" << "\t" << "\t" << "\t" << "\t" << "<" << "mesh" << "." << "material.emissive" << ">" << std::endl;
            OutputItemXML(fileOut, 7, "mesh", scene->m_Models[modelIndex].m_Meshes[meshIndex].m_Material.m_Emissive.x, "material.emissive.x");
            OutputItemXML(fileOut, 7, "mesh", scene->m_Models[modelIndex].m_Meshes[meshIndex].m_Material.m_Emissive.y, "material.emissive.y");
            OutputItemXML(fileOut, 7, "mesh", scene->m_Models[modelIndex].m_Meshes[meshIndex].m_Material.m_Emissive.z, "material.emissive.z");
            fileOut << "\t" << "\t" << "\t" << "\t" << "\t" << "\t" << "</" << "mesh" << "." << "material.emissive" << ">" << std::endl;

            OutputItemXML(fileOut, 6, "mesh", scene->m_Models[modelIndex].m_Meshes[meshIndex].m_Material.m_Shininess, "material.shininess");

            // Textures
            for (uint32 textureIndex = 0; 
                 textureIndex < scene->m_Models[modelIndex].m_Meshes[meshIndex].m_Material.m_Textures.size(); 
                 textureIndex++)
            {
                fileOut << "\t" << "\t" << "\t" << "\t" << "\t" << "\t" << "<" << "mesh" << "." << "material.texture" << ">" << std::endl;
                OutputItemXML(fileOut, 7, "mesh", scene->m_Models[modelIndex].m_Meshes[meshIndex].m_Material.m_Textures[textureIndex].m_FilePath, "material.texture.filepath");
                OutputItemXML(fileOut, 7, "mesh", scene->m_Models[modelIndex].m_Meshes[meshIndex].m_Material.m_Textures[textureIndex].m_Type, "material.texture.type");
                fileOut << "\t" << "\t" << "\t" << "\t" << "\t" << "\t" << "</" << "mesh" << "." << "material.texture" << ">" << std::endl;
            }
            // End of Material
            fileOut << "\t" << "\t" << "\t" << "\t" << "\t" << "</" << "mesh" << "." << "material" << ">" << std::endl;

            // End of Mesh
            fileOut << "\t" << "\t" << "\t" << "\t" << "</" << "item" << ">" << std::endl;
            fileOut << "\t" << "\t" << "\t" << "</" << "mesh" << ">" << std::endl;
        }
        // End of Model
        fileOut << "\t" << "\t" << "</" << "item" << ">" << std::endl;
        fileOut << "\t" << "</" << "model" << ">" << std::endl;
    }
    // End of Scene - Root
    fileOut << "</" << "scene" << ">" << std::endl; 

    return true;
}

void OutputItemXML(std::fstream& output, int32 indents, const std::string& name, const std::string& value, const std::string& attributes)
{
    for (int i = 0; i < indents; ++i)
        output << "\t";
    output << "<" << name;
    if (attributes != "")
        OutputAttributesXML(output, attributes);
    output << ">";
    output << value;
    output << "</" << name;
    if (attributes != "")
        OutputAttributesXML(output, attributes);
    output << ">" << std::endl;
}

void OutputItemXML(std::fstream& output, int32 indents, const std::string& name, const uint32 value, const std::string& attributes)
{
    for (int i = 0; i < indents; ++i)
        output << "\t";
    output << "<" << name;
    if (attributes != "")
        OutputAttributesXML(output, attributes);
    output << ">";
    output << value;
    output << "</" << name;
    if (attributes != "")
        OutputAttributesXML(output, attributes);
    output << ">" << std::endl;
}

void OutputItemXML(std::fstream& output, int32 indents, const std::string& name, const real32 value, const std::string& attributes)
{
    for(int i = 0; i < indents; ++i)
        output << "\t";
    output << "<" << name;
    if(attributes != "")
        OutputAttributesXML(output, attributes);
    output << ">";
    output << value;
    output << "</" << name;
    if(attributes != "")
        OutputAttributesXML(output, attributes);
    output << ">" << std::endl;
}

void OutputAttributesXML(std::fstream& output, const std::string& attributes)
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

    while(!fileStream.eof())
    {
        std::string line = "";
        std::string token = "";
        std::string value = "";
        std::getline(fileStream, line);

        OutputDebugStringA(line.c_str());
        OutputDebugString((LPCWSTR)"\n");

        if (line.compare("<?xml version=\"1.0\" encoding=\"UTF-8\"?>") == 0)
            continue;

        char prevChar;
        int32 count = 0;
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
                // Count
                if (token.compare("count") == 0)
                    count = std::stoi(value);
                                    
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

    return true;
}

}