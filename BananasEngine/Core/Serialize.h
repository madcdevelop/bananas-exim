#ifndef CORE_ENGINE_SERIALIZE_H
#define CORE_ENGINE_SERIALIZE_H

#include "Common.h"
#include "../Graphics/Scene.h"

namespace CoreEngine
{

bool SerializeToXML(const std::string& filePath, GraphicsEngine::Scene* scene);
void OutputItemXML(std::fstream& output, int32 indents, const std::string& name, const real32 value, const std::string& attributes);
void OutputAttributesXML(std::fstream& output, const std::string& attributes);

bool DeSerializeFromXML(const std::string& filePath, GraphicsEngine::Scene* scene);

}

#endif