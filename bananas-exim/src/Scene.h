#pragma once

#include "Common.h"
#include "Model.h"

namespace Core
{

class Scene
{
public:
    std::vector<Model> m_Models;

public:
    Scene();
    ~Scene();

private:
    void LoadModels();

};

}
