#include "Scene.h"

#include "Importer.h"

namespace Core
{

Scene::Scene()
{
}

Scene::~Scene()
{

}

void Scene::LoadModels()
{
    // TODO: Loop to add more than 1 model
    Model model1;
    model1.LoadModel("C:\\Code\\bananas-exim\\bananas-exim\\content\\models\\minecraft_hill.obj");
    m_Models.push_back(model1);

    Model lightCube;
    lightCube.LoadModel("C:\\Code\\bananas-exim\\bananas-exim\\content\\models\\cube_example_triangle.obj");
    m_Models.push_back(lightCube);

    for(auto& model : m_Models)
    {
        model.LoadTextures();
    }
}

}