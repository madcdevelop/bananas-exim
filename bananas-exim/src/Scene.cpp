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
    Model model;
    model.LoadModel("C:\\Code\\bananas-exim\\bananas-exim\\content\\models\\minecraft_hill.obj");
    m_Models.push_back(model);

    m_Models[0].LoadTextures();
}

}