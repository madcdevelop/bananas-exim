#pragma once

#include "Common.h"
#include "Mesh.h"

namespace Core
{

class Model
{
public:
    std::vector<Mesh> m_Meshes;

public:
    Model(const std::vector<Mesh> meshes);
    ~Model();

};

}