#include "scene.h"

namespace annileen
{
    SceneNodePtr Scene::getRoot()
    {
        return m_Root;
    }

    SceneNodePtr Scene::createNode()
    {
        SceneNodePtr node = new SceneNode();
        m_Nodes.push_back(node);
        node->setParentScene(this);
        node->setParent(m_Root);
        return node;
    }

    void Scene::removeNode(SceneNodePtr node)
    {
        m_Nodes.remove(node);
        delete node;
    }

    void Scene::addLight(Light* light)
    {
        m_Lights.push_back(light);
    }

    void Scene::clearNodeList()
    {
        /*for (auto node : m_Root->getChildren())
        {
            delete node;
        }*/

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

    Camera* Scene::getCamera()
    {
        return m_Camera;
    }

    Scene::Scene()
    {
        m_Camera = new Camera(60.0f, 0.1f, 300.0f);
        m_Root = new SceneNode();
    }

    Scene::~Scene()
    {
        if (m_Skybox != nullptr) delete m_Skybox;
        delete m_Camera;
        delete m_Root;
    }
}

