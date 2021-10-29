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

#define CURLY_BRACKET_START '{'
#define CURLY_BRACKET_END '}'
#define SQUARE_BRACKET_START '['
#define SQUARE_BRACKET_END ']'
#define COMMA ','
#define NO_COMMA ""

// XML
bool SerializeToXML(const std::string& filePath, GraphicsEngine::Scene* scene);

void SerializeBeginTagXML(std::fstream &output, int32 indents, const std::string &tag);
void SerializeBeginTagXMLAttributes(std::fstream &output, int32 indents, const std::string &tag, XMLAttribute attribute);
void SerializeEndTagXML(std::fstream& output, int32 indents, const std::string& tag);

void SerializeItemXML(std::fstream& output, int32 indents, const std::string& tag, const std::string& value);
void SerializeItemXML(std::fstream& output, int32 indents, const std::string& tag, const uint32 value);
void SerializeItemXML(std::fstream& output, int32 indents, const std::string& tag, const real32 value);

bool DeSerializeFromXML(const std::string& filePath, GraphicsEngine::Scene* scene);

// JSON
bool SerializeToJSON(const std::string &filePath, GraphicsEngine::Scene *scene);

void SerializeBeginJSON(std::fstream& output, int32 indents, char bracket);
void SerializeBeginObjectJSON(std::fstream& output, int32 indents, const std::string& object, char bracket);
void SerializeEndJSON(std::fstream& output, int32 indents, char bracket, bool comma);

void SerializeItemJSON(std::fstream& output, int32 indents, const std::string& key, real32 value, bool comma);
void SerializeItemJSON(std::fstream& output, int32 indents, const std::string& key, int32 value, bool comma);
void SerializeItemJSON(std::fstream& output, int32 indents, const std::string& key, const std::string& value, bool comma);

// YAML
bool SerializeToYAML(const std::string& filePath, GraphicsEngine::Scene* scene);

}

#endif