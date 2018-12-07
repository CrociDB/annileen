#include "scene.h"

void Scene::setCubemap(Cubemap* cubemap)
{
    m_Cubemap = cubemap;

    createSkybox();
}

Cubemap* Scene::getCubemap() const
{
    return m_Cubemap;
}

Skybox * Scene::getSkybox() const
{
    return m_Skybox;
}

void Scene::createSkybox()
{
    m_Skybox = new Skybox(m_Cubemap);
    m_Skybox->createMesh();
}

void Scene::addMesh(Mesh* mesh)
{
    mesh->genBuffers();
    m_Meshes.push_back(mesh);
}

void Scene::removeMesh(Mesh* mesh)
{
    m_Meshes.remove(mesh);
}

std::list<Mesh*>& Scene::getMeshList()
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
    delete m_Cubemap;
    delete m_Skybox;
    delete m_Camera;
}
