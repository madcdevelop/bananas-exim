#ifndef CORE_ENGINE_SERIALIZE_H
#define CORE_ENGINE_SERIALIZE_H

#include "Common.h"
#include "../Graphics/Scene.h"

namespace CoreEngine
{

bool SerializeToXML(const std::string& filePath, GraphicsEngine::Scene* scene);
void SerializeItemXML(std::fstream& output, int32 indents, const std::string& name, const std::string& value, const std::string& attributes);
void SerializeItemXML(std::fstream& output, int32 indents, const std::string& name, const uint32 value, const std::string& attributes);
void SerializeItemXML(std::fstream& output, int32 indents, const std::string& name, const real32 value, const std::string& attributes);
void SerializeAttributesXML(std::fstream& output, const std::string& attributes);

bool DeSerializeFromXML(const std::string& filePath, GraphicsEngine::Scene* scene);

}

#endif