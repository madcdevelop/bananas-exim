#ifndef CORE_ENGINE_SERIALIZE_H
#define CORE_ENGINE_SERIALIZE_H

#include "Common.h"
#include "../Graphics/Scene.h"

namespace CoreEngine
{

#define XML_ATTRIBUTE_MAX_SIZE 2
#define XML_CHAR_LENGTH 10

struct XMLAttribute
{
    int32 attributeCount;
    char name[XML_ATTRIBUTE_MAX_SIZE][XML_CHAR_LENGTH];
    int32 value[XML_ATTRIBUTE_MAX_SIZE];
};

bool SerializeToXML(const std::string& filePath, GraphicsEngine::Scene* scene);

void SerializeBeginTagXML(std::fstream &output, int32 indents, const std::string &tag);
void SerializeBeginTagXMLAttributes(std::fstream &output, int32 indents, const std::string &tag, XMLAttribute attribute);
void SerializeEndTagXML(std::fstream& output, int32 indents, const std::string& tag);

void SerializeItemXML(std::fstream& output, int32 indents, const std::string& tag, const std::string& value);
void SerializeItemXML(std::fstream& output, int32 indents, const std::string& tag, const uint32 value);
void SerializeItemXML(std::fstream& output, int32 indents, const std::string& tag, const real32 value);


bool DeSerializeFromXML(const std::string& filePath, GraphicsEngine::Scene* scene);
}

#endif