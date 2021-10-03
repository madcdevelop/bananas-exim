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
    
    // Start of Camera and Item
    GraphicsEngine::Camera* camera = &scene->m_Camera;
    fileOut << "<" << camera->m_Name << " " << "id=\"" << id << "\"" << " " << "version=\"" << version << "\"" << ">" << std::endl; // <camera>
    fileOut << "\t" << "<" << "count" << ">" << 1 << "</" << "count" << ">" << std::endl; // count
    fileOut << "\t" << "<" << "item" << " " << "id=\"" << ++id << "\"" << " " << "version" << "=\"" << version << "\"" << ">" << std::endl; // <item>
    
    // Camera.Position
    fileOut << "\t" << "\t" << "<" <<  camera->m_Name << "." << "position" << ">" << std::endl;
    OutputItemXML(fileOut, 3, camera->m_Name, camera->m_Position.x, "position.x");
    OutputItemXML(fileOut, 3, camera->m_Name, camera->m_Position.y, "position.y");
    OutputItemXML(fileOut, 3, camera->m_Name, camera->m_Position.z, "position.z");
    fileOut << "\t" << "\t" << "</" << camera->m_Name << "." << "position" << ">" << std::endl;

    // Camera.Front
    fileOut << "\t" << "\t" << "<" <<  camera->m_Name << "." << "front" << ">" << std::endl;
    OutputItemXML(fileOut, 3, camera->m_Name, camera->m_Front.x, "front.x");
    OutputItemXML(fileOut, 3, camera->m_Name, camera->m_Front.y, "front.y");
    OutputItemXML(fileOut, 3, camera->m_Name, camera->m_Front.z, "front.z");
    fileOut << "\t" << "\t" << "</" << camera->m_Name << "." << "front" << ">" << std::endl;

    // Camera.Up    
    fileOut << "\t" << "\t" << "<" <<  camera->m_Name << "." << "up" << ">" << std::endl;
    OutputItemXML(fileOut, 3, camera->m_Name, camera->m_Up.x, "up.x");
    OutputItemXML(fileOut, 3, camera->m_Name, camera->m_Up.y, "up.y");
    OutputItemXML(fileOut, 3, camera->m_Name, camera->m_Up.z, "up.z");
    fileOut << "\t" << "\t" << "</" << camera->m_Name << "." << "up" << ">" << std::endl;

    // Camera.Right
    fileOut << "\t" << "\t" << "<" <<  camera->m_Name << "." << "right" << ">" << std::endl;
    OutputItemXML(fileOut, 3, camera->m_Name, camera->m_Right.x, "right.x");
    OutputItemXML(fileOut, 3, camera->m_Name, camera->m_Right.y, "right.y");
    OutputItemXML(fileOut, 3, camera->m_Name, camera->m_Right.z, "right.z");
    fileOut << "\t" << "\t" << "</" << camera->m_Name << "." << "right" << ">" << std::endl;

    OutputItemXML(fileOut, 2, camera->m_Name, camera->m_Yaw, "yaw");
    OutputItemXML(fileOut, 2, camera->m_Name, camera->m_Pitch, "pitch");
    OutputItemXML(fileOut, 2, camera->m_Name, camera->m_Fov, "fov");
    OutputItemXML(fileOut, 2, camera->m_Name, camera->m_MovementSpeed, "movementSpeed");
    OutputItemXML(fileOut, 2, camera->m_Name, camera->m_Sensitivity, "sensitivity");

    // End of Item
    fileOut << "\t" << "</" << "item" << ">" << std::endl; // </item>

    // End of Camera
    fileOut << "</" << camera->m_Name << ">" << std::endl; // </camera>

    return true;
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