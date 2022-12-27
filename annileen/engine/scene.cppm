
#include <iostream>
#include <list>
#include <memory>
#include <glm.hpp>
#include "engine/forward_decl.h"

import scene;
import scenenode;
import camera;
import skybox;

namespace annileen
{
    SceneNodePtr Scene::getRoot()
    {
        return m_Root;
    }

    SceneNodePtr Scene::createNode(const std::string& name)
    {
        auto node = new SceneNode(name);
        node->setParentScene(this);
        node->setParent(getRoot());
        return node;
    }

    void Scene::destroyNode(SceneNodePtr node)
    {
        delete node;
    }

    void Scene::addNodeToList(SceneNodePtr node)
    {
        m_Nodes.push_back(node);
    }

    void Scene::removeNodeFromList(SceneNodePtr node)
    {
        m_Nodes.remove(node);
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
            if (camera->getSceneNode()->getActive() && camera->enabled)
            {
                return camera;
            }
        }
        return nullptr;
    }

    Scene::Scene() : fog()
    {
        //m_Camera = new Camera(60.0f, 0.1f, 300.0f);
        m_Root = new SceneNode("Root");
    }

    Scene::~Scene()
    {
        if (m_Skybox != nullptr) delete m_Skybox;
        delete m_Root;
    }
}
