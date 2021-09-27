#include "Serialize.h"

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

}