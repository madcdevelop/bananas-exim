#ifndef GRAPHICS_ENGINE_EXPORTER_H
#define GRAPHICS_ENGINE_EXPORTER_H

#include "../Core/Common.h"
#include "Model.h"

namespace GraphicsEngine
{

class Exporter
{

public:
    Exporter();
    ~Exporter();

    bool ExportModel(const std::string& filePath, std::vector<Model>& models);

};

}

#endif