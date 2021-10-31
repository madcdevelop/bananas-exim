#ifndef CORE_ENGINE_SERIALIZE_H
#define CORE_ENGINE_SERIALIZE_H

#include "Common.h"
#include "../Graphics/Scene.h"

namespace CoreEngine
{

#define XML_ATTRIBUTE_MAX_SIZE 2
#define XML_CHAR_LENGTH 10
#define TAB_SIZE 2

#define CURLY_BRACKET_START '{'
#define CURLY_BRACKET_END '}'
#define SQUARE_BRACKET_START '['
#define SQUARE_BRACKET_END ']'

#define YAML_DASH_START "- "
#define YAML_SPACE_START "  "

struct XMLAttribute
{
    int32 attributeCount;
    char name[XML_ATTRIBUTE_MAX_SIZE][XML_CHAR_LENGTH];
    int32 value[XML_ATTRIBUTE_MAX_SIZE];
};

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

void SerializeItemJSON(std::fstream& output, int32 indents, const std::string& key, const std::string& value, bool comma);
void SerializeItemJSON(std::fstream& output, int32 indents, const std::string& key, const uint32 value, bool comma);
void SerializeItemJSON(std::fstream& output, int32 indents, const std::string& key, const real32 value, bool comma);

void SerializeItemNoKeyJSON(std::fstream& output, int32 indents, const uint32 value, bool comma);

// YAML
bool SerializeToYAML(const std::string& filePath, GraphicsEngine::Scene* scene);

void SerializeBeginYAML(std::fstream& output, int32 indents, const std::string& name);
void SerializeItemYAML(std::fstream& output, int32 indents, const std::string& key, const std::string& value, const char* start);
void SerializeItemYAML(std::fstream& output, int32 indents, const std::string& key, uint32 value, const char* start);
void SerializeItemYAML(std::fstream& output, int32 indents, const std::string& key, real32 value, const char* start);

void SerializeVector3DYAML(std::fstream& output, int32 indents, const std::string& key, real32 x, real32 y, real32 z, const char* start);
void SerializeVector2DYAML(std::fstream& output, int32 indents, const std::string& key, real32 x, real32 y, const char* start);

// Helper
void SerializeInsertAfter(const std::string& lhs, std::string& rhs, const char r, const char i);

}

#endif