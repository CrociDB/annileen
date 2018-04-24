#include "scene.h"

void Scene::addMesh(Mesh* mesh)
{
    mesh->genBuffers();
    m_Meshes.push_back(std::shared_ptr<Mesh>(mesh));
}

std::list<std::shared_ptr<Mesh>>& Scene::getMeshList()
{
    return m_Meshes;
}

Camera* Scene::getCamera()
{
    return m_Camera;
}

Scene::Scene()
{
    m_Camera = new Camera(60.0f, 0.1f, 100.0f);
}

Scene::~Scene()
{
    delete m_Camera;
}
