#include "scene.h"

void Scene::addMesh(Mesh* mesh)
{
    m_Meshes.push_back(std::shared_ptr<Mesh>(mesh));
}

std::list<std::shared_ptr<Mesh>>& Scene::getMeshList()
{
    return m_Meshes;
}

Scene::Scene()
{
}

Scene::~Scene()
{
}
