#include "Scene.h"

#include "Importer.h"

namespace Core
{

Scene::Scene()
{
    LoadModels();
}

Scene::~Scene()
{

}

void Scene::LoadModels()
{
    // Load Models
    std::vector<std::string> outNames;
    std::vector<std::vector<Vertex>> outVertices;
    std::vector<std::vector<unsigned int>> outIndices;
    std::vector<unsigned int> outMeshSizes;
    std::vector<Material> outMaterials;
    Importer import;
    import.LoadModel("C:\\Code\\bananas-exim\\bananas-exim\\content\\models\\minecraft_hill.obj", outNames, outVertices, outIndices, outMeshSizes, outMaterials);

    Model model;
    // TODO: Optimize to use same material for each mesh instead 
    // of creating new ones per mesh if they are the same material.
    for(unsigned int i = 0; i < outMeshSizes.size(); i++)
    {
        Mesh mesh{outNames[i], outVertices[i], outIndices[i], outMaterials[i]};
        model.m_Meshes.push_back(mesh);
    }
    // TODO: Loop to add more than 1 model
    m_Models.push_back(model);

    // Load Textures
    m_Models[0].LoadTextures();
}

}