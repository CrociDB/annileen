#include <engine/scene.h>
#include <engine/serviceprovider.h>

namespace annileen
{
    SceneNodePtr Scene::getRoot()
    {
        return m_Root;
    }

    void Scene::addNodeToList(SceneNodePtr node)
    {
        m_Nodes.push_back(node);
    }

    void Scene::removeNodeFromList(SceneNodePtr node)
    {
        m_Nodes.remove(node);
    }

    void Scene::clearNodes()
    {
        if (m_Root != nullptr)
        {
            delete m_Root;
        }
        m_Root = new SceneNode();
        m_Nodes.clear();
    }

    std::list<SceneNodePtr>& Scene::getNodeList()
    {
        return m_Nodes;
    }

    std::list<Light*>& Scene::getLightList()
    {
        return m_Lights;
    }

    std::list<Camera*>& Scene::getCameraList()
    {
        return m_Cameras;
    }

    Camera* Scene::getCamera()
    {
        for (auto camera : m_Cameras)
        {
            if (camera->enabled)
            {
                return camera;
            }
        }

        ANNILEEN_LOG_ERROR(LoggingChannel::General, "Cannot find an active camera in the scene.");
        return nullptr;
    }

    Scene::Scene() : fog()
    {
        //m_Camera = new Camera(60.0f, 0.1f, 300.0f);
        m_Root = new SceneNode();
    }

    Scene::~Scene()
    {
        if (m_Skybox != nullptr) delete m_Skybox;
        delete m_Root;
    }
}

